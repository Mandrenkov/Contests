.BUILD_DIR=build

.PHONY: build
build:
	cmake -B $(.BUILD_DIR) .
	cmake --build $(.BUILD_DIR) -j`nproc`

.PHONY: test
test: build
	./$(.BUILD_DIR)/runner

.PHONY: clean
clean:
	rm -rf $(.BUILD_DIR)