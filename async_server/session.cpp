//
// Created by this on 2026/6/1.
//

#include "session.h"
#include <iostream>

void session::start()
{
    memset(_data, 0, max_length);
    _socket.async_read_some(boost::asio::buffer(_data, max_length)
                            , std::bind(&session::handle_read, this, std::placeholders::_1, std::placeholders::_2));
}

void session::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
    if (!error)
    {
        std::cout << "server receive data is:" << _data << std::endl;
        boost::asio::async_write(_socket, boost::asio::buffer(_data, bytes_transferred)
                                 , std::bind(&session::handle_write, this, std::placeholders::_1));
    }
    else
    {
        std::cout << "handle_read error" << std::endl;
        // 异步读取时，会出现session对象被销毁，导致异常
        delete this;
    }
}

void session::handle_write(const boost::system::error_code& error)
{
    if (!error)
    {
        memset(_data, 0, max_length);
        _socket.async_read_some(boost::asio::buffer(_data, max_length)
                                , std::bind(&session::handle_read, this, std::placeholders::_1, std::placeholders::_2));
    }
    else
    {
        std::cout << "handle_read error" << std::endl;
        // 异步读取时，会出现session对象被销毁，导致异常
        delete this;
    }
}

server::server(boost::asio::io_context& io_context, short port) : io_context(io_context)
                                                                  , _acceptor(io_context, tcp::endpoint(tcp::v4(), port))
{
    std::cout << "Server started success,on port:" << port << std::endl;
    start_accept();
}

void server::start_accept()
{
    session* new_session = new session(io_context);
    _acceptor.async_accept(new_session->get_socket()
                           , std::bind(&server::handle_accept, this, new_session, std::placeholders::_1));
}

void server::handle_accept(session* new_session, const boost::system::error_code& error)
{
    if (!error)
    {
        new_session->start();
    }
    else
    {
        delete new_session;
    }

    start_accept();
}
