# MasterMind Trader - Setup Guide

This guide will help you install all required tools and dependencies to build and run the MasterMind Trader.

## Required Tools Installation

### 1. Install Qt Framework

#### Option A: Qt Online Installer (Recommended)
1. **Download**: Go to https://www.qt.io/download-open-source
2. **Run installer**: Download and run the Qt Online Installer
3. **Login**: Create a free Qt account or login
4. **Select Components**:
   - Qt 6.8.3 or later (recommended)
   - Qt 6.8.3 or later (minimum)
   - **Must include**: Qt Charts, Qt WebSockets, Qt SQL
   - **Compiler**: MinGW 11.2.0 64-bit (Windows) or appropriate for your OS
5. **Installation Path**: Default is fine (usually `C:\Qt`)

#### Option B: Qt Maintenance Tool
If you already have Qt installed:
1. Run Qt Maintenance Tool
2. Add Components → Qt Charts, Qt WebSockets, Qt SQL

### 2. Install CMake

#### Windows
1. **Download**: Go to https://cmake.org/download/
2. **Choose**: "Windows x64 Installer" (.msi file)
3. **Install**: Run installer and **check "Add CMake to system PATH"**
4. **Verify**: Open new Command Prompt and run `cmake --version`

#### macOS
```bash
# Using Homebrew
brew install cmake

# Or download from https://cmake.org/download/
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install cmake build-essential
```

### 3. Install C++ Compiler

#### Windows
**Option A: MinGW (Recommended - comes with Qt)**
- Already included if you installed Qt with MinGW
- Path: `C:\Qt\Tools\mingw1310_64\bin`

**Option B: Visual Studio**
1. Download Visual Studio Community (free)
2. Install with "Desktop development with C++" workload

#### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install
```

#### Linux
```bash
# Ubuntu/Debian
sudo apt install build-essential g++ gdb

# CentOS/RHEL
sudo yum groupinstall "Development Tools"
```

### 4. Install Git (Optional but Recommended)
1. **Download**: https://git-scm.com/download
2. **Install**: Use default settings
3. **Verify**: Run `git --version`

## Environment Setup

### Windows Environment Variables
Add these to your system PATH:
```
C:\Qt\6.8.3\mingw_64\bin
C:\Qt\Tools\mingw1310_64\bin
C:\Qt\Tools\CMake_64\bin
```

### Quick Setup Script (Windows)
Create `setup_env.bat`:
```batch
@echo off
echo Setting up MasterMind Trader environment...

:: Add Qt to PATH
set PATH=C:\Qt\6.8.3\mingw_64\bin;%PATH%
set PATH=C:\Qt\Tools\mingw1310_64\bin;%PATH%
set PATH=C:\Qt\Tools\CMake_64\bin;%PATH%

:: Set Qt environment variables
set QTDIR=C:\Qt\6.8.3\mingw_64
set QT_PLUGIN_PATH=%QTDIR%\plugins

echo Environment setup complete!
echo You can now run: build.bat
```

## Verification

### Test Your Installation
Create `test_setup.bat`:
```batch
@echo off
echo Testing MasterMind Trader setup...

echo.
echo Testing CMake...
cmake --version
if %errorlevel% neq 0 (
    echo ERROR: CMake not found!
    goto :error
)

echo.
echo Testing Qt...
qmake --version
if %errorlevel% neq 0 (
    echo ERROR: Qt not found!
    goto :error
)

echo.
echo Testing C++ Compiler...
g++ --version
if %errorlevel% neq 0 (
    echo ERROR: G++ compiler not found!
    goto :error
)

echo.
echo SUCCESS: All tools are properly installed!
echo You can now build MasterMind Trader.
goto :end

:error
echo.
echo SETUP INCOMPLETE: Please install missing tools.
echo See SETUP.md for detailed instructions.

:end
pause
```

## Build Instructions

### Quick Build
```bash
# Clone/navigate to project directory
cd MasterMindTrader

# Run setup (Windows)
setup_env.bat

# Build the project
build.bat

# Or manually:
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Advanced Build Options

#### Debug Build
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
```

#### Specific Generator
```bash
# Visual Studio
cmake .. -G "Visual Studio 16 2019" -A x64

# MinGW
cmake .. -G "MinGW Makefiles"

# Ninja (if installed)
cmake .. -G "Ninja"
```

## Troubleshooting

### Common Issues

#### "CMake not found"
- Ensure CMake is installed and in PATH
- Restart terminal/command prompt
- On Windows: Check Environment Variables

#### "Qt not found"
- Verify Qt installation path
- Set QTDIR environment variable
- Ensure Qt bin directory is in PATH

#### "Compiler not found"
- Install MinGW (with Qt) or Visual Studio
- Ensure compiler is in PATH
- Check if 32-bit vs 64-bit matches

#### "Charts module not found"
- Install Qt Charts module via Qt Maintenance Tool
- Ensure all Qt modules are installed

### Build Errors

#### "No CMAKE_CXX_COMPILER could be found"
```bash
# Specify compiler explicitly
cmake .. -DCMAKE_CXX_COMPILER=g++
```

#### "Qt6 not found"
```bash
# Use Qt6 instead
cmake .. -DUSE_Qt6=ON
```

#### "WebSockets not found"
```bash
# Install Qt WebSockets module
# Or disable WebSockets temporarily
cmake .. -DDISABLE_WEBSOCKETS=ON
```

## Alternative Installation Methods

### Portable Installation
1. Download Qt offline installer
2. Extract to portable location
3. Update build scripts with absolute paths

### Docker Setup (Advanced)
```dockerfile
FROM ubuntu:20.04
RUN apt-get update && apt-get install -y \
    Qt6-base-dev Qt6-charts-dev Qt6-websockets-dev \
    cmake build-essential git
```

### Package Managers

#### Windows (Chocolatey)
```powershell
choco install qt-creator cmake git
```

#### macOS (Homebrew)
```bash
brew install qt cmake git
```

#### Linux (APT)
```bash
sudo apt install Qt6-base-dev Qt6-charts-dev cmake build-essential
```

## Next Steps

After successful installation:
1. Run `test_setup.bat` to verify everything works
2. Run `build.bat` to compile the project
3. Execute `./build/MasterMindTrader` to run the application
4. Configure your trading settings in the GUI

## Support

If you encounter issues:
1. Check this troubleshooting guide
2. Verify all tools are properly installed
3. Ensure environment variables are set correctly
4. Try building a simple Qt project first

---

**Note**: This setup is required only once. After installation, you can build and run the project easily with the provided scripts. 