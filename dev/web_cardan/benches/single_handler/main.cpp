/*
 * Copyright 2017 Alexey Syrnikov <san@masterspline.net>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstring>
#include <deque>
#include <memory>

#include <iostream>

#include "alterstack/api.hpp"

using alterstack::SocketAddrIn;
using alterstack::TcpServer;
using alterstack::TcpClient;
using alterstack::Task;

void request_processor( TcpClient* socket )
{
    std::unique_ptr<TcpClient> socket_guard( socket );
    // std::cout << "request_processor started\n";
    static const std::string message(
            "HTTP/1.1 200 OK\r\n"
            "Connection: keep-alive\r\n"
            //"Connection: close\r\n"
            "Content-Length: 12\r\n"
            "Server: Web_cardan bench server\r\n"
            "Content-Type: text/plain; charset=utf-8\r\n"
            "Accept-Ranges: bytes\r\n"
            "\r\n"
            "Hello world!" );
    static const size_t message_size = message.size();
    char buff[ 4096 ];
    ssize_t res;
    while( true )
    {
        res = socket->recv( buff, 4096 );
        if( res > 0 )
        {
            res = socket->send( message.c_str(), message_size );
        }
        else
            break;
    }
    // std::cout << "request_processor finished\n";
}

void server()
{
    std::cout << "server() started\n";
    TcpServer server_socket;

    server_socket.bind( SocketAddrIn( "::FFFF:127.0.0.1", 8080 ) );
    server_socket.listen();

    std::deque<Task> responders;

    while( true )
    {
        TcpClient* client = server_socket.accept();
        // std::cout << "client connected" << std::endl;
        responders.emplace_back(
                    [=](){ request_processor( client ); }, true );
        if( responders.size() > 64 )
        {
            responders.front().join();
            responders.pop_front();
        }
    }
    std::cout << "server() finished\n";
}

int main()
{
    Task web_server( server );
    web_server.set_priority( Task::Priority::Low );
    web_server.join();

    std::cout << "main finished\n";
    return 0;
}

