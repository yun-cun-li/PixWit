cmake_minimum_required(VERSION 3.16)
project(CapStep VERSION 0.0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt6组件
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network)

# 设置Qt6的MOC、UIC、RCC自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 源文件列表
set(SOURCES
    # 原始单文件版本（注释掉）
    # src/main_simple_screenshot.cpp
    
    # 模块化文件
    src/main_modular.cpp
    src/GlobalHotkey.cpp
    src/StylePopover.cpp
    src/ScreenshotEditWindow.cpp
    src/StickyNoteWindow.cpp
    src/RegionSelector.cpp
    src/ScreenshotTool.cpp
    src/MainWindow.cpp
    src/UpdateChecker.cpp
    src/WindowDetector.cpp
)

# 头文件列表（暂时注释掉，避免重复定义）
# set(HEADERS
#     src/GlobalHotkey.h
#     src/StylePopover.h
#     src/ScreenshotEditWindow.h
#     src/StickyNoteWindow.h
#     src/RegionSelector.h
#     src/ScreenshotTool.h
#     src/MainWindow.h
# )

# 资源文件
set(RESOURCES
    resources.qrc
)

# Windows图标资源
if(WIN32)
    set(RESOURCES ${RESOURCES} resources/app.rc)
endif()

# 创建可执行文件
add_executable(CapStep ${SOURCES} ${RESOURCES})

# 链接Qt6库
target_link_libraries(CapStep
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Network
)

# Windows特定设置
if(WIN32)
    set_target_properties(CapStep PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    
    # 复制Qt DLL到输出目录
    add_custom_command(TARGET CapStep POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt6::Core>
        $<TARGET_FILE:Qt6::Widgets>
        $<TARGET_FILE:Qt6::Gui>
        $<TARGET_FILE_DIR:CapStep>
    )
endif()

# 设置输出目录
set_target_properties(CapStep PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 安装规则
install(TARGETS CapStep
    RUNTIME DESTINATION bin
)

# 编译选项
if(MSVC)
    target_compile_options(CapStep PRIVATE /W4)
else()
    target_compile_options(CapStep PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 调试信息
message(STATUS "CapStep build configuration:")
message(STATUS "  Qt6 version: ${Qt6_VERSION}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
