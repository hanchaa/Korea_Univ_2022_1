# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: C:\Users\hanchaa\Desktop\Korea_Univ_2022_1\COSE321\class_experiments_v21\csd_lab_system\_ide\scripts\systemdebugger_csd_lab_system_standalone.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source C:\Users\hanchaa\Desktop\Korea_Univ_2022_1\COSE321\class_experiments_v21\csd_lab_system\_ide\scripts\systemdebugger_csd_lab_system_standalone.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent Zed 210248764082" && level==0 && jtag_device_ctx=="jsn-Zed-210248764082-23727093-0"}
fpga -file C:/Users/hanchaa/Desktop/Korea_Univ_2022_1/COSE321/class_experiments_v21/csd_lab/_ide/bitstream/SoC_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw C:/Users/hanchaa/Desktop/Korea_Univ_2022_1/COSE321/class_experiments_v21/csd_platform/export/csd_platform/hw/SoC_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source C:/Users/hanchaa/Desktop/Korea_Univ_2022_1/COSE321/class_experiments_v21/csd_lab/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow C:/Users/hanchaa/Desktop/Korea_Univ_2022_1/COSE321/class_experiments_v21/csd_lab/Debug/csd_lab.elf
configparams force-mem-access 0
bpadd -addr &main
