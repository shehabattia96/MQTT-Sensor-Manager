# mqtt_sensor_manager - Listens to sensors on an mqtt server

## About

As an application developer, there is a need for a central processing application that sends and receives data from peripheral units.
Peripheral units can be hardware units such as sensors, actuators, emitters, controllers (e.g. cameras, microphones, encoders, switches, motor controllers, buzzers, lcd screens), or software programs (e.g. transport (mqtt, rest, uart), photo viewer, video player, file writer).
The content of the data can be events, states, configurations, images, audio or any information related to the peripheral units.
The central processing application initializes the units it will use, implements the dispatch handlers the units will call, sets the dispatch handlers to their repective units, then let the units manage communicating with each other using events and dispatch handlers.

## Getting Started

Run `install.sh -o windows|linux|mac`. To build run `install.sh -b -o windows|linux|mac`.

### VSCode support for toolchain

You'll need the `cmaketools` extension. Scan for kits then search the Pallete for "CMake: Edit Local-User CMake Kits and add `"toolchainFile": "${workspaceFolder}/externals/vcpkg/scripts/buildsystems/vcpkg.cmake"` to your kit. To build the toolchain, you must run `install.sh` at least once, you do not need to run it before each build.

## Dependencies

- [cmake](https://cmake.org/)
    - On Mac, you should run `sudo "/Applications/CMake.app/Contents/bin/cmake-gui" --install` to install cmake to path after downloading cmake
- The install script automatically grabs these:
    - vcpkg
    - paho-mqtt
        - openssl is a dependency
    - opencv