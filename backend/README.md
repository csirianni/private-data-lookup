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

From `/backend`, start the server with our precomputed database:

```bash
build/src/server build/passwords.db 
```

or specify a new database path with the build flag:
```bash
build/src/server <path_to_database> --build
```


To fix VS Code import errors with Crow, try adding the following line to your `settings.json`:

```json
"C_Cpp.default.compileCommands": "${workspaceFolder}/build/compile_commands.json",
```

[Source](https://stackoverflow.com/questions/58077908/linking-conan-include-to-vs-code)

## Testing

After building, you can run tests from `/backend`:

```bash
cd build && ./tests
```
