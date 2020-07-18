SUMMARY = "Simple test application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM ="file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI = "file://SimSpec.json"
S = "${WORKDIR}"

INSANE_SKIP_${PN} = "arch"

do_install() {
	install -d ${D}/home/root
	install -m 0644 ${S}/SimSpec.json ${D}/home/root/SimSpec.json
}

FILES_${PN} = "/home/root/SimSpec.json"
