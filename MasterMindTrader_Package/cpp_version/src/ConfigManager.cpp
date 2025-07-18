#include "ConfigManager.h"
#include <QFile>
#include <QJsonParseError>

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
{
}

void ConfigManager::loadConfig(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error == QJsonParseError::NoError) {
        m_config = doc.object();
    }
}

void ConfigManager::saveConfig(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }
    
    QJsonDocument doc(m_config);
    file.write(doc.toJson());
    file.close();
} 