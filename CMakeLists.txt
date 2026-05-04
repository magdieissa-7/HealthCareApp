cmake_minimum_required(VERSION 3.16)
project(HealthCareApp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)
qt_standard_project_setup()


find_package(Boost REQUIRED)


find_package(OpenSSL REQUIRED)


find_package(SQLite3 REQUIRED)


find_package(GTest REQUIRED)


set(SOURCES
    src/main.cpp
    src/gui/LoginWindow.cpp
    src/gui/DashboardWindow.cpp
    src/gui/MedicationForm.cpp
    src/gui/EmergencyWindow.cpp
    src/gui/AIChatWindow.cpp
    src/core/User.cpp
    src/core/Medication.cpp
    src/core/ReminderManager.cpp
    src/core/EmergencyManager.cpp
    src/core/AuthManager.cpp
    src/core/AIHealthAssistant.cpp
    src/network/NetworkClient.cpp
    src/network/NetworkServer.cpp
    src/network/JsonMessage.cpp
    src/database/DatabaseManager.cpp
    src/utils/Validator.cpp
)

qt_add_executable(HealthCareApp ${SOURCES})

target_include_directories(HealthCareApp PRIVATE
    src
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(HealthCareApp PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    
    OpenSSL::SSL
    OpenSSL::Crypto
    SQLite::SQLite3
)

set(TEST_SOURCES
    tests/test_login.cpp
    tests/test_medication.cpp
    tests/test_reminder.cpp
    tests/test_emergency.cpp
    tests/test_validation.cpp
    tests/test_network_mock.cpp
    tests/test_ai_mock.cpp
    src/core/User.cpp
    src/core/Medication.cpp
    src/core/ReminderManager.cpp
    src/core/EmergencyManager.cpp
    src/core/AuthManager.cpp
    src/network/JsonMessage.cpp
    src/utils/Validator.cpp
)

add_executable(HealthCareTests ${TEST_SOURCES})

target_include_directories(HealthCareTests PRIVATE
    src
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(HealthCareTests PRIVATE
    GTest::GTest
    GTest::Main
    GTest::gmock
    GTest::gmock_main
    Qt6::Core
    
    OpenSSL::SSL
    OpenSSL::Crypto
)

enable_testing()
add_test(NAME HealthCareTests COMMAND HealthCareTests)


include(GoogleTest)
gtest_discover_tests(HealthCareTests)
