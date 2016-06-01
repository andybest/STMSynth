# STMSynth
A MIDI synthesizer for the STM32F4 Discovery board

## Build instructions

```
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../CMake/arm-none-eabi-gcc.cmake ..
make
```
