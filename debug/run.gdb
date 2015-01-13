target remote localhost:3333
#target remote | openocd -f debug/debug.cfg -c "gdb_port pipe; log_output openocd.log"
monitor reset halt
thbreak main
monitor reset init
#continue