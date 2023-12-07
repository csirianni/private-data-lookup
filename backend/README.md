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

From `/backend`, start the server. Use the `--build` flag to create or rebuild a database for the breached passwords or omit it to use an existing one:

```bash
build/src/server <database filepath> --build
```

```bash
build/src/server data/passwords.db 
```

To fix VS Code import errors with Crow, try adding the following line to your `settings.json`:

```json
"C_Cpp.default.compileCommands": "${workspaceFolder}/build/compile_commands.json",
```

[Source](https://stackoverflow.com/questions/58077908/linking-conan-include-to-vs-code)

## Testing

After building, you can run tests from `/backend`:

```bash
cd build && ./test/pdl_test	
```

or alternatively:
```bash
make check
```
