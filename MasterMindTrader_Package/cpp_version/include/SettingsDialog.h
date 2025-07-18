#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QGroupBox>

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog() = default;
    
    void loadSettings();
    void saveSettings();
    
private slots:
    void onAccepted();
    void onRejected();
    void onApplyClicked();
    void onResetClicked();
    
private:
    void setupUI();
    void setupTradingTab();
    void setupRiskTab();
    void setupExchangeTab();
    void setupUITab();
    
    QVBoxLayout *m_mainLayout;
    QTabWidget *m_tabWidget;
    QDialogButtonBox *m_buttonBox;
    
    // Trading settings
    QWidget *m_tradingTab;
    QDoubleSpinBox *m_brickSizeSpinBox;
    QSpinBox *m_tickBufferSpinBox;
    QCheckBox *m_setup1CheckBox;
    QCheckBox *m_setup2CheckBox;
    QCheckBox *m_counterTradingCheckBox;
    QSpinBox *m_tradesPerCounterSpinBox;
    
    // Risk settings
    QWidget *m_riskTab;
    QDoubleSpinBox *m_maxRiskPerTradeSpinBox;
    QDoubleSpinBox *m_maxDailyRiskSpinBox;
    QSpinBox *m_maxPositionsSpinBox;
    QSpinBox *m_maxTradesPerDaySpinBox;
    QDoubleSpinBox *m_maxDrawdownSpinBox;
    
    // Exchange settings
    QWidget *m_exchangeTab;
    QLineEdit *m_apiKeyLineEdit;
    QLineEdit *m_apiSecretLineEdit;
    QLineEdit *m_passphraseLineEdit;
    QCheckBox *m_testModeCheckBox;
    
    // UI settings
    QWidget *m_uiTab;
    QComboBox *m_themeComboBox;
    QCheckBox *m_enableSoundCheckBox;
    QCheckBox *m_enableNotificationsCheckBox;
    QSpinBox *m_updateIntervalSpinBox;
};

#endif // SETTINGSDIALOG_H 