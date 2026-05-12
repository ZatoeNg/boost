//
// Created by 33722 on 2026/5/12.
//
#include <iostream>

#include "boost/asio.hpp"

namespace ip = boost::asio::ip;
const int MAX_LENGTH = 1024;

auto main() ->int {
    try {
        /*创建上下文
         * 获取对端地址与端口号
         * 创建通信通道socket
         * 发出连接请求
         */
        boost::asio::io_context context;
        ip::tcp::endpoint remote_ep(ip::make_address("127.0.0.1"), 10086);
        ip::tcp::socket socket(context);
        boost::system::error_code ec = boost::asio::error::host_not_found;
        socket.connect(remote_ep, ec);
        if (ec) {
            std::cout << "error code: " << ec.value() << std::endl;
            std::cout << ec.message() << std::endl;
            return 0;
        }

        //write:向服务器发送信息
        std::cout<<"Enter message:";
        char request[MAX_LENGTH];
        std::cin.getline(request, MAX_LENGTH);
        size_t request_length = strlen(request);
        boost::asio::write(socket,boost::asio::buffer(request, request_length));

        //read：接受服务器的信息
        char response[MAX_LENGTH];
        size_t response_length = boost::asio::read(socket
            , boost::asio::buffer(response,request_length));

        std::cout<<"response is :";
        std::cout.write(response,response_length);
        std::cout<<std::endl;

    } catch (boost::system::system_error &e) {
        std::cout << "error code: " << e.code().value() << std::endl;
        std::cout << "error message: " << e.what() << std::endl;
        // return e.code().value();
    }

    return 0;
}