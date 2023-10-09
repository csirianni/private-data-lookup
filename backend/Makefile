BUILD_DIR = build

all: build

# the -p flag in mkdir ensures nested directories can be created
build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..
	cd $(BUILD_DIR) && cmake --build .

clean:
	rm -rf $(BUILD_DIR)