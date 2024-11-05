cmake_minimum_required(VERSION 3.16)

project(tictactoe VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing(true)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Test)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
find_package(GTest REQUIRED)

qt_standard_project_setup()

qt_add_library(tictactoe-lib)
qt_add_qml_module(tictactoe-lib
    URI tictactoe
    VERSION 1.0
    QML_FILES qml/Main.qml
    SOURCES include/gamestate.h src/gamestate.cpp
)

target_include_directories(tictactoe-lib PUBLIC "include/")
target_link_libraries(tictactoe-lib
    PUBLIC Qt6::Quick
)

qt_add_executable(tictactoe-app
    src/main.cpp
)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(tictactoe-app PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.tictactoe
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(tictactoe-app PRIVATE tictactoe-lib)

qt_add_executable(tictactoe-test

)

set_target_properties(tictactoe-test PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.tictactoe
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(tictactoe-test PRIVATE tictactoe-lib GTest::gtest_main GTest::gmock_main)


include(GNUInstallDirs)
install(TARGETS tictactoe-app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(TARGETS tictactoe-lib
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
