#include <cpprest/http_listener.h>
#include <cpprest/uri.h>
#include <cpprest/asyncrt_utils.h>
#include <cpprest/json.h>
#include <cpprest/filestream.h>
#include <cpprest/containerstream.h>
#include <cpprest/producerconsumerstream.h>

class handler
{
  public:
    handler( web::http::uri uri )
      :  m_listener{ std::move( uri ) }
    {
      m_listener.support(
        web::http::methods::GET,
        []( web::http::http_request message ){

          message.reply( web::http::status_codes::OK, "Hello world!" )
            .then( [](pplx::task<void> t)
            {
              try{
                t.get();
              }
              catch(...){
                //
              }
            } );
        } );
    }

    pplx::task<void> open(){ return m_listener.open(); }
    pplx::task<void> close(){ return m_listener.close(); }

  private:
    web::http::experimental::listener::http_listener m_listener;
};

int main( int argc, char *argv[] )
{
  utility::string_t port = "8080";

  if(argc == 2)
  {
    port = argv[1];
  }

  utility::string_t address{ "http://localhost:" };
  address.append( port );

  handler h( address );
  h.open().wait();
  std::cout << "Listening for requests at: " << address << std::endl;

  std::cout << "Press ENTER to exit." << std::endl;
  std::string line;
  std::getline( std::cin, line );

  h.close().wait();

  return 0;
}
