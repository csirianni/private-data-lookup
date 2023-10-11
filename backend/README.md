# Backend

## Configuration

```bash
brew install conan cmake
```

```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

```bash
conan install . --output-folder=build --build=missing
```
