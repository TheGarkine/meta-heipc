DEPENDS += "virtual/nativesdk-aarch64-pokysdk-linux-compilerlibs"

BBCLASSEXTEND = "native nativesdk"

# We do some preparation work for all variants
python () {
    filename, file_extension = os.path.splitext(d.getVar('HEIPC_PROTO_LOCATION'))
    base_name = filename.split("/")[-1]
    proto_path = "/".join(filename.split("/")[:-1])

    d.setVar('HEIPC_PYTHON_PACKAGE_NAME', d.getVar('HEIPC_PYTHON_PACKAGE_NAME').replace("-", "_"))
    d.setVar('HEIPC_PROTO_BASE_NAME', base_name)
    d.setVar('HEIPC_PROTO_PATH', proto_path)
}

require heipc-python.inc
require heipc-cpp.inc