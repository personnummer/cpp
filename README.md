# cpp-personnummer

Validate Swedish [personal identity
numbers](<https://en.wikipedia.org/wiki/Personal_identity_number_(Sweden)>) with
[C++](https://isocpp.org/)

## Build

Use `cmake`

### Configure

```sh
cmake \
    --no-warn-unused-cli \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++ \
    -DWITH_TEST=1 \
    -DWITH_EXAMPLES=1 \
    -H$(pwd) \
    -B$(pwd)/build
```

### Build

```sh
cmake \
    --build $(pwd)/build \
    --config Debug \
    --target all \
    -- \
    -j 10
```

## Usage

* Build the library
* Import `personnummer.hpp`
* Link library with your code `g++ -Wall -I/include/path -L/library/path
  your_program.cpp -lPersonnummer`

See [examples](./examples) for code examples.

## Testing

Tests are written with [Catch2](https://github.com/catchorg/Catch2). To make the
unittest compile the `Catch2` header is bundled in the `test` directory.

After building (with `WITH_TEST=1`), run the executable `unittest`.

```sh
./build/test/unittest
```

Or use the make target in `build/Makefile` and run `make test`.

## Format

Code is (and should continue to be) formatted with `clang-format`, the default
settings for VS Code.

```sh
$ clang-format -i src/*.{cpp,hpp} test/unittest.cpp
```
