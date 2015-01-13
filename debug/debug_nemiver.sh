#!/bin/bash

# start xterm with openocd in the background
xterm -e openocd -f ./debug/debug.cfg &

# save the PID of the background process
XTERM_PID=$!

# wait a bit to be sure the hardware is ready
sleep 2

# execute some initialisation commands via gdb
arm-none-eabi-gdb --batch --command=debug/run.gdb build/main

# start the gdb gui
nemiver --remote=localhost:3333 --gdb-binary="$(which arm-none-eabi-gdb)" build/main

# close xterm when the user has exited nemiver
kill $XTERM_PID