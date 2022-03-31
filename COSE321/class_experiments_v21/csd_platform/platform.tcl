# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct C:\Users\hanchaa\Desktop\Korea_Univ_2022_1\COSE321\class_experiments_v21\csd_platform\platform.tcl
# 
# OR launch xsct and run below command.
# source C:\Users\hanchaa\Desktop\Korea_Univ_2022_1\COSE321\class_experiments_v21\csd_platform\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {csd_platform}\
-hw {C:\Users\hanchaa\Desktop\Korea_Univ_2022_1\COSE321\class_experiments_v21\SoC_wrapper.xsa}\
-proc {ps7_cortexa9_0} -os {standalone} -out {C:/Users/hanchaa/Desktop/Korea_Univ_2022_1/COSE321/class_experiments_v21}

platform write
platform generate -domains 
platform active {csd_platform}
platform generate
platform clean
platform generate
platform active {csd_platform}
platform generate -domains 
platform generate
