# Backend

## Configuration

```bash
brew install conan cmake
```

```bash
mkdir build && cd build
cmake .. && cmake --build .
```

```bash
conan install . --output-folder=build --build=missing
```
