connect -url tcp:127.0.0.1:3121
source /home/marco/Documents/2020/RTOS_SDK_2018/RTOS_vivado_2018/ps7_init.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zed 210248764353"} -index 0
loadhw -hw /home/marco/Documents/2020/RTOS_SDK_2018/RTOS_vivado_2018/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zed 210248764353"} -index 0
stop
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zed 210248764353"} -index 0
rst -processor
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zed 210248764353"} -index 0
dow /home/marco/Documents/2020/RTOS_SDK_2018/Neuronal_NORTOS/Debug/Neuronal_NORTOS.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zed 210248764353"} -index 0
con
