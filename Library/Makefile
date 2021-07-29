.BUILD_DIR=build

.PHONY: test
test:
	cmake -B $(.BUILD_DIR) .
	cmake --build $(.BUILD_DIR) -j`nproc`
	./$(.BUILD_DIR)/runner

.PHONY: clean
clean:
	rm -rf $(.BUILD_DIR)