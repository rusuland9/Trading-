cmake_minimum_required(VERSION 3.16)
project(MasterMindTrader VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 - fallback to Qt5 if not found
find_package(Qt6 QUIET COMPONENTS Core Widgets Charts Network Sql WebSockets)
if(NOT Qt6_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets Charts Network Sql WebSockets)
    set(QT_VERSION_MAJOR 5)
else()
    set(QT_VERSION_MAJOR 6)
endif()

# Qt settings
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Include directories
include_directories(include)

# Create directories if they don't exist
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/logs)

# Header files
set(HEADERS
    include/MainWindow.h
    include/StrategyEngine.h
    include/RiskManager.h
    include/CapitalAllocator.h
    include/OrderManager.h
    include/ExchangeConnector.h
    include/TradeSessionManager.h
    include/Logger.h
    include/ConfigManager.h
    include/PaperTradeFallback.h
    include/TradingDashboard.h
    include/PositionWidget.h
    include/ChartWidget.h
    include/SettingsDialog.h
)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/StrategyEngine.cpp
    src/RiskManager.cpp
    src/CapitalAllocator.cpp
    src/OrderManager.cpp
    src/ExchangeConnector.cpp
    src/TradeSessionManager.cpp
    src/Logger.cpp
    src/ConfigManager.cpp
    src/PaperTradeFallback.cpp
    src/TradingDashboard.cpp
    src/PositionWidget.cpp
    src/ChartWidget.cpp
    src/SettingsDialog.cpp
)

# Create executable
add_executable(MasterMindTrader ${SOURCES} ${HEADERS})

# Link Qt libraries
if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(MasterMindTrader
        Qt6::Core
        Qt6::Widgets
        Qt6::Charts
        Qt6::Network
        Qt6::Sql
        Qt6::WebSockets
    )
else()
    target_link_libraries(MasterMindTrader
        Qt5::Core
        Qt5::Widgets
        Qt5::Charts
        Qt5::Network
        Qt5::Sql
        Qt5::WebSockets
    )
endif()

# Set target properties
set_target_properties(MasterMindTrader PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Install rules
install(TARGETS MasterMindTrader
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
) 