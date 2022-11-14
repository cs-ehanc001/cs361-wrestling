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
