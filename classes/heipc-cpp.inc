HEIPC_CPP_BITBAKE_PACKAGE_NAME = "${PN}-lib-cpp"

PACKAGES += "${HEIPC_CPP_BITBAKE_PACKAGE_NAME}"
PROVIDES += "${HEIPC_CPP_BITBAKE_PACKAGE_NAME}"

HEIPC_CPP_PACKAGE_DIR = "${B}/heipc-cpp"
HEIPC_CPP_LIBRARY_NAME = "${PN}_grpc_client"

# Replace all "-" with "_" to conform to Python standards
python () {
    d.setVar('HEIPC_CPP_LIBRARY_NAME', d.getVar('HEIPC_CPP_LIBRARY_NAME').replace("-", "_"))
}

FILES:${HEIPC_CPP_BITBAKE_PACKAGE_NAME} = "${libdir}/lib${HEIPC_CPP_LIBRARY_NAME}.so.*"
FILES:${HEIPC_CPP_BITBAKE_PACKAGE_NAME}-dev = "${libdir}/lib${HEIPC_CPP_LIBRARY_NAME}.so ${includedir}/${HEIPC_CPP_LIBRARY_NAME}/*.h"

RDEPENDS:${HEIPC_CPP_BITBAKE_PACKAGE_NAME} += "protobuf grpc"

do_configure:append() {
    mkdir -p ${HEIPC_CPP_PACKAGE_DIR}

    # Generate CMakeLists.txt
    cat << EOF > ${HEIPC_CPP_PACKAGE_DIR}/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(${HEIPC_CPP_BITBAKE_PACKAGE_NAME})

# Library output path
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY \${CMAKE_BINARY_DIR})

# Find Protobuf and gRPC
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# Set source files
set(SOURCE_FILES
    ${HEIPC_PROTO_BASE_NAME}.pb.cc
    ${HEIPC_PROTO_BASE_NAME}.grpc.pb.cc
)

# Build shared library
add_library(${HEIPC_CPP_LIBRARY_NAME} SHARED \${SOURCE_FILES})

# Link against gRPC and Protobuf
target_link_libraries(${HEIPC_CPP_LIBRARY_NAME} protobuf::libprotobuf gRPC::grpc++)

# Set the library version
set_target_properties(${HEIPC_CPP_LIBRARY_NAME} PROPERTIES
    VERSION 1.0
    SOVERSION 1
)

# Install library
install(TARGETS ${HEIPC_CPP_LIBRARY_NAME}
        LIBRARY DESTINATION lib)

# Install headers
install(FILES ${HEIPC_PROTO_BASE_NAME}.pb.h ${HEIPC_PROTO_BASE_NAME}.grpc.pb.h
        DESTINATION include/${HEIPC_CPP_LIBRARY_NAME})
EOF
}

do_compile:append() {
    mkdir -p ${HEIPC_CPP_PACKAGE_DIR}
    
    # Generate Protobuf and gRPC sources
    protoc --proto_path=${HEIPC_PROTO_PATH} --cpp_out=${HEIPC_CPP_PACKAGE_DIR} ${HEIPC_PROTO_LOCATION}
    protoc --proto_path=${HEIPC_PROTO_PATH} --grpc_out=${HEIPC_CPP_PACKAGE_DIR} --plugin=protoc-gen-grpc=$(which grpc_cpp_plugin) ${HEIPC_PROTO_LOCATION}
    
    # Run CMake to build the shared library
    cd ${HEIPC_CPP_PACKAGE_DIR}

    # Configure cmake with proper paths for installation (no DESTDIR here)
    cmake -B ${HEIPC_CPP_PACKAGE_DIR} -S ${HEIPC_CPP_PACKAGE_DIR} \
            -DCMAKE_INSTALL_PREFIX=${prefix} \
            -DCMAKE_INSTALL_LIBDIR=${libdir} \
            -DCMAKE_INSTALL_INCLUDEDIR=${includedir} \
            -DCMAKE_INSTALL_SO_NO_EXE=0 \
            -DCMAKE_TOOLCHAIN_FILE=${WORKDIR}/toolchain.cmake \
            -DCMAKE_VERBOSE_MAKEFILE=1 \
            ${EXTRA_OECMAKE} \
            -Wno-dev

    oe_runmake -C ${HEIPC_CPP_PACKAGE_DIR}
}

do_install:append() {
    # Install the compiled library and headers using CMake install
    cd ${HEIPC_CPP_PACKAGE_DIR}
    make install DESTDIR=${D}
}
