#pragma once
#undef emit // 🤡 syncstream::emit() conflicts with Qt
#include <bits/stdc++.h>
#include <filesystem>
#include <functional>
#include <boost/asio.hpp>
#include <boost/system.hpp>
#ifdef NDEBUG
#define BOOST_STACKTRACE_USE_BACKTRACE
#endif
#include <boost/stacktrace.hpp>
