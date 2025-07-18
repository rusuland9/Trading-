#include "PositionWidget.h"

PositionWidget::PositionWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void PositionWidget::setupUI()
{
    m_layout = new QVBoxLayout(this);
    
    m_groupBox = new QGroupBox("Open Positions");
    m_layout->addWidget(m_groupBox);
    
    QVBoxLayout *groupLayout = new QVBoxLayout(m_groupBox);
    
    // Create positions table
    m_positionsTable = new QTableWidget(0, 5);
    m_positionsTable->setHorizontalHeaderLabels({"Symbol", "Side", "Size", "Price", "P&L"});
    m_positionsTable->horizontalHeader()->setStretchLastSection(true);
    m_positionsTable->setAlternatingRowColors(true);
    groupLayout->addWidget(m_positionsTable);
    
    // Total P&L label
    m_totalPnLLabel = new QLabel("Total P&L: $0.00");
    m_totalPnLLabel->setStyleSheet("font-weight: bold; color: #42A5F5;");
    groupLayout->addWidget(m_totalPnLLabel);
    
    // Close all button
    m_closeAllButton = new QPushButton("Close All Positions");
    m_closeAllButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; font-weight: bold; padding: 8px; border-radius: 4px; }");
    m_closeAllButton->setEnabled(false);
    groupLayout->addWidget(m_closeAllButton);
}

void PositionWidget::updatePositions()
{
    // Stub implementation
}

void PositionWidget::addPosition(const QString &symbol, const QString &side, double size, double price)
{
    int row = m_positionsTable->rowCount();
    m_positionsTable->insertRow(row);
    
    m_positionsTable->setItem(row, 0, new QTableWidgetItem(symbol));
    m_positionsTable->setItem(row, 1, new QTableWidgetItem(side));
    m_positionsTable->setItem(row, 2, new QTableWidgetItem(QString::number(size)));
    m_positionsTable->setItem(row, 3, new QTableWidgetItem(QString::number(price)));
    m_positionsTable->setItem(row, 4, new QTableWidgetItem("$0.00"));
    
    m_closeAllButton->setEnabled(true);
}

void PositionWidget::removePosition(const QString &symbol)
{
    Q_UNUSED(symbol)
    // Stub implementation
} 