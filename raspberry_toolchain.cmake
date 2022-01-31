# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Linux)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# where is the target environment located

SET(CMAKE_FIND_ROOT_PATH /home/martin/Work/proofs-of-concepts/hello-world-cross-compile/raspberrypi/rootfs)

#IF(EXISTS ROOTFS_PATH)
 #   SET(CMAKE_FIND_ROOT_PATH ROOTFS_PATH)
#ELSE()
 #   MESSAGE(FATAL_ERROR "Specify existing target FS with -DROOTFS_PATH=/path/to/fs")
#ENDIF()

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)