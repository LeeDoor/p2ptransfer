add_executable(p2plisten
    main.cpp 
    argument_parser.cpp 
    manager.cpp
)
target_include_directories(p2plisten PUBLIC ${Boost_INCLUDE_DIRS})
