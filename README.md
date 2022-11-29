# CS361 Wrestling Project

## Build Instructions

```sh
mkdir build
cd build
cmake ../cpp -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel 4
```

There will be two binaries: `run_tests` and `wrestling`.
`run_tests` will run the tests. `wrestling` will begin a simulation.

## Building Doxygen Documentation

Run the following commands:

```
cd doc
doxygen
```

Then open `./doc/html/index.html` in your browser of choice. Internet Explorer may experience issues.

## Academic Integrity Statement

With little exception, every file committed to this repository was written entirely by me, Ethan Hancock.
The exceptions are as follows:

* ./doc/css/*
    * The doxygen CSS is a third-party project, which can be found here: https://github.com/jothepro/doxygen-awesome-css
* ./doc/Doxyfile
    * This file was created by dumping the default configuration, then I made adjustments to suit my needs
* ./cpp/.clang-*
    * These two files were created by dumping the default configuration of the respective tool,
    then I made adjustments to suit my needs
* ./cpp/inc/supl/*
    * These files were written entirely by me, however, development of them has been done in a separate git repository,
    which can be found here: https://gitlab.com/theomegacarrot/cpp-supplementaries
