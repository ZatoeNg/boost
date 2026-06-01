//
// Created by this on 2026/6/1.
//

#ifndef BOOST_SESSION_H
#define BOOST_SESSION_H
#include <iostream>
#include <random>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;


class session
{
public:
    session(boost::asio::io_context& io_context):_socket(io_context)
    {

    }
    tcp::socket& get_socket()
    {
        return _socket;
    }

    void start();

private:
    //错误码，实际发送的长度
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

    //async_send,可以确保一次接收完，所以只需要一个参数
    void handle_write(const boost::system::error_code& error);

private:
    tcp::socket _socket;
    enum{max_length = 1024};
    char _data[max_length];
};


class server
{
public:
    server(boost::asio::io_context& io_context,short port);

private:
    //监听连接
    void start_accept();

    //当有连接，则触发非回调
    void handle_accept(session* new_session,const boost::system::error_code& error);

    boost::asio::io_context& io_context;

    tcp::acceptor _acceptor;
};

#endif //BOOST_SESSION_H
