#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <thread>

#include <asio.hpp>

#include <args.hxx>

//
// app_args_t
//

struct app_args_t
{
	bool m_help{ false };
	std::size_t m_asio_pool_size{ 1 };

	app_args_t( int argc, const char * argv[] )
	{
		args::ArgumentParser parser( "Single handler thin asio wrapper impl", "" );
		args::HelpFlag help( parser, "Help", "Usage example", { 'h', "help" } );

		args::ValueFlag< std::string > config_file(
				parser, "config_file", "Configuration file",
				{ 'c', "config" } );
		args::ValueFlag< std::size_t > asio_pool_size(
				parser, "asio_pool_size",
				"Size of worker thread pool for Asio",
				{ 'n', "asio-pool-size" } );

		try
		{
			parser.ParseCLI( argc, argv );
		}
		catch( const args::Help & )
		{
			m_help = true;
			std::cout << parser;
		}

		if( asio_pool_size )
			m_asio_pool_size = args::get( asio_pool_size );
	}
};

const std::string RAW_RESP{
	"HTTP/1.1 200 OK\r\n"
	"Connection: keep-alive\r\n"
	"Content-Length: 12\r\n"
	"Server: Pure asio bench server\r\n"
	"Content-Type: text/plain; charset=utf-8\r\n"
	"\r\n"
	"Hello world!" };

class input_buffer_t
{
	public:
		auto
		make_asio_buffer()
		{
			return asio::buffer( m_buf.data(), m_buf.size() );
		}

		void
		obtained_bytes( std::size_t length )
		{
			m_ready_length = length; // Current bytes in buffer.
			m_ready_pos = 0; // Reset current pos.
		}

		void
		consumed_bytes( std::size_t length )
		{
			m_ready_length -= length; // decrement buffer length.
			m_ready_pos += length; // Shift current pos.
		}

		std::size_t
		length() const
		{
			return m_ready_length;
		}

		const char *
		bytes() const
		{
			return m_buf.data() + m_ready_pos;
		}

	private:
		std::array< char, 4096 > m_buf;
		std::size_t m_ready_pos{0};
		std::size_t m_ready_length{0};
};

class connection_t
	:	public std::enable_shared_from_this< connection_t >
{
	public:
		connection_t(
			asio::ip::tcp::socket && socket )
			:	m_socket{ std::move( socket ) }
		{}

		void
		wait_for_http_message()
		{
			if( 0 != m_input_buffer.length() )
			{
				// If a pipeline requests were sent by client
				// then the biginning (or even entire request) of it
				// is in the buffer obtained from socket in previous
				// read operation.
				consume_data( m_input_buffer.bytes(), m_input_buffer.length() );
			}
			else
			{
				// Next request (if any) must be obtained from socket.
				consume_message();
			}
		}

	private:
		void
		consume_message()
		{
			m_socket.async_read_some(
				m_input_buffer.make_asio_buffer(),
				[ this, ctx = shared_from_this() ]( auto ec, std::size_t length ){
					this->after_read( ec, length );
				} );
		}

		void
		after_read( const std::error_code & ec, std::size_t length )
		{
			if( !ec )
			{
				m_input_buffer.obtained_bytes( length );
				consume_data( m_input_buffer.bytes(), length );
			}
			else
				m_socket.close();
		}

		void
		consume_data( const char * data, std::size_t length )
		{
			bool message_complete = false;
			const char * d = data;

			// Assume "\r\n\r\n" - the end of message will never happened to
			// be split in 2 read operations.
			for( ; d <= data + length - 4; ++d )
			{
				if( d[ 0 ] == '\r' &&
					d[ 1 ] == '\n' &&
					d[ 2 ] == '\r' &&
					d[ 3 ] == '\n' )
				{
					message_complete = true;
					break;
				}
			}

			if( message_complete )
			{
				m_input_buffer.consumed_bytes( d - data + 4 );
				write_response_message();
			}
			else
			{
				m_input_buffer.consumed_bytes( length );
				consume_message();
			}
		}

		void
		write_response_message()
		{
			asio::async_write(
				m_socket,
				asio::buffer( RAW_RESP.data(), RAW_RESP.size() ),
				[ this, ctx = shared_from_this() ]
					( auto & ec, std::size_t ){
						if( ec )
						{
							if( ec != asio::error::operation_aborted )
							{
								m_socket.close();
							}
						}
						else
						{
							ctx->wait_for_http_message();
						}
				} );
		}
		asio::ip::tcp::socket m_socket;
		input_buffer_t m_input_buffer;
};

class http_server_t
{
	public:
		http_server_t( std::size_t asio_pool_size )
		{
			asio::ip::tcp::endpoint ep{
				asio::ip::tcp::v4(),
				8080 };

			ep.address( asio::ip::address::from_string( "127.0.0.1" ) );

			m_acceptor.open( ep.protocol() );

			m_acceptor.set_option(
				asio::ip::tcp::acceptor::reuse_address( true ) );

			m_acceptor.bind( ep );
			m_acceptor.listen( asio::socket_base::max_connections );

			accept_next();

			m_asio_pool.reserve( asio_pool_size );
			for( std::size_t i = 0; i < asio_pool_size; ++i )
				m_asio_pool.emplace_back( [&] { m_io_service.run(); });
		}

		~http_server_t()
		{
			m_io_service.post( [ & ]{ m_acceptor.close(); } );

			for( auto & t : m_asio_pool )
				t.join();
		}

	private:
		void
		accept_next()
		{
			m_acceptor.async_accept(
				m_socket,
				[ this ]( auto ec ){
					if( m_acceptor.is_open() )
					{
						if( !ec )
						{
							auto conn =
								std::make_shared< connection_t >(
									std::move( m_socket ) );

							if( conn )
								conn->wait_for_http_message();
						}

						this->accept_next();
					}
				} );
		}

		asio::io_service m_io_service;
		asio::ip::tcp::acceptor m_acceptor{ m_io_service };
		asio::ip::tcp::socket m_socket{ m_io_service };

		std::vector< std::thread > m_asio_pool;
};

int
main(int argc, const char *argv[] )
{
	try
	{
		const app_args_t args{ argc, argv };

		if( !args.m_help )
		{
			http_server_t server{ args.m_asio_pool_size };

			std::cout << "Press ENTER to exit." << std::endl;
			std::string line;
			std::getline( std::cin, line );
		}
	}
	catch( const std::exception & ex )
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
