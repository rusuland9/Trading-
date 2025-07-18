#!/usr/bin/env python3
"""
MasterMind Trader - Python Version
Professional High-Frequency Trading System
Based on Renko Chart Pattern Recognition
"""

import tkinter as tk
from tkinter import ttk, messagebox
import json
import threading
import time
import random
import queue
from datetime import datetime, timedelta
from typing import Dict, List, Optional, Tuple
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import matplotlib.animation as animation
from collections import deque
import numpy as np

class RenkoPatternEngine:
    """Renko pattern recognition engine"""
    
    def __init__(self, brick_size: float = 10.0):
        self.brick_size = brick_size
        self.bricks = deque(maxlen=100)
        self.formation_threshold = 0.75
        
    def add_price(self, price: float) -> List[str]:
        """Add price and return any new bricks formed"""
        new_bricks = []
        
        if not self.bricks:
            self.bricks.append(('green', price))
            return ['green']
        
        last_brick_type, last_price = self.bricks[-1]
        
        # Check for new brick formation
        if price >= last_price + self.brick_size:
            # Green brick
            new_bricks.append('green')
            self.bricks.append(('green', price))
        elif price <= last_price - self.brick_size:
            # Red brick
            new_bricks.append('red')
            self.bricks.append(('red', price))
        
        return new_bricks
    
    def check_setup1(self) -> bool:
        """Check for Setup 1: Two red bricks followed by green brick"""
        if len(self.bricks) < 3:
            return False
        
        recent_bricks = list(self.bricks)[-3:]
        return (recent_bricks[0][0] == 'red' and 
                recent_bricks[1][0] == 'red' and 
                recent_bricks[2][0] == 'green')
    
    def check_setup2(self) -> bool:
        """Check for Setup 2: Three brick pattern"""
        if len(self.bricks) < 3:
            return False
        
        recent_bricks = list(self.bricks)[-3:]
        pattern1 = (recent_bricks[0][0] == 'green' and 
                   recent_bricks[1][0] == 'red' and 
                   recent_bricks[2][0] == 'green')
        
        pattern2 = (recent_bricks[0][0] == 'red' and 
                   recent_bricks[1][0] == 'green' and 
                   recent_bricks[2][0] == 'red')
        
        return pattern1 or pattern2

class Trade:
    """Trading position representation"""
    
    def __init__(self, symbol: str, side: str, size: float, entry_price: float):
        self.symbol = symbol
        self.side = side
        self.size = size
        self.entry_price = entry_price
        self.current_price = entry_price
        self.pnl = 0.0
        self.status = "OPEN"
        self.timestamp = datetime.now()
        
    def update_price(self, price: float):
        """Update current price and calculate P&L"""
        self.current_price = price
        if self.side == "BUY":
            self.pnl = (price - self.entry_price) * self.size
        else:  # SELL
            self.pnl = (self.entry_price - price) * self.size
    
    def close(self):
        """Close the trade"""
        self.status = "CLOSED"

class RiskManager:
    """Risk management system"""
    
    def __init__(self, max_risk_per_trade: float = 2.0, max_daily_risk: float = 10.0):
        self.max_risk_per_trade = max_risk_per_trade
        self.max_daily_risk = max_daily_risk
        self.daily_risk_used = 0.0
        self.max_open_positions = 5
        
    def can_trade(self, risk_amount: float, open_positions: int) -> bool:
        """Check if we can place a trade"""
        if open_positions >= self.max_open_positions:
            return False
        
        if risk_amount > self.max_risk_per_trade:
            return False
        
        if self.daily_risk_used + risk_amount > self.max_daily_risk:
            return False
        
        return True
    
    def add_risk(self, risk_amount: float):
        """Add risk to daily usage"""
        self.daily_risk_used += risk_amount
    
    def reset_daily_risk(self):
        """Reset daily risk (call at start of new day)"""
        self.daily_risk_used = 0.0

