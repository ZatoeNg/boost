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



#endif //BOOST_ENDPOINT_H
