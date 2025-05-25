BUILD_DIR ?= build
CMAKE ?= cmake

all:
	$(CMAKE) -S . -B $(BUILD_DIR)
	$(CMAKE) --build $(BUILD_DIR)

clean:
	$(CMAKE) --build $(BUILD_DIR) --target clean || true
	rm -rf $(BUILD_DIR)
