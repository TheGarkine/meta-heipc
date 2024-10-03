# Hyper Easy Inter-Process Communication

`meta-heipc` (pronounced "hype C") is a personal passion project born of the idea to make usage of grpc in your yocto distro super easy.

## Usage

The setup is very easy, you just need to inherit the `heipc` class and set `HEIPC_PROTO_LOCATION`.

```bitbake
inherit heipc
HEIPC_PROTO_LOCATION = "${S}/src/greeter.proto"
```

There is also an example heipc server implementation `/recipes-examples/heipc-cpp-server/`

### Packages

With heipc, your recipe will build a ton of extra packages you can use:

- `${PN}-lib-python`: A python client library installed into your site-packages. The resulting package name for python imports is `<PN>_grpc_client`, where all `-` are replaced with `_`.
- `nativesdk-${PN}-lib-python`: Generates the nativesdk used for `TOOLCHAIN_HOST_TASKS` to add the python library to the sdk. Keep in mind that this will also add the prefix `nativesdk_` to the python library, so it could mean that this needs handling if trying to develop clients locally.
- `${PN}-lib-cpp`: Currently only C++17 and higher is supported.


## Variables

### HEIPC_PROTO_LOCATION

The location where after the fetch the proto file will be stored at.
> Note that `heipc` will use its own directories, therefore this should be an absolute path (e.g. `${S}/src/service.proto`).

### TODOs (move me to issues)

[] move server example code generation to cmake
[] cpp client
[] make specific clients optional
[] sdk setup
[x] nativesdk setup
[] error if variable is not set
[] clear up behavior due to multiple imports of the same base class
[] export heipc cpp lib as package for easier cmake integration
[] missing sdk libraries for grpc cpp  