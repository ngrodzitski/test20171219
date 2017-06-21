#include <string>
#include <memory>
#include <cstdlib>
#include <stdexcept>

#include <restbed>

using namespace std;
using namespace restbed;

void get_method_handler( const shared_ptr< Session > session )
{
    const auto request = session->get_request( );

    session->yield( OK, "Hello world!", { { "Content-Length", "12" }, { "Connection", "keep-alive" } } );
}

void error_handler( const int status_code, const exception error, const shared_ptr< Session > session )
{
    if ( session not_eq nullptr and session->is_open( ) )
    {
        string message = error.what( );
        message.push_back( '\n' );

        //we must leave the socket intact on error,
        //otherwise follow up messages will be lost.
        session->yield( status_code, message, { { "Content-Length", std::to_string( message.length( ) ) }, { "Connection", "keep-alive" } } );
    }
}

int main( const int, const char** )
{
    auto resource = make_shared< Resource >( );
    resource->set_path( "/" );
    resource->set_method_handler( "GET", get_method_handler );

    auto settings = make_shared< Settings >( );
    settings->set_port( 8080 );

    Service service;
    service.publish( resource );
    service.set_error_handler( error_handler );
    service.start( settings );

    return EXIT_SUCCESS;
}
