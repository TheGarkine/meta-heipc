cmake_minimum_required(VERSION 3.10)

project(my_grpc_project)

# Find the gRPC and Protobuf packages
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(absl REQUIRED)

include_directories(${CMAKE_BINARY_DIR})

# Define the source files
# Since the .pb.cc and .grpc.pb.cc files are generated in the Yocto recipe, we just include them directly
set(PROTO_SRCS
    ${CMAKE_BINARY_DIR}/greeter.pb.cc
    ${CMAKE_BINARY_DIR}/greeter.grpc.pb.cc
)

# Add the executable for the gRPC server
add_executable(grpc_server src/main.cpp ${PROTO_SRCS})

# Link the gRPC and Protobuf libraries
# TODO (rkrauthann) absl for some reason needs specific linking
# see: https://github.com/grpc/grpc/issues/34046
target_link_libraries(grpc_server
    gRPC::grpc++
    protobuf::libprotobuf
    absl::log_internal_check_op
    absl::log_internal_message
    absl::raw_logging_internal
    absl::spinlock_wait
)