class MasterMindTrader:
    """Main trading application"""
    
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("MasterMind Trader - Python Version")
        self.root.geometry("1400x900")
        
        # Trading state
        self.trading_active = False
        self.paper_trading = True
        self.current_symbol = "BTCUSD"
        self.current_exchange = "Binance"
        self.current_price = 50000.0
        self.current_equity = 10000.0
        self.daily_pnl = 0.0
        self.open_trades = []
        self.closed_trades = []
        self.total_trades = 0
        self.winning_trades = 0
        
        # Price simulation
        self.price_history = deque(maxlen=100)
        self.price_direction = 1
        self.price_volatility = 0.001
        
        # Initialize components
        self.renko_engine = RenkoPatternEngine(brick_size=10.0)
        self.risk_manager = RiskManager(max_risk_per_trade=2.0, max_daily_risk=10.0)
        
        # Threading
        self.trading_thread = None
        self.stop_trading_event = threading.Event()
        self.message_queue = queue.Queue()
        
        # Setup UI
        self.setup_ui()
        self.setup_chart()
        
        # Start message processing
        self.process_messages()
        
        # Load configuration
        self.load_config()
        
        # Initialize price history
        for i in range(50):
            self.price_history.append(self.current_price + random.uniform(-100, 100))
        
    def setup_ui(self):
        """Setup the user interface"""
        # Create main frame
        main_frame = ttk.Frame(self.root)
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # Left panel
        left_frame = ttk.LabelFrame(main_frame, text="Trading Control", padding=10)
        left_frame.pack(side=tk.LEFT, fill=tk.Y, padx=(0, 10))
        
        # Control buttons
        self.start_button = ttk.Button(left_frame, text="Start Trading", command=self.start_trading)
        self.start_button.pack(fill=tk.X, pady=2)
        
        self.stop_button = ttk.Button(left_frame, text="Stop Trading", command=self.stop_trading, state=tk.DISABLED)
        self.stop_button.pack(fill=tk.X, pady=2)
        
        ttk.Separator(left_frame, orient=tk.HORIZONTAL).pack(fill=tk.X, pady=10)
        
        # Trading mode
        mode_frame = ttk.LabelFrame(left_frame, text="Trading Mode", padding=5)
        mode_frame.pack(fill=tk.X, pady=5)
        
        self.trading_mode = tk.StringVar(value="paper")
        ttk.Radiobutton(mode_frame, text="Paper Trading", variable=self.trading_mode, 
                       value="paper", command=self.update_trading_mode).pack(anchor=tk.W)
        ttk.Radiobutton(mode_frame, text="Live Trading", variable=self.trading_mode, 
                       value="live", command=self.update_trading_mode).pack(anchor=tk.W)
        
        # Exchange and symbol
        market_frame = ttk.LabelFrame(left_frame, text="Market Selection", padding=5)
        market_frame.pack(fill=tk.X, pady=5)
        
        ttk.Label(market_frame, text="Exchange:").pack(anchor=tk.W)
        self.exchange_var = tk.StringVar(value=self.current_exchange)
        exchange_combo = ttk.Combobox(market_frame, textvariable=self.exchange_var, 
                                     values=["Binance", "Coinbase", "Deribit", "Delta Exchange", "MetaTrader 4", "MetaTrader 5"])
        exchange_combo.pack(fill=tk.X, pady=2)
        
        ttk.Label(market_frame, text="Symbol:").pack(anchor=tk.W)
        self.symbol_var = tk.StringVar(value=self.current_symbol)
        symbol_combo = ttk.Combobox(market_frame, textvariable=self.symbol_var, 
                                   values=["BTCUSD", "ETHUSD", "EURUSD", "GBPUSD", "XAUUSD", "USDJPY"])
        symbol_combo.pack(fill=tk.X, pady=2)
        
        # Strategy settings
        strategy_frame = ttk.LabelFrame(left_frame, text="Strategy Settings", padding=5)
        strategy_frame.pack(fill=tk.X, pady=5)
        
        ttk.Label(strategy_frame, text="Brick Size:").pack(anchor=tk.W)
        self.brick_size_var = tk.DoubleVar(value=10.0)
        brick_size_spin = ttk.Spinbox(strategy_frame, from_=1.0, to=100.0, textvariable=self.brick_size_var, increment=1.0)
        brick_size_spin.pack(fill=tk.X, pady=2)
        
        self.setup1_var = tk.BooleanVar(value=True)
        ttk.Checkbutton(strategy_frame, text="Setup 1 (Two Red + Green)", variable=self.setup1_var).pack(anchor=tk.W)
        
        self.setup2_var = tk.BooleanVar(value=True)
        ttk.Checkbutton(strategy_frame, text="Setup 2 (Three Brick Pattern)", variable=self.setup2_var).pack(anchor=tk.W)
        
        self.counter_trading_var = tk.BooleanVar(value=False)
        ttk.Checkbutton(strategy_frame, text="Counter Trading", variable=self.counter_trading_var).pack(anchor=tk.W)
        
        # Status panel
        status_frame = ttk.LabelFrame(left_frame, text="System Status", padding=5)
        status_frame.pack(fill=tk.X, pady=5)
        
        self.status_labels = {}
        status_items = [
            ("Connection:", "Disconnected"),
            ("Trading:", "Stopped"),
            ("Price:", f"${self.current_price:,.2f}"),
            ("Equity:", f"${self.current_equity:,.2f}"),
            ("Daily P&L:", "$0.00"),
            ("Open Positions:", "0"),
            ("Total Trades:", "0"),
            ("Win Rate:", "0.0%")
        ]
        
        for i, (label, value) in enumerate(status_items):
            ttk.Label(status_frame, text=label).grid(row=i, column=0, sticky=tk.W, pady=1)
            self.status_labels[label] = ttk.Label(status_frame, text=value)
            self.status_labels[label].grid(row=i, column=1, sticky=tk.W, padx=(10, 0), pady=1)
        
        # Risk management
        risk_frame = ttk.LabelFrame(left_frame, text="Risk Management", padding=5)
        risk_frame.pack(fill=tk.X, pady=5)
        
        ttk.Label(risk_frame, text="Risk per Trade (%):").pack(anchor=tk.W)
        self.risk_per_trade_var = tk.DoubleVar(value=2.0)
        risk_spin = ttk.Spinbox(risk_frame, from_=0.1, to=10.0, textvariable=self.risk_per_trade_var, increment=0.1)
        risk_spin.pack(fill=tk.X, pady=2)
        
        ttk.Label(risk_frame, text="Max Daily Risk (%):").pack(anchor=tk.W)
        self.max_daily_risk_var = tk.DoubleVar(value=10.0)
        daily_risk_spin = ttk.Spinbox(risk_frame, from_=1.0, to=50.0, textvariable=self.max_daily_risk_var, increment=1.0)
        daily_risk_spin.pack(fill=tk.X, pady=2)
        
        ttk.Label(risk_frame, text="Risk Usage:").pack(anchor=tk.W)
        self.risk_progress = ttk.Progressbar(risk_frame, mode='determinate')
        self.risk_progress.pack(fill=tk.X, pady=2)
        
        # Right panel with tabs
        right_frame = ttk.Frame(main_frame)
        right_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)
        
        self.notebook = ttk.Notebook(right_frame)
        self.notebook.pack(fill=tk.BOTH, expand=True)
        
        # Dashboard tab
        self.dashboard_frame = ttk.Frame(self.notebook)
        self.notebook.add(self.dashboard_frame, text="Dashboard")
        
        # Positions tab
        self.positions_frame = ttk.Frame(self.notebook)
        self.notebook.add(self.positions_frame, text="Positions")
        
        # Setup positions table
        self.setup_positions_table()
        
        # Logs tab
        self.logs_frame = ttk.Frame(self.notebook)
        self.notebook.add(self.logs_frame, text="Logs")
        
        # Setup logs
        self.setup_logs()
        
    def setup_chart(self):
        """Setup price chart"""
        # Create matplotlib figure
        self.fig = Figure(figsize=(8, 6), dpi=100)
        self.ax = self.fig.add_subplot(111)
        
        # Setup chart
        self.ax.set_title(f"{self.current_symbol} - {self.current_exchange}")
        self.ax.set_xlabel("Time")
        self.ax.set_ylabel("Price ($)")
        self.ax.grid(True, alpha=0.3)
        
        # Create canvas
        self.canvas = FigureCanvasTkAgg(self.fig, self.dashboard_frame)
        self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)
        
        # Initialize empty line
        self.price_line, = self.ax.plot([], [], 'b-', linewidth=2)
        self.ax.set_ylim(self.current_price - 500, self.current_price + 500)
        
    def setup_positions_table(self):
        """Setup positions table"""
        # Create treeview
        columns = ("Time", "Symbol", "Side", "Size", "Entry Price", "Current Price", "P&L", "Status")
        self.positions_tree = ttk.Treeview(self.positions_frame, columns=columns, show='headings')
        
        # Configure columns
        for col in columns:
            self.positions_tree.heading(col, text=col)
            self.positions_tree.column(col, width=100)
        
        # Add scrollbar
        scrollbar = ttk.Scrollbar(self.positions_frame, orient=tk.VERTICAL, command=self.positions_tree.yview)
        self.positions_tree.configure(yscrollcommand=scrollbar.set)
        
        # Pack
        self.positions_tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
    def setup_logs(self):
        """Setup logs display"""
        self.log_text = tk.Text(self.logs_frame, height=20, width=80)
        log_scrollbar = ttk.Scrollbar(self.logs_frame, orient=tk.VERTICAL, command=self.log_text.yview)
        self.log_text.configure(yscrollcommand=log_scrollbar.set)
        
        self.log_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        log_scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # Add initial messages
        self.log_message("MasterMind Trader Python v1.0.0 initialized")
        self.log_message("Paper trading mode enabled for safety")
        self.log_message("Ready to trade - click 'Start Trading' to begin")
        
    def log_message(self, message: str):
        """Add message to log"""
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        full_message = f"[{timestamp}] {message}\n"
        self.log_text.insert(tk.END, full_message)
        self.log_text.see(tk.END)
        
    def update_trading_mode(self):
        """Update trading mode"""
        if self.trading_mode.get() == "live":
            result = messagebox.askyesno(
                "Switch to Live Trading",
                "⚠️ WARNING: You are about to switch to LIVE TRADING mode!\n\n"
                "This will use REAL MONEY and can result in ACTUAL LOSSES.\n"
                "Are you absolutely sure you want to proceed?",
                icon='warning'
            )
            if result:
                self.paper_trading = False
                self.log_message("⚠️ WARNING: Switched to LIVE trading mode")
                self.log_message("Real money at risk - ensure proper risk management")
            else:
                self.trading_mode.set("paper")
                self.paper_trading = True
        else:
            self.paper_trading = True
            self.log_message("Switched to paper trading mode")
        
    def start_trading(self):
        """Start trading"""
        if self.trading_active:
            return
        
        self.trading_active = True
        self.stop_trading_event.clear()
        
        # Update UI
        self.start_button.config(state=tk.DISABLED)
        self.stop_button.config(state=tk.NORMAL)
        self.status_labels["Connection:"].config(text="Connected")
        self.status_labels["Trading:"].config(text="Running")
        
        # Update settings
        self.current_symbol = self.symbol_var.get()
        self.current_exchange = self.exchange_var.get()
        self.renko_engine.brick_size = self.brick_size_var.get()
        self.risk_manager.max_risk_per_trade = self.risk_per_trade_var.get()
        self.risk_manager.max_daily_risk = self.max_daily_risk_var.get()
        
        # Start trading thread
        self.trading_thread = threading.Thread(target=self.trading_loop)
        self.trading_thread.daemon = True
        self.trading_thread.start()
        
        self.log_message(f"Trading started on {self.current_exchange} for {self.current_symbol}")
        if self.paper_trading:
            self.log_message("Paper trading mode - trades are simulated")
        
    def stop_trading(self):
        """Stop trading"""
        if not self.trading_active:
            return
        
        self.trading_active = False
        self.stop_trading_event.set()
        
        # Update UI
        self.start_button.config(state=tk.NORMAL)
        self.stop_button.config(state=tk.DISABLED)
        self.status_labels["Connection:"].config(text="Disconnected")
        self.status_labels["Trading:"].config(text="Stopped")
        
        # Close all open positions
        for trade in self.open_trades:
            trade.close()
            self.closed_trades.append(trade)
        self.open_trades.clear()
        
        win_rate = (self.winning_trades / self.total_trades * 100) if self.total_trades > 0 else 0
        self.log_message("Trading stopped")
        self.log_message(f"Session summary: {self.total_trades} trades, ${self.daily_pnl:.2f} P&L, {win_rate:.1f}% win rate")
        
    def trading_loop(self):
        """Main trading loop"""
        while self.trading_active and not self.stop_trading_event.is_set():
            try:
                # Simulate price movement
                self.simulate_price_movement()
                
                # Check for trading signals
                self.check_trading_signals()
                
                # Update open positions
                self.update_positions()
                
                # Update UI
                self.message_queue.put(("update_ui", {}))
                
                # Sleep
                time.sleep(1)
                
            except Exception as e:
                self.log_message(f"Error in trading loop: {e}")
                break
                
    def simulate_price_movement(self):
        """Simulate realistic price movement"""
        # Random price change
        change = random.uniform(-1, 1) * self.price_volatility * self.current_price
        
        # Add trend
        if random.random() < 0.02:  # 2% chance to change trend
            self.price_direction = 1 if random.random() > 0.5 else -1
        
        change += self.price_direction * self.price_volatility * self.current_price * 0.1
        
        self.current_price += change
        
        # Ensure price stays positive
        if self.current_price < 100:
            self.current_price = 100 + random.uniform(0, 1000)
        
        # Add to history
        self.price_history.append(self.current_price)
        
        # Update Renko engine
        new_bricks = self.renko_engine.add_price(self.current_price)
        if new_bricks:
            self.message_queue.put(("log", f"New Renko bricks: {', '.join(new_bricks)}"))
        
    def check_trading_signals(self):
        """Check for trading signals"""
        if not self.setup1_var.get() and not self.setup2_var.get():
            return
        
        signal = None
        
        if self.setup1_var.get() and self.renko_engine.check_setup1():
            signal = "BUY"
            self.message_queue.put(("log", "Setup 1 signal detected: Two red bricks + green brick"))
        
        if self.setup2_var.get() and self.renko_engine.check_setup2():
            signal = "SELL" if not signal else signal  # Don't override BUY signal
            self.message_queue.put(("log", "Setup 2 signal detected: Three brick pattern"))
        
        if signal and random.random() < 0.1:  # 10% chance to execute
            self.execute_trade(signal)
            
    def execute_trade(self, side: str):
        """Execute a trade"""
        if len(self.open_trades) >= 5:  # Max 5 open positions
            return
        
        # Calculate position size
        risk_amount = self.current_equity * (self.risk_per_trade_var.get() / 100)
        position_size = risk_amount / (self.current_price * 0.01)  # 1% stop loss
        
        # Check risk management
        if not self.risk_manager.can_trade(self.risk_per_trade_var.get(), len(self.open_trades)):
            self.message_queue.put(("log", "Trade rejected: Risk limits exceeded"))
            return
        
        # Create trade
        trade = Trade(
            symbol=self.current_symbol,
            side=side,
            size=position_size,
            entry_price=self.current_price + random.uniform(-2, 2)  # Simulate spread
        )
        
        self.open_trades.append(trade)
        self.total_trades += 1
        
        # Add to risk usage
        self.risk_manager.add_risk(self.risk_per_trade_var.get())
        
        self.message_queue.put(("log", f"Trade #{self.total_trades}: {side} {position_size:.3f} @ ${trade.entry_price:.2f}"))
        self.message_queue.put(("add_trade", trade))
        
    def update_positions(self):
        """Update open positions"""
        for trade in self.open_trades[:]:  # Copy list to avoid modification during iteration
            trade.update_price(self.current_price)
            
            # Simple exit logic (10% chance to close position)
            if random.random() < 0.05:
                trade.close()
                self.closed_trades.append(trade)
                self.open_trades.remove(trade)
                
                # Update P&L
                self.daily_pnl += trade.pnl
                self.current_equity += trade.pnl
                
                # Update statistics
                if trade.pnl > 0:
                    self.winning_trades += 1
                
                self.message_queue.put(("log", f"Position closed: {trade.pnl:+.2f} P&L"))
                self.message_queue.put(("update_trade", trade))
                
    def process_messages(self):
        """Process messages from trading thread"""
        try:
            while True:
                message_type, data = self.message_queue.get_nowait()
                
                if message_type == "update_ui":
                    self.update_status_labels()
                    self.update_chart()
                elif message_type == "log":
                    self.log_message(data)
                elif message_type == "add_trade":
                    self.add_trade_to_table(data)
                elif message_type == "update_trade":
                    self.update_trade_in_table(data)
                    
        except queue.Empty:
            pass
        
        # Schedule next check
        self.root.after(100, self.process_messages)
        
    def update_status_labels(self):
        """Update status labels"""
        self.status_labels["Price:"].config(text=f"${self.current_price:,.2f}")
        self.status_labels["Equity:"].config(text=f"${self.current_equity:,.2f}")
        
        pnl_text = f"${self.daily_pnl:+,.2f}"
        self.status_labels["Daily P&L:"].config(text=pnl_text)
        
        self.status_labels["Open Positions:"].config(text=str(len(self.open_trades)))
        self.status_labels["Total Trades:"].config(text=str(self.total_trades))
        
        win_rate = (self.winning_trades / self.total_trades * 100) if self.total_trades > 0 else 0
        self.status_labels["Win Rate:"].config(text=f"{win_rate:.1f}%")
        
        # Update risk progress
        risk_percentage = (self.risk_manager.daily_risk_used / self.risk_manager.max_daily_risk) * 100
        self.risk_progress['value'] = min(risk_percentage, 100)
        
    def update_chart(self):
        """Update price chart"""
        if len(self.price_history) < 2:
            return
        
        # Update line data
        x_data = list(range(len(self.price_history)))
        y_data = list(self.price_history)
        
        self.price_line.set_data(x_data, y_data)
        
        # Update axes
        self.ax.set_xlim(0, len(self.price_history))
        self.ax.set_ylim(min(y_data) - 50, max(y_data) + 50)
        
        # Refresh canvas
        self.canvas.draw()
        
    def add_trade_to_table(self, trade: Trade):
        """Add trade to positions table"""
        self.positions_tree.insert('', 'end', values=(
            trade.timestamp.strftime("%H:%M:%S"),
            trade.symbol,
            trade.side,
            f"{trade.size:.3f}",
            f"${trade.entry_price:.2f}",
            f"${trade.current_price:.2f}",
            f"${trade.pnl:+.2f}",
            trade.status
        ))
        
    def update_trade_in_table(self, trade: Trade):
        """Update trade in positions table"""
        # Find and update the trade in the table
        for child in self.positions_tree.get_children():
            values = self.positions_tree.item(child)['values']
            if (values[0] == trade.timestamp.strftime("%H:%M:%S") and 
                values[1] == trade.symbol and 
                values[2] == trade.side):
                
                self.positions_tree.item(child, values=(
                    trade.timestamp.strftime("%H:%M:%S"),
                    trade.symbol,
                    trade.side,
                    f"{trade.size:.3f}",
                    f"${trade.entry_price:.2f}",
                    f"${trade.current_price:.2f}",
                    f"${trade.pnl:+.2f}",
                    trade.status
                ))
                break
    
    def load_config(self):
        """Load configuration from file"""
        try:
            with open('config.json', 'r') as f:
                config = json.load(f)
                
            # Load settings
            trading = config.get('trading', {})
            self.current_exchange = trading.get('defaultExchange', 'Binance')
            self.current_symbol = trading.get('defaultSymbol', 'BTCUSD')
            self.paper_trading = trading.get('paperTradingMode', True)
            
            # Update UI
            self.exchange_var.set(self.current_exchange)
            self.symbol_var.set(self.current_symbol)
            self.trading_mode.set('paper' if self.paper_trading else 'live')
            
            self.log_message("Configuration loaded successfully")
            
        except FileNotFoundError:
            self.log_message("No configuration file found, using defaults")
        except Exception as e:
            self.log_message(f"Error loading configuration: {e}")
            
    def save_config(self):
        """Save configuration to file"""
        try:
            config = {
                'trading': {
                    'defaultExchange': self.exchange_var.get(),
                    'defaultSymbol': self.symbol_var.get(),
                    'paperTradingMode': self.paper_trading
                },
                'strategy': {
                    'brickSize': self.brick_size_var.get(),
                    'setup1Enabled': self.setup1_var.get(),
                    'setup2Enabled': self.setup2_var.get(),
                    'counterTradingEnabled': self.counter_trading_var.get()
                },
                'risk': {
                    'maxRiskPerTrade': self.risk_per_trade_var.get(),
                    'maxDailyRisk': self.max_daily_risk_var.get()
                }
            }
            
            with open('config.json', 'w') as f:
                json.dump(config, f, indent=2)
                
            self.log_message("Configuration saved successfully")
            
        except Exception as e:
            self.log_message(f"Error saving configuration: {e}")
            
    def on_closing(self):
        """Handle application closing"""
        if self.trading_active:
            result = messagebox.askyesno(
                "Exit Application",
                "Trading is currently active. Are you sure you want to exit?",
                icon='warning'
            )
            if not result:
                return
            
            self.stop_trading()
            time.sleep(0.5)  # Give time for thread to stop
        
        self.save_config()
        self.root.destroy()
        
    def run(self):
        """Run the application"""
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
        self.root.mainloop()

def main():
    """Main function"""
    print("Starting MasterMind Trader Python Version...")
    print("=" * 50)
    
    app = MasterMindTrader()
    app.run()

if __name__ == "__main__":
    main() 