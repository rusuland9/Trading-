#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include <QFont>
#include <QFontDatabase>
#include <QSplashScreen>
#include <QPixmap>
#include <QTimer>
#include <QScreen>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("MasterMind Trader");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("MasterMind Trading Systems");
    app.setOrganizationDomain("mastermindtrader.com");
    
    // Enable high DPI support
    app.setAttribute(Qt::AA_EnableHighDpiScaling);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    
    // Set modern dark theme
    app.setStyle(QStyleFactory::create("Fusion"));
    
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    app.setPalette(darkPalette);
    
    // Set application font
    QFont font("Segoe UI", 9);
    app.setFont(font);
    
    // Create splash screen
    QPixmap splashPixmap(400, 300);
    splashPixmap.fill(QColor(25, 25, 25));
    QSplashScreen splash(splashPixmap, Qt::WindowStaysOnTopHint);
    splash.setFont(QFont("Segoe UI", 12, QFont::Bold));
    
    splash.show();
    app.processEvents();
    
    splash.showMessage("Loading MasterMind Trader...", Qt::AlignCenter | Qt::AlignBottom, Qt::white);
    QTimer::singleShot(1000, [&]() {
        splash.showMessage("Initializing Trading Engine...", Qt::AlignCenter | Qt::AlignBottom, Qt::white);
    });
    
    QTimer::singleShot(2000, [&]() {
        splash.showMessage("Connecting to Exchanges...", Qt::AlignCenter | Qt::AlignBottom, Qt::white);
    });
    
    QTimer::singleShot(3000, [&]() {
        splash.showMessage("Loading Configuration...", Qt::AlignCenter | Qt::AlignBottom, Qt::white);
    });
    
    // Create main window
    MainWindow window;
    
    // Show main window after splash
    QTimer::singleShot(4000, [&]() {
        splash.finish(&window);
        window.show();
    });
    
    return app.exec();
} 