cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME scia)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

add_subdirectory(thirdparty/sqlite)
add_subdirectory(shared)
add_subdirectory(person)
add_subdirectory(app_composition)
add_subdirectory(qml)

qt_add_resources(app_resources resources/resources.qrc)
set(APP_NAME app${PROJECT_NAME})
qt_add_executable(${APP_NAME}
        main.cpp
        ${app_resources}
)
set_target_properties(${APP_NAME} PROPERTIES
        ENVIRONMENT "LANG=C.UTF-8;LC_ALL=C.UTF-8"
)
qt_import_qml_plugins(${APP_NAME})

target_link_libraries(${APP_NAME}
        PRIVATE
        ${PROJECT_NAME}::Person
        ${PROJECT_NAME}::Shared
        ${PROJECT_NAME}::AppComposition
        ${PROJECT_NAME}::Qml
        ${PROJECT_NAME}::Sqlite
        Qt6::Core
        Qt6::Quick
        Qt6::QuickControls2
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(${APP_NAME} PROPERTIES
        #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${APP_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${APP_NAME}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
