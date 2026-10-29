cmake_minimum_required(VERSION 3.14)
project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(app
    main.cpp
    cli_user.cpp
    rule_store.cpp
    RuleMatcher.cpp
    RuleEngine.cpp
)
