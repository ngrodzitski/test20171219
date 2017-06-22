/*
   Mathieu Stefani, 13 février 2016

   Example of an hello world server

   Adopted for benchmark with other frameworks.
*/

#include "endpoint.h"
#include "http_headers.h"

#include <args.hxx>

struct app_args
{
    bool m_help{ false };
    std::string m_config_file;
    std::size_t m_thread_pool_size{ 1 };

    app_args( int argc, const char * argv[] )
    {
        args::ArgumentParser parser( "Single handler Pistache benchmark", "" );
        args::HelpFlag help( parser, "Help", "Usage example", { 'h', "help" } );

        args::ValueFlag< std::size_t > thread_pool_size(
                parser, "thread_pool_size",
                "Size of worker thread pool",
                { 'n', "thread-pool-size" } );

        try
        {
            parser.ParseCLI( argc, argv );
        }
        catch( const args::Help & )
        {
            m_help = true;
            std::cout << parser;
        }

        if( thread_pool_size )
            m_thread_pool_size = args::get( thread_pool_size );
    }
};


using namespace Net;

class HelloHandler : public Http::Handler {
public:

    HTTP_PROTOTYPE(HelloHandler)

    void onRequest(const Http::Request& request, Http::ResponseWriter response) {
        auto m = MIME( Text, Plain );
        m.setParam( "charset", "utf-8");

        response.headers()
            .add< Http::Header::Server >("Pistache benchmark")
            .add< Http::Header::ContentType>( m );

        response.send( Http::Code::Ok, "Hello World!" );
    }
};

int main( int argc, const char * argv[] ) {
    try
    {
        const app_args args{ argc, argv };

        if( !args.m_help )
        {
            if( 1 > args.m_thread_pool_size )
                throw std::runtime_error{ "invalid thread_pool_size" };

            Net::Address addr(Ipv4::any(), Net::Port(8080));
            auto opts = Http::Endpoint::options()
                .threads( args.m_thread_pool_size );

            Http::Endpoint server(addr);
            server.init(opts);
            server.setHandler(Http::make_handler<HelloHandler>());
            server.serve();

            server.shutdown();
        }
    }
    catch( const std::exception & ex )
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
