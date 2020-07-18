connect -url tcp:127.0.0.1:3121
source /home/marco/Documents/tesis/2020_www34_neuronal_new_IP/2020_newIP_project1/project_2/project_2.sdk/design_1_wrapper_hw_platform_0/ps7_init.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zed 210248764353"} -index 0
loadhw -hw /home/marco/Documents/tesis/2020_www34_neuronal_new_IP/2020_newIP_project1/project_2/project_2.sdk/design_1_wrapper_hw_platform_0/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zed 210248764353"} -index 0
stop
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zed 210248764353"} -index 0
rst -processor
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zed 210248764353"} -index 0
dow /home/marco/Documents/tesis/2020_www34_neuronal_new_IP/2020_newIP_project1/project_2/project_2.sdk/NEURONAL_LINUX_SDK/Debug/NEURONAL_LINUX_SDK.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zed 210248764353"} -index 0
con
