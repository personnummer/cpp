# cpp-personnummer

Swedish social security number validation implemented in C++.

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

```cpp
#include <string>
#include "personnummer.hpp"

int main() {
    std::string ssn = "19900101-0007"
    Personnummer pnr(ssn);

    if (!pnr.valid()) {
        std::cout << "invalid social security number\n";
        return 1;
    }

    std::cout << ssn << "is valid!\n";

    return 0;
}
```

## Testing

Tests are written with [Catch2](https://github.com/catchorg/Catch2). To make the
unittest compile the `Catch2` header is bundled in the `test` directory.

After building (with `WITH_TEST=1`), run the executable `unittest`.

```sh
./build/test/unittest
```

Or use the make target in `build/Makefile` and run `make test`.

## Format

Code is (and should continue to be) formatted with `clang-format` and `llvm`
style.

```sh
$ clang-format -i -style=llvm src/*.{cpp,hpp} test/unittest.cpp
```
