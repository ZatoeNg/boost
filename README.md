### ==============================================
### C++ 网络编程 之 Boost库
### 项目说明文件
### 源作者:Bilibli:恋恋风辰zack
### 源作者仓库:https://gitee.com/secondtonone1/boostasio-learn
### ==============================================

# 注意事项
```CMakeLists
    # Linux 需要删除,WINDOWS 保留
    link_libraries(ws2_32)
```

# 记录
## 服务端：socket--->bind--->listen---->accept--->read/write;
* socket为监听socket,不是通道socket
```c++
    socket--bind--listen
    #可以分开写:
    ip::tcp::acceptor acceptor(io_context);
    ip::tcp protocol = ip::tcp::v4();
    boost::system::error_code ec;
    acceptor.open(protocol, ec);
    acceptor.bind(ep, ec);
    #也可以一步执行:tcp::acceptor acceptor
```

## 客户端：socket--->connect--->read/write;
* socket为通道socket

## async_write_some --- async_send
* async_write_some:不保证每次发送完,需要在回调进行判断
* async_send:一次性就发送完了,底层帮你做完了多次async_write_some
* 需要信息队列，否则,数据乱序、重叠、内存崩溃

## async_read_some --- async_receive
* async_read_some:不保证每次读完,需要在回调进行判断
* async_receive:一次性就读完了,底层帮你做完了多次async_read_some

