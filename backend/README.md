# Backend

## Configuration

In `/backend`, install conan:

```bash
brew install conan cmake
conan install . --output-folder=build --build=missing
```

Set up the `/build` folder:

```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

From `/backend`, start the server:

```bash
cd build && ./backend
```
