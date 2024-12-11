verilator --cc bitt.v dpi_monitor.v --exe main1.cpp bitty_dpi.cpp BittyEmulator.cpp BittyInstructionGenerator.cpp

make -C obj_dir -f Vbitt.mk

./obj_dir/Vbitt
