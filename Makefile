BUILD_DIR = build
CMAKE = cmake
SUDO = sudo

# .PHONY targets ensure these run even if a file with the same name exists
.PHONY: all build install clean

# Default target
all: build

build:
	$(CMAKE) -S . -B $(BUILD_DIR) -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS=ON
	$(CMAKE) --build $(BUILD_DIR)

install: build
	$(SUDO) $(CMAKE) --install $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
	sudo rm -f /usr/local/lib/libcommon-control.so /usr/local/lib/libcommon-control.so.0 /usr/local/lib/libcommon-control.so.0.1.0 
	sudo rm -rf /usr/local/include/common-control/common-control /usr/local/include/common-control/common-control.h /usr/local/include/common-control/logging.h
	# Optional: Use CMake's built-in clean if you want to keep the build folder but remove binaries
	# $(CMAKE) --build $(BUILD_DIR) --target clean
