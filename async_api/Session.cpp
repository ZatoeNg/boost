//
// Created by 33722 on 2026/5/13.
//

#include "Session.h"

#include <iostream>


Session::Session(std::shared_ptr<asio::ip::tcp::socket> socket)
    : _socket(socket), _send_pending(false), _recv_pending(false) {
}

void Session::connect(const asio::ip::tcp::endpoint &ep) {
    _socket->connect(ep);
}

void Session::write_to_socket(const std::string &bufs) {
    _send_queue.emplace(new MsgNode(bufs.c_str(), bufs.length()));
    if (_send_pending) return;

    this->_socket->async_write_some(asio::buffer(bufs)
                                    , std::bind(&Session::write_callback, this
                                                , std::placeholders::_1, std::placeholders::_2));
    _send_pending = true;
}

void Session::write_callback(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    if (ec.value() != 0) {
        std::cout << "Error code : " << ec.value() << std::endl;
        std::cout << "Error message : " << ec.message() << std::endl;
        return;
    }

    auto &send_data = _send_queue.front();
    // 累加本次发送的字节数（更新已发送长度）
    send_data->cur_length += bytes_transferred;

    /*如果没发完，继续发送剩余部分（递归回调）
     * 使用aync_write_some,需要在回调函数判断是否发完，没发完继续发
     */
    if (send_data->cur_length < send_data->total_length) {
        this->_socket->async_write_some(asio::buffer(send_data->msg + send_data->cur_length
                                                     , send_data->total_length - send_data->cur_length),
                                        std::bind(&Session::write_callback, this
                                                  , std::placeholders::_1, std::placeholders::_2));
        return;
    }

    _send_queue.pop();

    if (_send_queue.empty()) {
        _send_pending = false;
    }

    // 队列还有消息：重置发送标记，发起下一条消息的发送
    if (!_send_queue.empty()) {
        auto &send_data = _send_queue.front();
        this->_socket->async_write_some(asio::buffer(send_data->msg + _send_node->cur_length
                                                     , _send_node->total_length - _send_node->cur_length)
                                        , std::bind(&Session::write_callback, this
                                                    , std::placeholders::_1, std::placeholders::_2));
    }
}

void Session::write_all_to_socket(const std::string &bufs) {
    _send_queue.emplace(new MsgNode(bufs.c_str(), bufs.length()));

    if (_send_pending) return;

    this->_socket->async_send(asio::buffer(bufs),
                              std::bind(&Session::write_callback, this
                                        , std::placeholders::_1, std::placeholders::_2));
    _send_pending = true;
}

void Session::write_all_callback(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    if (ec.value() != 0) {
        std::cout << "Error code : " << ec.value() << std::endl;
        std::cout << "Error message : " << ec.message() << std::endl;
        return;
    }

    _send_queue.pop();
    if (_send_queue.empty()) {
        _send_pending = false;
    }

    if (!_send_queue.empty()) {
        auto &send_data = _send_queue.front();
        this->_socket->async_write_some(asio::buffer(send_data->msg + _send_node->cur_length
                                                     , _send_node->total_length - _send_node->cur_length)
                                        , std::bind(&Session::write_callback, this
                                                    , std::placeholders::_1, std::placeholders::_2));
    }
}

void Session::read_from_socket() {
    if (_recv_pending) return;

    _recv_node = std::make_shared<MsgNode>(RECVSIZE);
    _socket->async_read_some(asio::buffer(_recv_node->msg, _recv_node->total_length)
                             , std::bind(&Session::read_callback, this
                                         , std::placeholders::_1, std::placeholders::_2));

    _recv_pending = true;
}

void Session::read_callback(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    if (ec.value() != 0) {
        std::cout << "Error code : " << ec.value() << std::endl;
        std::cout << "Error message : " << ec.message() << std::endl;
        return;
    }

    _recv_node->cur_length += bytes_transferred;
    if (_recv_node->cur_length < _recv_node->total_length) {
        this->_socket->async_read_some(asio::buffer(_recv_node->msg + _recv_node->cur_length
                                                    , _recv_node->total_length - _recv_node->cur_length)
                                       , std::bind(&Session::read_callback, this
                                                   , std::placeholders::_1, std::placeholders::_2));

        return;
    }

    _recv_pending = false;
    _send_node = nullptr;
}

void Session::read_all_from_socket() {
    if (_recv_pending) return;
    _recv_node = std::make_shared<MsgNode>(RECVSIZE);
    _socket->async_receive(asio::buffer(_recv_node->msg, _recv_node->total_length)
                           , std::bind(&Session::read_all_callback, this
                                       , std::placeholders::_1, std::placeholders::_2));

    _recv_pending = true;
}

void Session::read_all_callback(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    if (ec.value() != 0) {
        std::cout << "Error code : " << ec.value() << std::endl;
        std::cout << "Error message : " << ec.message() << std::endl;
        return;
    }

    _recv_node->cur_length += bytes_transferred;
    _recv_node = nullptr;
    _recv_pending = false;
}
