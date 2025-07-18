# MasterMind Trader - New PC Setup Guide

## 🚀 **Quick Setup (5 Minutes)**

### **Method 1: Automated Setup (Recommended)**
1. **Copy the `python_version` folder** to your new PC
2. **Run the setup script**: Double-click `setup_new_pc.bat`
3. **Done!** The application will start automatically

### **Method 2: Manual Setup**
1. **Install Python** (if not already installed)
2. **Install dependencies**
3. **Run the application**

---

## 📋 **Detailed Instructions**

### **Step 1: Install Python**
1. Go to **https://www.python.org/downloads/**
2. Download **Python 3.8 or newer**
3. **⚠️ IMPORTANT**: During installation, check "Add Python to PATH"
4. Click "Install Now"

### **Step 2: Copy Project Files**
Copy the entire `python_version` folder to your new PC. It should contain:
```
📁 python_version/
├── mastermind_trader.py      # Main application
├── requirements.txt          # Dependencies list
├── run_trader.bat           # Quick run script
├── setup_new_pc.bat         # New PC setup script
├── config.json              # Configuration file
├── README.md                # Documentation
└── SETUP_NEW_PC.md          # This file
```

### **Step 3: Install Dependencies**
Open **Command Prompt** and run:
```bash
pip install matplotlib numpy
```

### **Step 4: Run the Application**
Navigate to the project folder and run:
```bash
cd python_version
python mastermind_trader.py
```

---

## 🛠️ **Troubleshooting**

### **Problem: "Python is not recognized"**
**Solution:**
1. Reinstall Python with "Add to PATH" checked
2. Or manually add Python to PATH in Environment Variables

### **Problem: "No module named 'tkinter'"**
**Solution:**
1. Tkinter comes with Python by default
2. If missing, reinstall Python from python.org (not Microsoft Store)

### **Problem: "No module named 'matplotlib'"**
**Solution:**
```bash
pip install matplotlib numpy
```

### **Problem: Interface doesn't show**
**Solution:**
1. Check if Python is 32-bit or 64-bit compatible
2. Try running from Command Prompt to see error messages
3. Check if antivirus is blocking the application

---

## 🎯 **System Requirements**

### **Minimum Requirements:**
- **OS**: Windows 7/8/10/11
- **Python**: 3.7 or newer
- **RAM**: 2GB
- **Storage**: 100MB free space

### **Recommended:**
- **OS**: Windows 10/11
- **Python**: 3.9 or newer
- **RAM**: 4GB+
- **Storage**: 500MB free space

---

## 🎨 **What You'll See**

When successful, you'll see a professional trading interface with:

### **Left Panel:**
- Start/Stop Trading buttons
- Trading mode selection (Paper/Live)
- Market selection (Exchange & Symbol)
- Strategy settings (Renko patterns)
- System status display
- Risk management controls

### **Right Panel:**
- **Dashboard**: Live price chart with real-time updates
- **Positions**: Trade tracking table
- **Logs**: Real-time trading activity

---

## 🔧 **Alternative Run Methods**

### **Method 1: Double-click the file**
1. Navigate to `python_version` folder
2. Double-click `mastermind_trader.py`
3. Choose "Open with Python"

### **Method 2: Command Prompt**
1. Open Command Prompt
2. Navigate to folder: `cd path\to\python_version`
3. Run: `python mastermind_trader.py`

### **Method 3: Use the batch file**
1. Double-click `run_trader.bat`
2. The application will start automatically

---

## 🎉 **Success Indicators**

You'll know it's working when:
- ✅ A window opens with "MasterMind Trader - Python Version" title
- ✅ You see a professional dark-themed interface
- ✅ The left panel shows trading controls
- ✅ The right panel shows tabs for Dashboard, Positions, Logs
- ✅ You can click "Start Trading" and see live price simulation

---

## 🔍 **Quick Test**

To verify everything works:
1. **Start the application**
2. **Click "Start Trading"**
3. **Go to Dashboard tab** - you should see a live price chart
4. **Go to Logs tab** - you should see real-time messages
5. **Go to Positions tab** - you should see trades appearing

---

## 📞 **Support**

If you encounter issues:
1. **Check Python version**: `python --version`
2. **Check dependencies**: `pip list`
3. **Run with errors visible**: `python mastermind_trader.py` in Command Prompt
4. **Try the automated setup**: `setup_new_pc.bat`

---

## 🎯 **Summary**

**For New PC Setup:**
1. **Copy** the `python_version` folder
2. **Install** Python 3.8+ (with PATH)
3. **Run** `setup_new_pc.bat`
4. **Enjoy** professional trading simulation!

**That's it!** The application is completely self-contained and ready to run. 