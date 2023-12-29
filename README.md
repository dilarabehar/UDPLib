
# udplib
UDP Library

This project is a Server-based software development project developed on **Linux**, with communication provided within the scope of **UDP protocol**.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to **deploy** the project on a live system.

### Dependencies

To compile and use this project, you will need the following dependencies:

* [CMake](https://cmake.org/features/)(Minimum Version: 3.0) : The Standard Build System
* [g++ or another C++ compiler] (C++11 compatible): A C++ compiler is required to compile the project.
* [googletest](https://github.com/google/googletest) : Testing framework

### Installing

Clone the repository:

```
git clone https://github.com/dilarabehar/UDPLib.git
```

Navigate to the project directory:

```
cd udp-server-library
```

Configure the build with CMake:

```
mkdir build
cd build
cmake ..

```

Build the project:

```
make
```

Once the compilation process is complete, the library will be ready for use.

## Running the tests
```
g++ -std=c++17 -pthread -o testExecutable test.cpp -lgtest_main -lgtest
```

## Built With

* [Ubuntu](https://help.ubuntu.com/?_ga=2.16077302.1738386347.1695980326-1628496463.1693213598) - Linux operating system.
* [WSL](https://learn.microsoft.com/en-us/windows/wsl/install#install-wsl-command) - The Windows Subsystem for Linux
* [CMake](https://cmake.org/features/) - The Standard Build System


## Authors

* **Dilara Behar** -  [dilarabehar](https://github.com/dilarabehar)

