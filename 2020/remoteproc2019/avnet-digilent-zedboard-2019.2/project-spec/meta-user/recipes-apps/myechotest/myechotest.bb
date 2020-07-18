
SUMMARY = "RPMsg examples: echo test demo"

LICENSE = "BSD"
LIC_FILES_CHKSUM = "file://LICENSE;md5=88c79b255c8e69a374365a4d9af40779"

SRC_URI = "\
	file://LICENSE \
	file://Makefile \
	file://myechotest.c \
	"

S = "${WORKDIR}"

RRECOMMENDS_${PN} = "kernel-module-rpmsg-char"

FILES_${PN} = "\
	/usr/bin/myechotest\
"

do_install () {
	install -d ${D}/usr/bin
	install -m 0755 myechotest ${D}/usr/bin/myechotest
}



