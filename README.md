# MasterMind Trader - Professional Trading System

A comprehensive high-frequency trading system with beautiful GUI interface, built in C++ with Qt framework.

## Features

### Core Trading System
- **Renko Chart Pattern Recognition**: Advanced pattern detection based on two proprietary setups
- **Multi-Exchange Support**: Binance, Coinbase, Deribit, Delta Exchange, MetaTrader 4/5
- **High-Frequency Trading**: Low-latency order execution and market data processing
- **Risk Management**: Advanced capital protection with position sizing and drawdown controls
- **Paper Trading**: Safe simulation mode for strategy testing

### Beautiful GUI Interface
- **Modern Dark Theme**: Professional trading interface with customizable layouts
- **Real-time Dashboard**: Live metrics, charts, and trading statistics
- **Position Management**: Visual position tracking with P&L monitoring
- **Trading Log**: Real-time activity logging with export capabilities
- **Settings Panel**: Comprehensive configuration for all trading parameters

### Advanced Features
- **Counter Trading**: Automated fallback to paper trading after losses
- **Session Management**: Configurable trading hours and swap avoidance
- **Capital Allocation**: Per-symbol capital assignment and management
- **Audit Trail**: Complete trade logging for compliance and analysis
- **System Tray**: Background operation with system notifications

## System Requirements

- **Operating System**: Windows 10/11, macOS 10.14+, or Linux
- **Qt Framework**: Qt 5.15+ or Qt 6.2+
- **C++ Compiler**: GCC 7+, Clang 9+, or MSVC 2019+
- **CMake**: Version 3.16 or higher
- **RAM**: Minimum 4GB (8GB recommended)
- **Storage**: 500MB free space

## Installation

### Prerequisites

1. **Install Qt Framework**:
   - Download from https://www.qt.io/download
   - Install Qt 5.15+ or Qt 6.2+ with Charts module

2. **Install CMake**:
   - Download from https://cmake.org/download/
   - Ensure CMake is in your PATH

3. **Install C++ Compiler**:
   - **Windows**: Visual Studio 2019+ or MinGW
   - **macOS**: Xcode Command Line Tools
   - **Linux**: GCC development packages

### Building from Source

```bash
# Clone the repository
git clone https://github.com/yourusername/mastermind-trader.git
cd mastermind-trader

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build . --config Release

# Run the application
./MasterMindTrader
```

### Windows-specific Instructions

```powershell
# Using Visual Studio
cmake -G "Visual Studio 16 2019" -A x64 ..
cmake --build . --config Release

# Using MinGW
cmake -G "MinGW Makefiles" ..
mingw32-make
```

## Configuration

### First Run Setup

1. **Launch the application**
2. **Configure API credentials** in Settings > Exchange tab
3. **Set risk parameters** in Settings > Risk tab
4. **Choose trading strategy** in Settings > Trading tab
5. **Test with paper trading** before going live

### Risk Management Configuration

- **Max Risk per Trade**: 0.1% - 10% of account equity
- **Max Daily Risk**: 1% - 50% of account equity
- **Max Open Positions**: 1 - 20 simultaneous positions
- **Max Drawdown**: 5% - 50% maximum account drawdown
- **Counter Trading**: Enable automatic fallback to paper trading

### Strategy Settings

- **Setup 1**: Two consecutive red bricks + green brick pattern
- **Setup 2**: Three-brick pattern recognition (green-red-green, red-green-red)
- **Brick Size**: 0.1 - 1000 pips depending on instrument
- **Tick Buffer**: 0 - 10 tick buffer for entry confirmation

## Usage

### Basic Trading Workflow

1. **Start the Application**
2. **Select Exchange and Symbol**
3. **Configure Risk Parameters**
4. **Enable Paper Trading** (recommended for testing)
5. **Click "Start Trading"**
6. **Monitor Dashboard** for real-time metrics
7. **Review Logs** for trade activity

### Dashboard Overview

- **Trading Control Panel**: Start/stop trading, mode selection
- **System Status**: Connection, trading status, equity display
- **Risk Management**: Real-time risk usage and limits
- **Position Table**: Open positions with P&L
- **Trading Log**: Chronological activity log

### Advanced Features

- **Live Charts**: Real-time price charting with indicators
- **Trade History**: Complete trading history with analytics
- **Settings Dialog**: Comprehensive configuration options
- **System Tray**: Minimize to system tray for background operation

## Safety Features

### Risk Controls
- **Automatic stop-loss** adjustment after favorable moves
- **Daily risk limits** with automatic trading halt
- **Position size limits** based on account equity
- **Drawdown monitoring** with emergency stop

### Paper Trading Fallback
- **Automatic activation** after consecutive losses
- **Simulated trading** until profitability returns
- **Safe testing environment** for strategy validation

## Troubleshooting

### Common Issues

1. **CMake Configuration Errors**:
   - Ensure Qt is properly installed and in PATH
   - Check CMake version compatibility
   - Verify all dependencies are available

2. **Compilation Errors**:
   - Update to latest Qt version
   - Check C++ compiler compatibility
   - Ensure all required Qt modules are installed

3. **Runtime Issues**:
   - Verify Qt libraries are in system PATH
   - Check log files for error messages
   - Ensure proper API credentials configuration

### Getting Help

- **Documentation**: Check inline help and tooltips
- **Log Files**: Review `logs/trading.log` for detailed information
- **Support**: Contact support for technical assistance

## License

This project is proprietary software. All rights reserved.

**Important**: This trading system is for educational and demonstration purposes. Trading involves significant risk and can result in substantial losses. Always test thoroughly with paper trading before using real money.

## Disclaimer

- **No Trading Advice**: This software does not provide trading advice
- **Risk Warning**: Trading involves substantial risk of loss
- **No Warranty**: Software is provided "as is" without warranty
- **User Responsibility**: Users are responsible for their trading decisions

## Version History

- **v1.0.0**: Initial release with core trading functionality
- **Future**: Real-time market data integration, advanced charting, backtesting

---

© 2024 MasterMind Trading Systems. All rights reserved. 