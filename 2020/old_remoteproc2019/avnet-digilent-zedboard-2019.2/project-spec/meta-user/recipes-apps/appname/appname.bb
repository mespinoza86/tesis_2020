SUMMARY = "Simple test application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM ="file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI = "file://remoteproc2019.elf"
S = "${WORKDIR}"

INSANE_SKIP_${PN} = "arch"

do_install() {
	install -d ${D}/lib/firmware
	install -m 0644 ${S}/remoteproc2019.elf ${D}/lib/firmware/remoteproc2019.elf
}

FILES_${PN} = "/lib/firmware/remoteproc2019.elf"
