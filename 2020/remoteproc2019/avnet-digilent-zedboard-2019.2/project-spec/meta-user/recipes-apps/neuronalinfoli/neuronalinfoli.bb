#
# This file is the neuronalinfoli recipe.
#

SUMMARY = "Simple neuronalinfoli application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://neuronalinfoli.cpp \
           file://argParse.cpp \
           file://CLI11.hpp \
           file://dma_uio.h \
           file://GapJunctionDriver.hpp \
           file://getSimSpec.cpp \
           file://infoli.cpp \
           file://infoli.h \ 
           file://ip-test.h \
           file://neuronalinfoli.cpp \
           file://newudmabuftest.cpp \
           file://rapidjson \
           file://socketClient.cpp \ 
           file://statistics.hpp \
           file://stepSimulate.hpp \
           file://stimQueue \
           file://uio.hpp \
           file://Makefile \
		  "

S = "${WORKDIR}"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}${bindir}
	     install -m 0755 neuronalinfoli ${D}${bindir}
}
