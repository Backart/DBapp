cmake_minimum_required(VERSION 3.16)

project(SqlLiteBDApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

add_subdirectory(Bcryptcpp)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SqlLiteBDApp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        dialogregisterwindow.h dialogregisterwindow.cpp dialogregisterwindow.ui
        adminwindow.h adminwindow.cpp adminwindow.ui
        userwindow.h userwindow.cpp userwindow.ui
        dbconnectionmanager.h dbconnectionmanager.cpp
        userauthorization.h userauthorization.cpp
        databasequeries.h databasequeries.cpp
        errormessages.h errormessages.cpp
        securityutils.h securityutils.cpp
        settingsconfigeditor.h settingsconfigeditor.cpp settingsconfigeditor.ui

    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SqlLiteBDApp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SqlLiteBDApp SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SqlLiteBDApp
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SqlLiteBDApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(SqlLiteBDApp PRIVATE Qt${QT_VERSION_MAJOR}::Sql bcryptcpp)

# target_link_libraries(SqlLiteBDApp PRIVATE bcryptcpp)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SqlLiteBDApp)
endif()
set_target_properties(SqlLiteBDApp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SqlLiteBDApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SqlLiteBDApp)
endif()
