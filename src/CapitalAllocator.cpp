#include "CapitalAllocator.h"

CapitalAllocator::CapitalAllocator(QObject *parent)
    : QObject(parent)
{
}

void CapitalAllocator::setCapitalForSymbol(const QString &symbol, double capital)
{
    QMutexLocker locker(&m_mutex);
    m_capitalAllocation[symbol] = capital;
}

double CapitalAllocator::getCapitalForSymbol(const QString &symbol) const
{
    QMutexLocker locker(&m_mutex);
    auto it = m_capitalAllocation.find(symbol);
    return it != m_capitalAllocation.end() ? it->second : 0.0;
}

double CapitalAllocator::getTotalCapital() const
{
    QMutexLocker locker(&m_mutex);
    double total = 0.0;
    for (const auto &pair : m_capitalAllocation) {
        total += pair.second;
    }
    return total;
} 