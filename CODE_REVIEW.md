# Code Review

I've been writing code for some time but never had the need (or opportunity) to
write C or C++ code. Because of this I know way less than I woud like to about
compiling, linking, ordering and distributing this kind of code.

To learn a thing or two about C++ I wrote this code. The code itself works fine
as far as I can tell and I didn't really have any coding relating issues (at
least not just to get it to work). However, I struggeled a lot with compiling
and linking the code. To be sure I'm not doing anything stupid I would like some
feedback on my code.

## The Code

* Does the code itself look OK?
* Are there any issues (big or small) that I've missed (bugs, memory leaks etc).
* How far off C++ pragma is it? What would be a more C++ way of things?

## Project layout

This is a really small project but even with a single `.cpp` file I feel
insecure about the project layout.

* Is the layout reasonable?
* Are files in the right place and named correctly?

## Testing

I always want to unit test my code but I didn't find a superior solution. Right
now I'm building an executable with the unit tests and using Catch2.

* Could this be done better?
* How could I add some kind of target so e.g. `cmake --test` would work?
* When assertions fail, the variable names are printed. Becuase of that I create
  a label and use `SECTION` for each test. How could this be improved?

## (c)make

Since I'm new to C and C++ I feel really insecure about `cmake` and
`CMakeLists.txt`.

* Should I use a regular `Makefile`?
* How is my `CMakeLists.txt`?
* Should I use separate `CMakeList.txt` for the library and the executable
  (unittest)?
* Should I generate or manage `CMakeLists.txt` or is manual work OK? (Is it even
  possible to generate?)

## Building

So, obviously this is working and this is how I build the code.

**Configure**

```sh
cmake \
    --no-warn-unused-cli \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang \
    -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++ \
    -H$(pwd) \
    -B$(pwd)/build
```

**Build**

```sh
cmake \
    --build $(pwd)/build \
    --config Debug \
    --target all \
    -- \
    -j 10
```

* Is this a correct way to do it?

## Files and headers

To be frank I'm still not really sure about the best practices around files
with code and header files. As I understand it you cannot reference code that's
not yet been seen by the compiler. One way to work around this is to define your
functions in a header file and by including those definitions (like `#include
<personnummer.cpp>` I can then write my code in whatever order I want.

* Is this a correct assumption about header files?
* Is the way I defined methods and struct in the header file OK?

## Distributing

I wanted to write this code to be able to distribute the library. I noticed by
using Catch2 that you can distribute your code in a single header file. This is
not the way I went, mostly because I don't understand how. I've understood that
one reasone to distribute the library as source code is that the consumer can
compile the code for a given architecture instead of me having to deal with
those things in the potential header file.

* Is distributing as source code a good way?
* If someone wants to use this code, is their best bet to compile the library
  according to `CMakeLists.txt` and then link said library with their own
  application?
* If someone were to release an application dependent on this code, how would
  they this be bundled?
  * Like a submodule?
  * Just linked in a README?
  * Bundled with compiled library?
