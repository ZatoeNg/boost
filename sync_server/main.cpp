//
// Created by 33722 on 2026/5/12.
//
#include <iostream>
#include <boost/asio.hpp>
#include <set>
#include <memory>
using tcp = boost::asio::ip::tcp;
constexpr int MAX_LENGTH = 1024;
typedef std::shared_ptr<tcp::socket> socket_ptr;
std::set<std::shared_ptr<std::thread>> thread_set;

//服务函数
void session(socket_ptr socket) {
    try {
        while (true) {
            char data[MAX_LENGTH];
            memset(data, '\0', MAX_LENGTH);
            boost::system::error_code ec;
            /*size_t length = boost::asio::read(*socket
                ,boost::asio::buffer(data,MAX_LENGTH),ec);*/
            size_t length = socket->read_some(boost::asio::buffer(data,MAX_LENGTH)
                , ec);
            if (ec == boost::asio::error::eof) {
                std::cerr << ec.message() << "\n";
                break;
            }else if (ec) {
                throw boost::system::system_error(ec);
            }

            std::cout <<"receive from" << socket->remote_endpoint().address().to_string()
            <<std::endl;

            std::cout << "receive message is:"<< data<<std::endl;
            //数据回传
            boost::asio::write(*socket, boost::asio::buffer(data), ec);
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

/*监听函数
 * acceptor
 * bind
 * listen
 * read/write
 */
void server(boost::asio::io_context &io_context,unsigned short port) {
    //创建监听socket
    tcp::acceptor acceptor(io_context,tcp::endpoint(tcp::v4(), port));
    while (true) {
        /*创建新通信socket
         * 将新socket绑定到请求端
         * 开启新线程存放socket，并调用session函数
         * 加入线程集合
         */
        socket_ptr socket(new tcp::socket(io_context));
        acceptor.accept(*socket);
        auto t = std::make_shared<std::thread>(&session, socket);
        thread_set.insert(t);
    }

}

int main() {
    try {
        boost::asio::io_context io_context;
        server(io_context,10086);
        for (auto &t : thread_set) {
            t->join();
        }

    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;

    }

    return 0;
}