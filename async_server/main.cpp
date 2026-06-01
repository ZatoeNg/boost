//
// Created by this on 2026/6/1.
//


#include <boost/asio/io_context.hpp>

#include "session.h"

int main()
{
    try
    {
        boost::asio::io_context io_context;
        server s(io_context,10086);
        io_context.run();
    }catch (std::exception& e)
    {
        std::cerr<<"exception:" << e.what() << '\n';
    }

    return 0;
}
