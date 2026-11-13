.PHONY: build build-debug build-win build-win-debug clean

# 1. Native Linux Release
build:
	cmake -B build-linux-release -G Ninja -DCMAKE_BUILD_TYPE=Release
	cmake --build build-linux-release
	ln -sf build-linux-release/compile_commands.json compile_commands.json

# 2. Native Linux Debug
build-debug:
	cmake -B build-linux-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
	cmake --build build-linux-debug
	ln -sf build-linux-debug/compile_commands.json compile_commands.json

# 3. Windows MinGW Release
build-win:
	cmake -B build-win-release -G Ninja -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-mingw64.cmake -DCMAKE_BUILD_TYPE=Release
	cmake --build build-win-release

# 4. Windows MinGW Debug
build-win-debug:
	cmake -B build-win-debug -G Ninja -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-mingw64.cmake -DCMAKE_BUILD_TYPE=Debug
	cmake --build build-win-debug

# Clean all build outputs
clean:
	rm -rf build-linux-release build-linux-debug build-win-release build-win-debug compile_commands.json