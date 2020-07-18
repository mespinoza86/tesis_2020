#
# This file is the elfrtosaxon recipe.
#

SUMMARY = "Simple elfrtosaxon application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI = "file://elfrtosaxon.elf"
S = "${WORKDIR}"

INSANE_SKIP_${PN} = "arch"

do_install() {
	install -d ${D}/lib/firmware
	install -m 0644 ${S}/elfrtosaxon.elf ${D}/lib/firmware/elfrtosaxon.elf
}

FILES_${PN} = "/lib/firmware/elfrtosaxon.elf"

