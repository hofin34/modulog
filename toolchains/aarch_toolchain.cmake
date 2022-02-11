SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR aarch64)

SET(CMAKE_C_COMPILER   aarch64-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

SET(CMAKE_FIND_ROOT_PATH /home/martin/Work/proofs-of-concepts/hello-world-cross-compile/train-sensor)
SET(CMAKE_SYSROOT /home/martin/Work/proofs-of-concepts/hello-world-cross-compile/train-sensor)

# if never -> searching programs just in host
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)