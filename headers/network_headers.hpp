#pragma once
#define BOOST_ASIO_NO_DEPRECATED
namespace boost { namespace asio { namespace ip { class tcp; } } };
namespace net = boost::asio;
using tcpip = net::ip::tcp;
