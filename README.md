# cpp-personnummer

Swedish social security number validation implemented in C++.

## Build

Use `cmake` and/or `CMakeLists`.

```sh
# Configure
cmake \
    --no-warn-unused-cli \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++ \
    -H$(pwd) \
    -B$(pwd)/build

# Build
cmake \
    --build $(pwd)/build \
    --config Debug \
    --target all \
    -- \
    -j 10
```

## Usage

* Use `cmake` (or any other preferred method) to build static library.
* Import `personnummer.hpp`
* Link library with your code `g++ -Wall -I/include/path -L/library/path
  your_program.cpp -lPersonnummer`

```cpp
#include "personnummer.hpp"

int main() {
	Personnummer::Personnummer p;
	std::string pnr = "19900101-0007";

	bool is_valid = Personnummer::from_string(pnr, p);

	if (!is_valid) {
		std::cout << "could not parse social security number (nvalid syntax)\n";
		return 1;
	}

    // Check date and luhn separate
	if (p.is_valid_date()) {
		std::cout << "invalid date\n";
		return 1;
	}

	if (p.is_valid_luhn()) {
		std::cout << "invalid last four digits\n";
		return 1;
	}

	// Or just check personnummer validity
	if (!p.valid()) {
		std::cout << "invalid social security number\n";
		return 1;
	}

	std::cout << pnr << "is valid!\n";

	return 0;
}
```

## Testing

Tests are written with [Catch2](https://github.com/catchorg/Catch2). To make the
unittest compile the `Catch2` header is bundled in the `test` directory.

After building, run the executable `unittest`.

```sh
./build/unittest
```

## Format

Code is (and should continue to be) formatted with `clang-format` and `llvm`
style.

```sh
$ clang-format -i -style=llvm src/*.{cpp,hpp} test/unittest.cpp
```
