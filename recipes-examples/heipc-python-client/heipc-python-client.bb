LICENSE="CLOSED"

RDEPENDS:${PN} = "heipc-cpp-server-lib-python python3-core"

SRC_URI ="file://main.py" 

FILES:${PN} = "${bindir}/main.py"

S = "${WORKDIR}"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/main.py ${D}${bindir}/main.py
}