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
    std::string remote_address = "127.0.0.1";
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
    unsigned short port_num = 3333;
    ip::address ip_address = ip::address_v4::any();
    ip::tcp::endpoint ep(ip_address, port_num);

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
    asio::io_context io_context;
    ip::tcp::socket sock(io_context, ip::tcp::v4());
    return 0;
}

int create_acceptor_socket_old() {
    asio::io_context io_context;
    ip::tcp::acceptor acceptor(io_context);
    ip::tcp protocol = ip::tcp::v4();
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
    ip::tcp::endpoint ep(ip::address_v4::any(), port_num);

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
    std::string remote_address = "127.0.0.1";
    unsigned short remote_port = 3333;
    try {
        ip::tcp::endpoint ep(ip::make_address(remote_address), remote_port);
        asio::io_context io_context;
        ip::tcp::socket sock(io_context, ep.protocol());
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
        ip::tcp::socket sock(io_context);
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
    ip::tcp::endpoint ep(ip::address_v4::any(), port_num);
    asio::io_context io_context;
    try {
        ip::tcp::acceptor acceptor(io_context, ep.protocol());
        acceptor.bind(ep);
        acceptor.listen(BACKLOG_SIZE);
        ip::tcp::socket sock(io_context);
        acceptor.accept(sock);
    } catch (boost::system::system_error &e) {
        std::cout << "error code: " << e.code().value() << "-----"
                << "error message: " << e.what() << std::endl;
        return e.code().value();
    }

    return 0;
}

void use_const_buffer() {
    std::string buf = "Hello World!";
    asio::const_buffer buffer(buf.c_str(), buf.length());
    std::vector<asio::const_buffer> buffers;
    buffers.push_back(buffer);
}

void use_buffer_str() {
    asio::const_buffer output = asio::buffer("Hello World!");
}

void use_buffer_array() {
    const size_t BUFFER_SIZE_BYTES = 32;
    std::unique_ptr<char[]> buffer(new char[BUFFER_SIZE_BYTES]);
    auto input_buf = asio::buffer(static_cast<void *>(buffer.get()), BUFFER_SIZE_BYTES);
}

void write_to_socket(asio::ip::tcp::socket &socket) {
    std::string buf = "Hello World!";
    std::size_t total_bytes_written = 0;
    while (total_bytes_written != buf.length()) {
        total_bytes_written += socket.write_some(asio::buffer(buf.c_str() + total_bytes_written
                                                              , buf.length() - total_bytes_written));
    }
}

int send_data_by_write_some_old() {
    std::string remote_address = "127.0.0.1";
    unsigned short remote_port = 3333;

    try {
        ip::tcp::endpoint ep(ip::make_address(remote_address), remote_port);
        asio::io_context io_context;
        ip::tcp::socket sock(io_context, ep.protocol());
        sock.connect(ep);
        write_to_socket(sock);
    } catch (boost::system::system_error &e) {
        std::cout << "error code: " << e.code().value() << "-----"
                << "error message: " << e.what() << std::endl;
        return e.code().value();
    }

    return 0;
}

int send_data_by_write_some_new() {
    std::string remote_address = "127.0.0.1";
    unsigned short remote_port = 3333;

    try {
        ip::tcp::endpoint ep(ip::make_address(remote_address), remote_port);
        asio::io_context io_context;
        ip::tcp::socket sock(io_context, ep.protocol());
        sock.connect(ep);
        std::string buf = "Hello World!";
        int send_length = sock.send(asio::buffer(buf.c_str(), buf.length()));
        if (send_length < -0) {
            return 0;
        }
    } catch (boost::system::system_error &e) {
        std::cout << "error code: " << e.code().value() << "-----"
                << "error message: " << e.what() << std::endl;
        return e.code().value();
    }

    return 0;
}

int send_data_by_write_old() {
    std::string remote_address = "127.0.0.1";
    unsigned short remote_port = 3333;

    try {
        ip::tcp::endpoint ep(ip::make_address(remote_address), remote_port);
        asio::io_context io_context;
        ip::tcp::socket sock(io_context, ep.protocol());
        sock.connect(ep);
        std::string buf = "Hello World!";
        int send_length = asio::write(sock, asio::buffer(buf.c_str(), buf.length()));
        if (send_length < -0) {
            return 0;
        }
    } catch (boost::system::system_error &e) {
        std::cout << "error code: " << e.code().value() << "-----"
                << "error message: " << e.what() << std::endl;
        return e.code().value();
    }

    return 0;
}

std::string read_from_socket(ip::tcp::socket &socket) {
    const unsigned char MESSAGE_SIZE = 7;
    char buffer[MESSAGE_SIZE];
    std::size_t total_bytes_read = 0;
    while (total_bytes_read != MESSAGE_SIZE) {
        total_bytes_read += socket.read_some(asio::buffer(buffer + total_bytes_read
                                                          , MESSAGE_SIZE - total_bytes_read));
    }

    return std::string(buffer, total_bytes_read);
}

int read_data_by_read_some() {
    std::string remote_address = "127.0.0.1";
    unsigned short remote_port = 3333;
    try {
        ip::tcp::endpoint ep(ip::make_address(remote_address), remote_port);
        asio::io_context io_context;
        ip::tcp::socket sock(io_context, ep.protocol());
        sock.connect(ep);
        read_from_socket(sock);
    } catch (boost::system::system_error &e) {
        std::cout << "error code: " << e.code().value() << "-----"
                << "error message: " << e.what() << std::endl;
        return e.code().value();
    }
}

int read_data_by_receive() {
    std::string remote_address = "127.0.0.1";
    unsigned short remote_port = 3333;
    try {
        ip::tcp::endpoint ep(ip::make_address(remote_address), remote_port);
        asio::io_context io_context;
        ip::tcp::socket sock(io_context, ep.protocol());
        sock.connect(ep);
        const unsigned char MESSAGE_SIZE = 7;
        char buffer[MESSAGE_SIZE];
        int receive_length = sock.receive(asio::buffer(buffer,MESSAGE_SIZE));
        if (receive_length < 0) {
            std::cout << "error receive failed";
        }

    } catch (boost::system::system_error &e) {
        std::cout << "error code: " << e.code().value() << "-----"
                << "error message: " << e.what() << std::endl;
        return e.code().value();
    }
}

int read_data_by_read() {
    std::string remote_address = "127.0.0.1";
    unsigned short remote_port = 3333;
    try {
        ip::tcp::endpoint ep(ip::make_address(remote_address), remote_port);
        asio::io_context io_context;
        ip::tcp::socket sock(io_context, ep.protocol());
        sock.connect(ep);
        const unsigned char MESSAGE_SIZE = 7;
        char buffer[MESSAGE_SIZE];
        int receive_length = asio::read(sock,asio::buffer(buffer,MESSAGE_SIZE));
        if (receive_length < 0) {
            std::cout << "error receive failed";
        }

    } catch (boost::system::system_error &e) {
        std::cout << "error code: " << e.code().value() << "-----"
                << "error message: " << e.what() << std::endl;
        return e.code().value();
    }
}
