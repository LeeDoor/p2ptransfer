#pragma once
#undef emit // 🤡 syncstream::emit() conflicts with Qt
#include <bits/stdc++.h>
#include <filesystem>
#include <functional>
#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/system.hpp>
#include <boost/program_options.hpp>
using namespace boost::asio::experimental::awaitable_operators;

