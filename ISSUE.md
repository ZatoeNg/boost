```C++
/* 有BUG的例子
 * 无法保证完整传输
 * 例如:第一次发送了一个“Hello world”,TCP一次只能传送Hello，wrold未传输
 * 准备传输world的时候，又发送了一个“Hello world”,就有可能导致传输成了“Hello” “Hello world" "world"
 */
void Session::write_callback_err(const boost::system::error_code &ec, std::size_t bytes_transferred,
    std::shared_ptr<MsgNode> msgs_node) {

    if (ec.value() != 0) {
        std::cout << "Error code : " << ec.value() << std::endl;
        std::cout << "Error message : " << ec.message() << std::endl;
        return;
    }

    if (bytes_transferred + msgs_node->cur_length < msgs_node->total_length) {
        _send_node->cur_length += bytes_transferred;
        this->_socket->async_write_some(asio::buffer(_send_node->msg+_send_node->cur_length
            ,_send_node->total_length - _send_node->cur_length),
            std::bind(&Session::write_callback_err, this, std::placeholders::_1, std::placeholders::_2
                ,_send_node));
    }
}

void Session::write_to_socket_err(const std::string buf) {
    _send_node = std::make_shared<MsgNode>(buf.c_str(), buf.length());
    this->_socket->async_write_some(asio::buffer(_send_node->msg,_send_node->total_length)
        ,std::bind(&Session::write_callback_err, this
            , std::placeholders::_1,std::placeholders::_2
            ,_send_node));
}
```