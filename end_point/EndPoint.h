//
// Created by 33722 on 2026/5/11.
//

#ifndef BOOST_ENDPOINT_H
#define BOOST_ENDPOINT_H

extern int client_end_point();
extern int server_end_point();
extern int create_tcp_socket_old();
extern int create_tcp_socket_new();
extern int create_acceptor_socket_old();
extern int create_acceptor_socket_new();
extern int bind_acceptor_socket_old();
extern int ip_connect_to_end();
extern int dns_connect_to_end();
extern int accept_connection_new();
extern void use_const_buffer();
extern void use_buffer_str();
extern void use_buffer_array();
// extern void write_to_socket(asio::ip::tcp::socket& socket);



#endif //BOOST_ENDPOINT_H
