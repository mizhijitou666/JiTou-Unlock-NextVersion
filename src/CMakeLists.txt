cmake_minimum_required(VERSION 3.20)

project(JiTou-Unlock-NextVersion-master VERSION 0.1 LANGUAGES CXX)

#配置通用编译
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

if(APPLE)
    set(APPLICATION_DIR_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.app/Contents/MacOS)
else()
    set(APPLICATION_DIR_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()


#导入Qt相关依赖包
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Quick Svg Network Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Quick Svg Network Widgets)



find_program(QT_LUPDATE NAMES lupdate)
find_program(QT_LRELEASE NAMES lrelease)
file(GLOB TS_FILE_PATHS ${CMAKE_CURRENT_LIST_DIR}/ *.ts)
add_custom_target(Script-UpdateTranslations
    COMMAND ${QT_LUPDATE} ${CMAKE_CURRENT_LIST_DIR} -ts ${PROJECT_NAME}_en_US.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${QT_LUPDATE} ${CMAKE_CURRENT_LIST_DIR} -ts ${PROJECT_NAME}_zh_CN.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${QT_LRELEASE} ${PROJECT_NAME}_en_US.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${QT_LRELEASE} ${PROJECT_NAME}_zh_CN.ts WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${APPLICATION_DIR_PATH}/i18n
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_NAME}_en_US.qm ${PROJECT_NAME}_zh_CN.qm ${APPLICATION_DIR_PATH}/i18n
    SOURCES ${TS_FILE_PATHS}
)

#遍历所有Cpp文件
file(GLOB_RECURSE CPP_FILES *.cpp *.h)
foreach (filepath ${CPP_FILES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND sources_files ${filename})
endforeach (filepath)

#如果是Windows平台，则生成rc文件，还有inno setup脚本文件
set(EXAMPLE_VERSION_RC_PATH "")
if (WIN32)
    set(EXAMPLE_VERSION_RC_PATH ${CMAKE_CURRENT_BINARY_DIR}/version_${PROJECT_NAME}.rc)
    configure_file(
            ${CMAKE_SOURCE_DIR}/.cmake/version_exe.rc.in
            ${EXAMPLE_VERSION_RC_PATH}
    )
endif ()


list(APPEND sources_files ${PROJECT_NAME}.qrc)

#添加可执行文件
if (WIN32)
    list(APPEND sources_files ${EXAMPLE_VERSION_RC_PATH})
endif ()
if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
            MANUAL_FINALIZATION
            ${sources_files}
    )
else ()
    add_executable(${PROJECT_NAME}
            ${sources_files}
            plistmanager.cpp plistmanager.h
            helper/SettingsHelper.cpp helper/SettingsHelper.h
            singleton.h
            devicemanager.cpp
            devicemanager.h
            utils.cpp
            utils.h
            helper/LinkHelper.h
            helper/LinkHelper.cpp
            helper/deviceLink.h
            helper/deviceLink.cpp

    )
endif ()

add_dependencies(${PROJECT_NAME} Script-UpdateTranslations)

# Copy preset files to the output directory
set(FILES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/files")
file(COPY ${FILES_PATH} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#导入component头文件,不然通过QML_NAMED_ELEMENT生成的c++类会找不到头文件报错
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/component
)


#如果是静态库则需要手动注册插件，导入FluentUI.h头文件
if (FLUENTUI_BUILD_STATIC_LIB)
    target_include_directories(${PROJECT_NAME} PRIVATE
            ${CMAKE_SOURCE_DIR}/src
    )
endif ()

#设置属性
set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.${PROJECT_NAME}.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Svg
    fluentuiplugin
)
# Copy the DLL files to the output directory
set(DLL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/dll")
set(EXE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/exe")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
if(WIN32)
    file(GLOB EXE_FILES "${EXE_PATH}/*.exe")
    file(GLOB DLL_FILES "${DLL_PATH}/*.dll")
    file(COPY ${EXE_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${DLL_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/libimobiledevice-1.0.dll")
    target_link_libraries(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/libplist-2.0.dll")
    target_link_libraries(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/libplist++-2.0.dll")
    target_link_libraries(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/libssl-3.dll")
    target_link_libraries(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/libcrypto-3.dll")
    target_link_libraries(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/libtatsu.dll")
endif()

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if (CMAKE_BUILD_TYPE MATCHES "Release")
    if (APPLE)
        find_program(QT_DEPLOY_QT NAMES macdeployqt)
        add_custom_target(Script-DeployRelease
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/dist
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}/dist
            COMMAND ${QT_DEPLOY_QT} ${CMAKE_SOURCE_DIR}/dist/${PROJECT_NAME}.app -qmldir=${CMAKE_CURRENT_LIST_DIR}
            COMMENT "MacOs Deploying Qt Dependencies After Build........."
            SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()
    if(WIN32)
        find_program(QT_DEPLOY_QT NAMES windeployqt)
        add_custom_target(Script-DeployRelease
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/dist
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_SOURCE_DIR}/dist
            COMMAND ${QT_DEPLOY_QT} --qmldir=${CMAKE_CURRENT_LIST_DIR} --plugindir ${CMAKE_SOURCE_DIR}/dist/plugins --no-translations --compiler-runtime ${CMAKE_SOURCE_DIR}/dist/${PROJECT_NAME}.exe
            COMMENT "Windows Deploying Qt Dependencies After Build........."
            SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endif()
endif()
