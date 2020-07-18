#
# This file is the newudmabuftest recipe.
#

SUMMARY = "Simple newudmabuftest application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://newudmabuftest.cpp \
           file://dma_uio.h \
           file://GapJunctionDriver.hpp \
           file://ip-test.h \
           file://uio.hpp \
           file://statistics.hpp \
           file://Makefile \
		  "

S = "${WORKDIR}"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}${bindir}
	     install -m 0755 newudmabuftest ${D}${bindir}
}
