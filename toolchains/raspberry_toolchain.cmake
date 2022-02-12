# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)

# compilers for raspberry:
SET(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

SET(CMAKE_FIND_ROOT_PATH /home/martin/Work/proofs-of-concepts/hello-world-cross-compile/raspberry-fs)
SET(CMAKE_SYSROOT /home/martin/Work/proofs-of-concepts/hello-world-cross-compile/raspberry-fs)

# if never -> searching programs just in host
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)