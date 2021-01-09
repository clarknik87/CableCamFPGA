set working_dir [pwd]
set curr_folder [file tail [pwd] ]
set design_name $curr_folder
set project_name cablecamFPGA
set design_name_top ${design_name}_top
cd  $working_dir

#create a new project, set part and board (if necessary)
create_project -force $project_name _project
set_property board_part digilentinc.com:basys3:part0:1.1 [current_project]
set_property target_language VHDL [current_project]

#add local src files
add_files -fileset sources_1 ./$project_name.srcs/sources_1/

#add constraint files
add_files -fileset constrs_1 ./$project_name.srcs/constrs_1/Basys3_Master.xdc

start_gui

#import the microblaze subsystem block design
source mb_subsystem.tcl
make_wrapper -files [get_files _project/$project_name.srcs/sources_1/bd/mb_subsystem/mb_subsystem.bd] -top
add_files -norecurse _project/$project_name.srcs/sources_1/bd/mb_subsystem/hdl/mb_subsystem_wrapper.vhd
update_compile_order -fileset sources_1
regenerate_bd_layout
save_bd_design