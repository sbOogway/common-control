# Usage:
# cmake -DCMAKE_TOOLCHAIN_FILE=./user_cross_compile_setup.cmake -B build -S .
# make  -C build -j



message("$ENV{TOOLS}")
message("$ENV{STAGE}")

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# set(CMAKE_SYSROOT $ENV{STAGE})

set(CMAKE_C_COMPILER $ENV{TOOLS}gcc)
set(CMAKE_CXX_COMPILER $ENV{TOOLS}g++)

# Configure pkg-config for cross-compilation
# set(ENV{PKG_CONFIG_SYSROOT_DIR} "$ENV{STAGE}")
# set(ENV{PKG_CONFIG_PATH} "$ENV{STAGE}/usr/lib/pkgconfig:$ENV{STAGE}/usr/share/pkgconfig")

# Configure CMake find commands for cross-compilation
# set(CMAKE_FIND_ROOT_PATH "$ENV{STAGE}")
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# If necessary, set STAGING_DIR
# if not work, please try(in shell command): export STAGING_DIR=/home/ubuntu/Your_SDK/out/xxx/openwrt/staging_dir/target
# set(ENV{STAGING_DIR} "/home/oogway/amel/buildroot/output/staging")

# set(CMAKE_SYSROOT /home/oogway/amel/buildroot/output/staging)

