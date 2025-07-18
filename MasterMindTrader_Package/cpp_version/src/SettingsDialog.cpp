#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle("Settings");
    setModal(true);
    resize(600, 400);
}

void SettingsDialog::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    m_tabWidget = new QTabWidget;
    m_mainLayout->addWidget(m_tabWidget);
    
    setupTradingTab();
    setupRiskTab();
    setupExchangeTab();
    setupUITab();
    
    // Dialog buttons
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply | QDialogButtonBox::Reset);
    m_mainLayout->addWidget(m_buttonBox);
    
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onAccepted);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::onRejected);
    
    QPushButton *applyButton = m_buttonBox->button(QDialogButtonBox::Apply);
    if (applyButton) {
        connect(applyButton, &QPushButton::clicked, this, &SettingsDialog::onApplyClicked);
    }
    
    QPushButton *resetButton = m_buttonBox->button(QDialogButtonBox::Reset);
    if (resetButton) {
        connect(resetButton, &QPushButton::clicked, this, &SettingsDialog::onResetClicked);
    }
}

void SettingsDialog::setupTradingTab()
{
    m_tradingTab = new QWidget;
    QFormLayout *layout = new QFormLayout(m_tradingTab);
    
    // Strategy settings
    QGroupBox *strategyGroup = new QGroupBox("Strategy Settings");
    QFormLayout *strategyLayout = new QFormLayout(strategyGroup);
    
    m_brickSizeSpinBox = new QDoubleSpinBox;
    m_brickSizeSpinBox->setRange(0.1, 1000.0);
    m_brickSizeSpinBox->setValue(10.0);
    m_brickSizeSpinBox->setSuffix(" pips");
    strategyLayout->addRow("Brick Size:", m_brickSizeSpinBox);
    
    m_tickBufferSpinBox = new QSpinBox;
    m_tickBufferSpinBox->setRange(0, 10);
    m_tickBufferSpinBox->setValue(2);
    strategyLayout->addRow("Tick Buffer:", m_tickBufferSpinBox);
    
    m_setup1CheckBox = new QCheckBox("Enable Setup 1 (Two Red + Green)");
    m_setup1CheckBox->setChecked(true);
    strategyLayout->addRow(m_setup1CheckBox);
    
    m_setup2CheckBox = new QCheckBox("Enable Setup 2 (Three Brick Pattern)");
    m_setup2CheckBox->setChecked(true);
    strategyLayout->addRow(m_setup2CheckBox);
    
    layout->addRow(strategyGroup);
    
    // Counter trading
    QGroupBox *counterGroup = new QGroupBox("Counter Trading");
    QFormLayout *counterLayout = new QFormLayout(counterGroup);
    
    m_counterTradingCheckBox = new QCheckBox("Enable Counter Trading");
    counterLayout->addRow(m_counterTradingCheckBox);
    
    m_tradesPerCounterSpinBox = new QSpinBox;
    m_tradesPerCounterSpinBox->setRange(5, 50);
    m_tradesPerCounterSpinBox->setValue(10);
    counterLayout->addRow("Trades per Counter:", m_tradesPerCounterSpinBox);
    
    layout->addRow(counterGroup);
    
    m_tabWidget->addTab(m_tradingTab, "Trading");
}

void SettingsDialog::setupRiskTab()
{
    m_riskTab = new QWidget;
    QFormLayout *layout = new QFormLayout(m_riskTab);
    
    QGroupBox *riskGroup = new QGroupBox("Risk Parameters");
    QFormLayout *riskLayout = new QFormLayout(riskGroup);
    
    m_maxRiskPerTradeSpinBox = new QDoubleSpinBox;
    m_maxRiskPerTradeSpinBox->setRange(0.1, 10.0);
    m_maxRiskPerTradeSpinBox->setValue(2.0);
    m_maxRiskPerTradeSpinBox->setSuffix("%");
    riskLayout->addRow("Max Risk per Trade:", m_maxRiskPerTradeSpinBox);
    
    m_maxDailyRiskSpinBox = new QDoubleSpinBox;
    m_maxDailyRiskSpinBox->setRange(1.0, 50.0);
    m_maxDailyRiskSpinBox->setValue(10.0);
    m_maxDailyRiskSpinBox->setSuffix("%");
    riskLayout->addRow("Max Daily Risk:", m_maxDailyRiskSpinBox);
    
    m_maxPositionsSpinBox = new QSpinBox;
    m_maxPositionsSpinBox->setRange(1, 20);
    m_maxPositionsSpinBox->setValue(5);
    riskLayout->addRow("Max Open Positions:", m_maxPositionsSpinBox);
    
    m_maxTradesPerDaySpinBox = new QSpinBox;
    m_maxTradesPerDaySpinBox->setRange(1, 100);
    m_maxTradesPerDaySpinBox->setValue(20);
    riskLayout->addRow("Max Trades per Day:", m_maxTradesPerDaySpinBox);
    
    m_maxDrawdownSpinBox = new QDoubleSpinBox;
    m_maxDrawdownSpinBox->setRange(5.0, 50.0);
    m_maxDrawdownSpinBox->setValue(20.0);
    m_maxDrawdownSpinBox->setSuffix("%");
    riskLayout->addRow("Max Drawdown:", m_maxDrawdownSpinBox);
    
    layout->addRow(riskGroup);
    
    m_tabWidget->addTab(m_riskTab, "Risk");
}

