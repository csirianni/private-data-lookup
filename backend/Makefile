BUILD_DIR = build

all: build

build:
	cd $(BUILD_DIR) && cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	cd $(BUILD_DIR) && cmake --build .

run: build
	cd ${BUILD_DIR} && ./src/server

check: build
	cd $(BUILD_DIR) && ./test/pdl_test	

.PHONY: all build run check