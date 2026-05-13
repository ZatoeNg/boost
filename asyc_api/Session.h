//
// Created by 33722 on 2026/5/13.
//

#ifndef BOOST_SESSION_H
#define BOOST_SESSION_H
#include <memory>
#include <queue>
#include <boost/asio.hpp>

namespace asio = boost::asio;

const int RECVSIZE = 1024;

/*  消息节点
 */
class MsgNode {
public:
    MsgNode(const char *data, int len)
        : total_length(len), cur_length(0) {
        msg = new char[len];
        memcpy(msg, data, len);
    };

    MsgNode(int len):total_length(len) ,cur_length(0){
        msg = new char[len];
    }

    ~MsgNode() {
        delete[] msg;
    }
public:
    char* msg = nullptr;
    int total_length;
    int cur_length;

};

/*会话管理
 */
class Session {
public:
    Session(std::shared_ptr<asio::ip::tcp::socket> socket);
    ~Session();

public:
    void connect(const asio::ip::tcp::endpoint& ep);
    void write_to_socket(const std::string& bufs);
    void write_callback(const boost::system::error_code& ec,std::size_t bytes_transferred);

    void write_all_to_socket(const std::string& bufs);
    void write_all_callback(const boost::system::error_code& ec,std::size_t bytes_transferred);

    void read_from_socket();
    void read_callback(const boost::system::error_code& ec,std::size_t bytes_transferred);

    void read_all_from_socket();
    void read_all_callback(const boost::system::error_code& ec,std::size_t bytes_transferred);

private:
    std::shared_ptr<asio::ip::tcp::socket> _socket;
    std::shared_ptr<MsgNode> _send_node;
    std::shared_ptr<MsgNode> _recv_node;
    std::queue<std::shared_ptr<MsgNode>> _send_queue;
    bool _send_pending; //发送标识
    bool _recv_pending; //接收标识
};


#endif //BOOST_SESSION_H
