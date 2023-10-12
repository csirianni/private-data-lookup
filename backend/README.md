# Backend

## Configuration

install conan:
```bash
brew install conan cmake
conan install . --output-folder=build --build=missing
```

set up build:
```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

start server:
```bash
cd build && ./backend
```
