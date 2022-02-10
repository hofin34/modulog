# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR aarch64)

# compilers for raspberry:
#SET(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
#SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# compilers for train sensor:


SET(CMAKE_C_COMPILER   aarch64-linux-gnu-gcc-8)
SET(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++-8)

SET(CMAKE_FIND_ROOT_PATH /home/martin/Work/proofs-of-concepts/hello-world-cross-compile/train-sensor)
SET(CMAKE_SYSROOT /home/martin/Work/proofs-of-concepts/hello-world-cross-compile/train-sensor)

# if never -> searching programs just in host
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)