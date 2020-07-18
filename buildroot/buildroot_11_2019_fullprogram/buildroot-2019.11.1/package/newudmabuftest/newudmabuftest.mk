NEWUDMABUFTEST_VERSION = 1.0.0
NEWUDMABUFTEST_SITE = $(TOPDIR)/package/newudmabuftest/src
NEWUDMABUFTEST_SITE_METHOD = local

#define UDMABUFTEST_BUILD_CMDS
#    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
#endef

#define UDMABUFTEST_INSTALL_TARGET_CMDS
#    $(INSTALL) -D -m 0755 $(@D)/udmabuftest $(TARGET_DIR)/usr/bin/udmabuftest
#endef

CXX=arm-linux-gnueabihf-g++
NEWUDMABUFTEST_LIB = $(TOPDIR)/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/
HOST_MPI_LIB = $(TOPDIR)/output/host/arm-buildroot-linux-uclibcgnueabihf/sysroot/usr/

TARGET_CXXFLAGS += -mcpu=cortex-a9 -lm -std=c++17 -Wall -O3 -Wpedantic -Wshadow -ftree-vectorize  -mfpu=neon -O3 -pthread -Wl,-rpath -Wl,$(HOST_MPI_LIB)/lib/ -Wl,--enable-new-dtags -L$(HOST_MPI_LIB)/lib/ -L$(HOST_MPI_LIB)/lib/ -lmpi -I$(NEWUDMABUFTEST_LIB)/include -I$(HOST_MPI_LIB)/lib/
 


define NEWUDMABUFTEST_BUILD_CMDS
    $(MAKE) $(TARGET_CONFIGURE_OPTS) CPU_TARGET=arm OS_TARGET=linux  -C $(@D)
endef
define NEWUDMABUFTEST_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/newudmabuftest $(TARGET_DIR)/usr/bin/newudmabuftest
endef

$(eval $(generic-package))
