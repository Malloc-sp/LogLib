cmake_minimum_required(VERSION 3.10)
project(LoggerLib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 定义源文件和头文件
set(SOURCES log.cpp)
set(PUBLIC_HEADERS log.h)

# 添加动态库目标
add_library(logger SHARED ${SOURCES} ${PUBLIC_HEADERS})

# 为 Windows 定义导出宏（如果需要跨平台）
target_compile_definitions(logger PRIVATE LOGGER_EXPORTS)

# 指定头文件目录（以便外部使用时找到）
target_include_directories(logger PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# 设置符号可见性（可选，用于 Unix 系统）
set_target_properties(logger PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

# 安装规则
install(TARGETS logger
    EXPORT LoggerLibConfig
    RUNTIME DESTINATION bin      # 对于 DLL（Windows）会安装到 bin
    LIBRARY DESTINATION lib      # 对于 .so/.dylib 安装到 lib
    ARCHIVE DESTINATION lib      # 对于 .lib（Windows 导入库）安装到 lib
)

install(FILES ${PUBLIC_HEADERS} DESTINATION include)

# 生成 CMake 包配置文件（可选，方便 find_package）
install(EXPORT LoggerLibConfig
    DESTINATION lib/cmake/LoggerLib
    NAMESPACE LoggerLib::
)