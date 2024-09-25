# Simple recipe for MAX Box image
SUMMARY = "HEIPC Example Image"
DESCRIPTION = "An image installing multiple clients and servers to show the functionalities of the HEIPC toolchain."
LICENSE = "MIT"

inherit core-image

IMAGE_INSTALL += " htop "

IMAGE_FEATURES += " debug-tweaks ssh-server-openssh"