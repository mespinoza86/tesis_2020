#
# This file is the multiplyorg recipe.
#

SUMMARY = "Simple multiplyorg application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://orgmultiply.elf \
	"

S = "${WORKDIR}"

do_install() {
	     install -d ${D}/lib/firmware
	     install -m 0755 ${S}/orgmultiply.elf ${D}/lib/firmware/orgmultiply.elf
}

FILES_${PN} = "/lib/firmware/orgmultiply.elf"
