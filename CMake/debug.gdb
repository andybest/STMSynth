target remote localhost:3333
monitor reset
monitor halt
load STMSynth.elf
disconnect
target remote localhost:3333
monitor reset
monitor halt