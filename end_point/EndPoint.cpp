//================================
// Created by 33722 on 2026/5/11.
//================================
#include "EndPoint.h"

#include <iostream>
#include <string>
#include <boost/asio.hpp>

namespace asio = boost::asio;
namespace ip = asio::ip;

int client_end_point() {
    std::string remote_address = "127.4.8.1";
    unsigned short remote_port = 3333;
    boost::system::error_code ec;
    ip::address ip_address = ip::make_address(remote_address, ec);

    if (ec.value() != 0) {
        std::cout
                << "error code: " << ec.value() << "-----"
                << "error message: " << ec.message() << std::endl;
        return ec.value();
    }

    //生成端点
    ip::tcp::endpoint ep(ip_address, remote_port);

    return 0;
}

int server_end_point() {
    unsigned short prot_num = 3333;
    ip::address ip_address = ip::address_v4::any();
    ip::tcp::endpoint ep(ip_address, prot_num);

    return 0;
}

int create_tcp_socket_old() {
    asio::io_context io_context;
    ip::tcp protocol = ip::tcp::v4();
    ip::tcp::socket sock(io_context);
    boost::system::error_code ec;
    sock.open(protocol, ec);
    if (ec.value() != 0) {
        std::cout
                << "error code: " << ec.value() << "-----"
                << "error message: " << ec.message() << std::endl;
        return ec.value();
    }

    return 0;
}

int create_tcp_socket_new() {
    //TODO: not writer
    return 0;
}

int create_acceptor_socket_old() {
    asio::io_context io_context;
    asio::ip::tcp::acceptor acceptor(io_context);
    asio::ip::tcp protocol = asio::ip::tcp::v4();
    boost::system::error_code ec;
    acceptor.open(protocol, ec);
    if (ec.value() != 0) {
        std::cout
                << "error code: " << ec.value() << "-----"
                << "error message: " << ec.message() << std::endl;
        return ec.value();
    }

    return 0;
}

int create_acceptor_socket_new() {
    asio::io_context io_context;
    ip::tcp::acceptor acceptor(io_context, ip::tcp::endpoint(ip::tcp::v4(), 3333));

    return 0;
}

int bind_acceptor_socket_old() {
    unsigned short port_num = 3333;
    asio::ip::tcp::endpoint ep(ip::address_v4::any(), port_num);

    asio::io_context io_context;
    ip::tcp::acceptor acceptor(io_context, ep.protocol());
    boost::system::error_code ec;
    acceptor.bind(ep, ec);

    if (ec.value() != 0) {
        std::cout << "error code: " << ec.value() << "-----"
                << "error message: " << ec.message() << std::endl;
        return ec.value();
    }

    return 0;
}

int ip_connect_to_end() {
    std::string remote_address = "192.168.1.124";
    unsigned short remote_port = 3333;
    try {
        asio::ip::tcp::endpoint ep(ip::make_address(remote_address), remote_port);
        asio::io_context io_context;
        asio::ip::tcp::socket sock(io_context, ep.protocol());
        sock.connect(ep);
    } catch (boost::system::system_error &e) {
        std::cout << "error code: " << e.code().value() << "-----"
                << "error message: " << e.what() << std::endl;
        return e.code().value();
    }

    return 0;
}

int dns_connect_to_end() {
    std::string remote_host = "llfc.club";
    std::string remote_port = "3333";
    asio::io_context io_context;
    ip::tcp::resolver resolver(io_context);
    try {
        auto endpoints = resolver.resolve(remote_host, remote_port);
        asio::ip::tcp::socket sock(io_context);
        asio::connect(sock, endpoints);
    } catch (boost::system::system_error &e) {
        std::cout << "error code: " << e.code().value() << "-----"
                << "error message: " << e.what() << std::endl;
        return e.code().value();
    }

    return 0;
}

int accept_connection_new() {
    const int BACKLOG_SIZE = 30;
    unsigned short port_num = 3333;
    asio::ip::tcp::endpoint ep(ip::address_v4::any(), port_num);
    asio::io_context io_context;
    try {
        asio::ip::tcp::acceptor acceptor(io_context, ep.protocol());
        acceptor.bind(ep);
        acceptor.listen(BACKLOG_SIZE);
        asio::ip::tcp::socket sock(io_context);
        acceptor.accept(sock);

    } catch (boost::system::system_error &e) {
        std::cout << "error code: " << e.code().value() << "-----"
              << "error message: " << e.what() << std::endl;
        return e.code().value();
    }

    return 0;
}