void SettingsDialog::setupExchangeTab()
{
    m_exchangeTab = new QWidget;
    QFormLayout *layout = new QFormLayout(m_exchangeTab);
    
    QGroupBox *apiGroup = new QGroupBox("API Settings");
    QFormLayout *apiLayout = new QFormLayout(apiGroup);
    
    m_apiKeyLineEdit = new QLineEdit;
    m_apiKeyLineEdit->setEchoMode(QLineEdit::Password);
    apiLayout->addRow("API Key:", m_apiKeyLineEdit);
    
    m_apiSecretLineEdit = new QLineEdit;
    m_apiSecretLineEdit->setEchoMode(QLineEdit::Password);
    apiLayout->addRow("API Secret:", m_apiSecretLineEdit);
    
    m_passphraseLineEdit = new QLineEdit;
    m_passphraseLineEdit->setEchoMode(QLineEdit::Password);
    apiLayout->addRow("Passphrase:", m_passphraseLineEdit);
    
    m_testModeCheckBox = new QCheckBox("Enable Test Mode");
    m_testModeCheckBox->setChecked(true);
    apiLayout->addRow(m_testModeCheckBox);
    
    layout->addRow(apiGroup);
    
    m_tabWidget->addTab(m_exchangeTab, "Exchange");
}

void SettingsDialog::setupUITab()
{
    m_uiTab = new QWidget;
    QFormLayout *layout = new QFormLayout(m_uiTab);
    
    QGroupBox *uiGroup = new QGroupBox("UI Settings");
    QFormLayout *uiLayout = new QFormLayout(uiGroup);
    
    m_themeComboBox = new QComboBox;
    m_themeComboBox->addItems({"Dark", "Light"});
    uiLayout->addRow("Theme:", m_themeComboBox);
    
    m_enableSoundCheckBox = new QCheckBox("Enable Sound Notifications");
    uiLayout->addRow(m_enableSoundCheckBox);
    
    m_enableNotificationsCheckBox = new QCheckBox("Enable System Notifications");
    uiLayout->addRow(m_enableNotificationsCheckBox);
    
    m_updateIntervalSpinBox = new QSpinBox;
    m_updateIntervalSpinBox->setRange(100, 10000);
    m_updateIntervalSpinBox->setValue(1000);
    m_updateIntervalSpinBox->setSuffix(" ms");
    uiLayout->addRow("Update Interval:", m_updateIntervalSpinBox);
    
    layout->addRow(uiGroup);
    
    m_tabWidget->addTab(m_uiTab, "UI");
}

void SettingsDialog::loadSettings()
{
    // Stub implementation
}

void SettingsDialog::saveSettings()
{
    // Stub implementation
}

void SettingsDialog::onAccepted()
{
    saveSettings();
    accept();
}

void SettingsDialog::onRejected()
{
    reject();
}

void SettingsDialog::onApplyClicked()
{
    saveSettings();
}

void SettingsDialog::onResetClicked()
{
    // Reset to defaults
    m_brickSizeSpinBox->setValue(10.0);
    m_tickBufferSpinBox->setValue(2);
    m_setup1CheckBox->setChecked(true);
    m_setup2CheckBox->setChecked(true);
    m_counterTradingCheckBox->setChecked(false);
    m_tradesPerCounterSpinBox->setValue(10);
    
    m_maxRiskPerTradeSpinBox->setValue(2.0);
    m_maxDailyRiskSpinBox->setValue(10.0);
    m_maxPositionsSpinBox->setValue(5);
    m_maxTradesPerDaySpinBox->setValue(20);
    m_maxDrawdownSpinBox->setValue(20.0);
    
    m_apiKeyLineEdit->clear();
    m_apiSecretLineEdit->clear();
    m_passphraseLineEdit->clear();
    m_testModeCheckBox->setChecked(true);
    
    m_themeComboBox->setCurrentIndex(0);
    m_enableSoundCheckBox->setChecked(false);
    m_enableNotificationsCheckBox->setChecked(false);
    m_updateIntervalSpinBox->setValue(1000);
} 