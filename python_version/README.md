# MasterMind Trader - Python Version

Professional High-Frequency Trading System based on Renko Chart Pattern Recognition

## Features

🎯 **Core Trading Features:**
- **Renko Pattern Recognition**: Automated detection of Setup 1 and Setup 2 patterns
- **Real-time Price Simulation**: Realistic price movement simulation with trends
- **Risk Management**: Comprehensive risk controls with daily limits
- **Paper Trading**: Safe simulation mode for testing strategies
- **Multi-Exchange Support**: Binance, Coinbase, Deribit, Delta Exchange, MetaTrader 4/5
- **Live Trading Mode**: Real money trading with safety confirmations

🎨 **Beautiful User Interface:**
- **Modern GUI**: Clean, professional interface using tkinter
- **Real-time Charts**: Live price charts with matplotlib integration
- **Trading Dashboard**: Comprehensive overview of trading metrics
- **Position Management**: Real-time position tracking and P&L monitoring
- **Live Logging**: Real-time trading log with detailed information

⚡ **Advanced Features:**
- **Multithreaded Architecture**: Separate threads for trading and UI
- **Configuration Management**: JSON-based configuration with auto-save
- **Pattern Recognition**: Advanced Renko brick pattern detection
- **Risk Controls**: Position sizing, daily risk limits, emergency stops
- **Performance Metrics**: Win rate, profit factor, drawdown tracking

## Installation

### Prerequisites

- Python 3.7 or higher
- pip package manager

### Setup

1. **Clone/Download the project:**
```bash
git clone <repository-url>
cd python_version
```

2. **Install dependencies:**
```bash
pip install -r requirements.txt
```

3. **Run the application:**
```bash
python mastermind_trader.py
```

## Quick Start

### First Time Setup

1. **Launch the application**
2. **Verify settings** in the left panel:
   - Exchange: Binance (default)
   - Symbol: BTCUSD (default)
   - Trading Mode: Paper Trading (recommended)
   - Risk settings: 2% per trade, 10% daily max

3. **Configure strategy:**
   - Brick Size: 10 pips (default)
   - Setup 1: Two red bricks + green brick (enabled)
   - Setup 2: Three brick pattern (enabled)

4. **Start trading:**
   - Click "Start Trading" button
   - Watch the price chart and log for activity
   - Monitor positions in the Positions tab

### Understanding the Interface

#### Left Panel - Trading Control
- **Control Buttons**: Start/Stop trading
- **Trading Mode**: Paper vs Live trading selection
- **Market Selection**: Exchange and symbol configuration
- **Strategy Settings**: Renko brick size and pattern selection
- **System Status**: Real-time connection and trading status
- **Risk Management**: Risk per trade and daily limits

#### Right Panel - Trading Dashboard
- **Dashboard Tab**: Live price chart and trading metrics
- **Positions Tab**: Real-time position tracking
- **Logs Tab**: Detailed trading log with timestamps

## Configuration

### Strategy Configuration

```python
# Renko Settings
brick_size = 10.0  # Size of each Renko brick in pips
setup1_enabled = True  # Two red bricks + green brick pattern
setup2_enabled = True  # Three brick alternating pattern
counter_trading = False  # Counter-trend trading (experimental)
```

### Risk Management

```python
# Risk Settings
max_risk_per_trade = 2.0  # Maximum risk per trade (% of equity)
max_daily_risk = 10.0     # Maximum daily risk (% of equity)
max_open_positions = 5    # Maximum simultaneous positions
```

### Trading Modes

**Paper Trading (Default):**
- Simulated trades with no real money
- Perfect for testing strategies
- No risk to capital
- Full functionality testing

**Live Trading:**
- Real money trading (requires API setup)
- Confirmation dialog for safety
- All risk management controls active
- Use only after thorough testing

## Renko Pattern Recognition

### Setup 1: Two Red + Green
- Pattern: Red → Red → Green
- Signal: BUY signal when detected
- Logic: Reversal after two consecutive down moves

### Setup 2: Three Brick Pattern
- Pattern: Green → Red → Green OR Red → Green → Red
- Signal: SELL signal when detected
- Logic: Reversal after alternating pattern

### Brick Formation
- Bricks form when price moves by brick_size amount
- Only considers closing prices
- Ignores time, focuses on price movement
- 75% formation threshold for partial bricks

## Risk Management

### Position Sizing
- Based on percentage of equity
- Automatic calculation based on stop loss
- Adjusts for current price levels
- Prevents over-leveraging

### Daily Risk Limits
- Tracks total risk exposure
- Stops trading when limit reached
- Resets daily (manual reset available)
- Visual progress indicator

### Emergency Controls
- Immediate stop all trading
- Close all open positions
- Risk limit breached protection
- Connection loss handling

## Trading Simulation

### Price Movement
- Realistic price volatility
- Trend-based movement
- Random walk with drift
- Configurable parameters

### Trade Execution
- Spread simulation
- Slippage modeling
- Realistic fill prices
- Position tracking

### Performance Metrics
- Win rate calculation
- Profit factor tracking
- Drawdown monitoring
- Trade statistics

## Troubleshooting

### Common Issues

**"Module not found" error:**
```bash
pip install -r requirements.txt
```

**Chart not displaying:**
- Ensure matplotlib is installed
- Check Python tkinter installation
- Try running with `python -m tkinter` test

**Trading not starting:**
- Check strategy settings are enabled
- Verify risk limits are not exceeded
- Ensure proper configuration loaded

### Performance Tips

- Close unnecessary programs for better performance
- Use paper trading mode for testing
- Monitor system resources during trading
- Save configuration regularly

## Advanced Usage

### Custom Strategies
Modify the `RenkoPatternEngine` class to implement custom patterns:

```python
def check_custom_pattern(self) -> bool:
    """Implement your custom pattern logic"""
    if len(self.bricks) < 4:
        return False
    
    # Your pattern logic here
    return pattern_detected
```

### API Integration
For live trading, integrate with exchange APIs:

```python
class ExchangeConnector:
    def __init__(self, api_key, api_secret):
        self.api_key = api_key
        self.api_secret = api_secret
        
    def place_order(self, symbol, side, size, price):
        # Implement exchange API calls
        pass
```

### Data Logging
Enhanced logging with JSON format:

```python
import json
import logging

# Configure structured logging
logging.basicConfig(
    format='%(asctime)s %(levelname)s %(message)s',
    level=logging.INFO,
    handlers=[
        logging.FileHandler('trading.log'),
        logging.StreamHandler()
    ]
)
```

## Safety Notice

⚠️ **Important Risk Warnings:**

1. **Paper Trading First**: Always test thoroughly with paper trading before using real money
2. **Risk Management**: Never risk more than you can afford to lose
3. **Strategy Testing**: Backtest strategies on historical data before live trading
4. **Capital Protection**: Use proper position sizing and stop losses
5. **System Monitoring**: Monitor system performance and connectivity
6. **Legal Compliance**: Ensure compliance with local trading regulations

## Support

For issues or questions:
1. Check the troubleshooting section
2. Review the configuration settings
3. Test with paper trading mode
4. Check Python and package versions

## License

This software is provided for educational and testing purposes. Use at your own risk.

---

**Happy Trading! 🚀**

*Remember: The best traders are risk managers first, profit makers second.* 