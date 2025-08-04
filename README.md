# Autholas C++ Authentication System

A C++ implementation for Autholas authentication service with hardware ID verification.

## Features

- User authentication via Autholas API
- Hardware ID generation using system information
- Cross-platform support (Windows/Linux)
- Built-in SHA-256 implementation
- Comprehensive error handling

## Prerequisites

Before building this project, you need to install the required dependencies:

### Required Libraries

1. **libcurl** - For HTTP requests
2. **nlohmann/json** - For JSON parsing

## Installation Guide

### Windows (Visual Studio)

#### Installing libcurl

**Option 1: Using vcpkg (Recommended)**
```bash
# Install vcpkg if you haven't already
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install

# Install curl
.\vcpkg install curl:x64-windows
```

**Option 2: Manual Installation**
1. Download libcurl from https://curl.se/download.html
2. Extract and build according to the documentation
3. Add include and library paths to your project

#### Installing nlohmann/json

**Option 1: Using vcpkg**
```bash
.\vcpkg install nlohmann-json:x64-windows
```

**Option 2: Header-only installation**
1. Download `json.hpp` from https://github.com/nlohmann/json
2. Place it in your include directory

### Linux (Ubuntu/Debian)

```bash
# Update package list
sudo apt update

# Install libcurl development files
sudo apt install libcurl4-openssl-dev

# Install nlohmann-json (Ubuntu 18.04+)
sudo apt install nlohmann-json3-dev

# For older versions, install manually:
# wget https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
# sudo cp json.hpp /usr/local/include/
```

### Linux (CentOS/RHEL/Fedora)

```bash
# For CentOS/RHEL
sudo yum install libcurl-devel
# or for newer versions
sudo dnf install libcurl-devel

# For Fedora
sudo dnf install libcurl-devel json-devel

# Install nlohmann-json manually if not available:
wget https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
sudo cp json.hpp /usr/local/include/nlohmann/
```

### macOS

```bash
# Using Homebrew
brew install curl nlohmann-json

# Using MacPorts
sudo port install curl json
```

## Building the Project

### Windows (Visual Studio)

1. Create a new C++ project
2. Add all source files to your project
3. Configure include directories for curl and nlohmann/json
4. Configure library directories and link against libcurl
5. Build the project

**CMake example:**
```cmake
cmake_minimum_required(VERSION 3.10)
project(AutholasAuth)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(autholas_auth main.cpp autholas.cpp)
target_link_libraries(autholas_auth CURL::libcurl nlohmann_json::nlohmann_json)
```

### Linux/macOS

```bash
# Using g++
g++ -std=c++17 main.cpp autholas.cpp -lcurl -o autholas_auth

# Using CMake
mkdir build
cd build
cmake ..
make
```

## Configuration

1. Open `autholas.cpp`
2. Replace `YOUR_API_KEY_HERE` with your actual Autholas API key:
   ```cpp
   const std::string API_KEY = "your_actual_api_key_here";
   ```

## Usage

1. Compile the project
2. Run the executable
3. Enter your username and password when prompted
4. The system will automatically generate a hardware ID and authenticate

```bash
./autholas_auth
```

## File Structure

```
├── autholas.h          # Header file with function declarations
├── autholas.cpp        # Implementation of all authentication functions
├── main.cpp            # Main program entry point
├── README.md           # This file
└── CMakeLists.txt      # CMake build configuration (optional)
```

## Error Handling

The system handles various authentication errors:

- `INVALID_CREDENTIALS` - Wrong username/password
- `USER_BANNED` - Account suspended
- `SUBSCRIPTION_EXPIRED` - Subscription ended
- `MAX_DEVICES_REACHED` - Device limit exceeded
- `HWID_BANNED` - Device banned
- `RATE_LIMIT_EXCEEDED` - Too many attempts

## Security Features

- Hardware ID generation using system information
- SHA-256 hashing for device identification
- Secure HTTPS communication with Autholas API
- No sensitive data stored locally

## Dependencies

- **libcurl**: HTTP client library for API communication
- **nlohmann/json**: Modern C++ JSON library for parsing responses
- **Standard C++ libraries**: iostream, string, map, sstream, iomanip, vector

## Compatibility

- **Windows**: Windows 7+ with Visual Studio 2017+
- **Linux**: Most distributions with GCC 7+ or Clang 5+
- **macOS**: macOS 10.12+ with Xcode 9+

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is provided as-is for educational and development purposes.

## Support

For issues related to:
- **Autholas API**: Contact Autholas support
- **Code issues**: Create an issue in this repository