SUMMARY = "Minimal gRPC C++ project"
LICENSE = "CLOSED"

SRC_URI = "file://CMakeLists.txt \
           file://src/greeter.proto \
           file://src/main.cpp"

DEPENDS = "grpc protobuf abseil-cpp protobuf-native grpc-native"

inherit heipc

inherit cmake

S = "${WORKDIR}"

# NOTE: You can also do this directly in CMake, which may create a better development cycle!
do_configure:prepend() {
    # Generate Protobuf source files
    ${STAGING_BINDIR_NATIVE}/protoc --proto_path=${S}/src \
        --cpp_out=${B} ${S}/src/greeter.proto
    
    # Generate gRPC source files
    ${STAGING_BINDIR_NATIVE}/protoc --proto_path=${S}/src \
        --grpc_out=${B} --plugin=protoc-gen-grpc=${STAGING_BINDIR_NATIVE}/grpc_cpp_plugin ${S}/src/greeter.proto
}

# Specify where to install the server binary
do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${B}/grpc_server ${D}${bindir}/grpc_server
}

FILES:${PN} = "${bindir}/grpc_server"
