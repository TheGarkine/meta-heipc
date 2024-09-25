inherit python3native setuptools3

HEIPC_PYTHON_BITBAKE_PACKAGE_NAME = "${PN}-lib-python"
HEIPC_PYTHON_PACKAGE_NAME = "${PN}_grpc_client"

python () {
    d.setVar('HEIPC_PYTHON_PACKAGE_NAME', d.getVar('HEIPC_PYTHON_PACKAGE_NAME').replace("-", "_"))
}

PACKAGES += "${HEIPC_PYTHON_BITBAKE_PACKAGE_NAME}"

HEIPC_PYTHON_PACKAGE_DIR = "${B}/heipc-python"
HEIPC_PACKAGE_SUBDIR = "${HEIPC_PYTHON_PACKAGE_DIR}/${HEIPC_PYTHON_PACKAGE_NAME}"

# Override the default Python package location
SETUPTOOLS_SETUP_PATH = "${HEIPC_PYTHON_PACKAGE_DIR}"

#Package the Python client library
FILES:${HEIPC_PYTHON_BITBAKE_PACKAGE_NAME} = "${PYTHON_SITEPACKAGES_DIR}/${HEIPC_PYTHON_PACKAGE_NAME}*"
DEPENDS += " python3-native python3-grpcio-tools-native python3-protobuf-native"
RDEPENDS:${HEIPC_PYTHON_BITBAKE_PACKAGE_NAME} += "python3 python3-grpcio python3-protobuf"

# Dynamically generate setup.py during the configuration step
do_configure:append() {
    mkdir -p ${HEIPC_PACKAGE_SUBDIR}

    touch ${HEIPC_PACKAGE_SUBDIR}/__init__.py
    cat << EOF > ${HEIPC_PYTHON_PACKAGE_DIR}/${HEIPC_PYTHON_PACKAGE_NAME}/__init__.py
import os
import glob
import importlib
import sys

# Get the current directory
current_dir = os.path.dirname(__file__)
sys.path.insert(0, current_dir)

#Dynamically import all Python files in the directory
for module_file in glob.glob(os.path.join(current_dir, "*.py")):
    module_name, ext = os.path.splitext(os.path.basename(module_file))
    if module_name != "__init__" and ext == ".py":  # Skip __init__.py itself
        # Import the module
        importlib.import_module(f".{module_name}", package=__name__)
EOF

    cat << EOF > ${HEIPC_PYTHON_PACKAGE_DIR}/setup.py
from setuptools import setup, find_packages

setup(
    name='${PN}-grpc-client',
    version='${PV}',
    packages=['${HEIPC_PYTHON_PACKAGE_NAME}'],
    install_requires=[
        'grpcio',
        'grpcio-tools',
        'protobuf'
    ],
    description='A dynamically generated gRPC client package for ${PN}',
)
EOF
}

do_compile:append() {
    ${STAGING_BINDIR_NATIVE}/python3-native/python3 -m grpc_tools.protoc --proto_path=${S}/src \
        --python_out=${HEIPC_PACKAGE_SUBDIR} --grpc_python_out=${HEIPC_PACKAGE_SUBDIR} ${S}/src/greeter.proto
}

# Installation steps for the C++ server and Python package
do_install:append() {
    # Install the Python package using dynamically generated setup.py
    cd ${HEIPC_PYTHON_PACKAGE_DIR}
    ${STAGING_BINDIR_NATIVE}/python3-native/python3 ${HEIPC_PYTHON_PACKAGE_DIR}/setup.py install --root=${D} --prefix=${prefix}
}