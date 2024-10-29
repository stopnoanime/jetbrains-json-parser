# jetbrains-json-parser
JSON parser and query evaluator made as a JetBrains internship recrutation task.

## Compiling
Prerequisites: You need make, CMake and GCC. 
I used Fedora Linux and GCC 14.2.1.

To build you can either manually invoke CMake:
```
mkdir build
cd build
cmake ../src/
cmake --build .
```
Or use the convenience script defined in makefile by running:
`make all`

## Running
After compiling, the binary will be generated in `./build/json_eval`.
You can invoke it like this: 
```
./build/json_eval filename.json "query"
```

## Testing
This project has unit test made in Catch2. After compiling you can run them by running:
```
./build/tests/tests
```
