cmake_minimum_required(VERSION 3.10)
project(test_protobuf)
find_package(protobuf CONFIG REQUIRED)
set(SOURCE
    main.cpp
    test.pb.cc
)
add_executable(test_proto ${SOURCE})

target_include_directories(test_proto PUBLIC
    ${PROJECT_SOURCE_DIR}/
    ${PROTOBUF_INCLUDE_DIRS}
)
target_link_libraries(test_proto 
    PUBLIC 
    protobuf::libprotobuf)