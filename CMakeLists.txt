# /*
#     Copyright (C) 2021  kelvin-luo

#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU Affero General Public License as published
#     by the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.

#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU Affero General Public License for more details.

#     You should have received a copy of the GNU Affero General Public License
#     along with this program.  If not, see <https://www.gnu.org/licenses/>.

#     My Github homepage: https://github.com/kelvin-luo
# */
cmake_minimum_required(VERSION 3.16)

set(NAME StableVerse)
project(${NAME} LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets OpenGL OpenGLWidgets REQUIRED)

if(QT_VERSION_MAJOR MATCHES "5")
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Concurrent Multimedia MultimediaWidgets   Network  REQUIRED) # OpenGL
else()
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Concurrent Multimedia MultimediaWidgets Core5Compat  OpenGL OpenGLWidgets Network REQUIRED)
endif()

# set(trage_runtime_output_dir ${OUTPUT_DIR_KENT})
set(trage_runtime_output_dir ${CMAKE_SOURCE_DIR}/../bin)
set(EXECUTABLE_OUTPUT_PATH "${trage_runtime_output_dir}") # qtcreator + msvc 可执行文件的输出目录
set(CMAKE_BINARY_DIR "${trage_runtime_output_dir}")             # visual studio 2022 可执行的输出目录
file(MAKE_DIRECTORY ${trage_runtime_output_dir})                      # 创建输出目录


#***************************************************通用参数的定义 开始**********************************************
# get kit architecture bit
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(architecture_bit "x86")
    set(build_x64bit false)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(architecture_bit "x64")
    set(build_x64bit true)
endif()

# 设置变量 qt/ssl/lupdate/lrelease 在不同编译器下的环境和版本下的 path
get_target_property(QtCore_location Qt${QT_VERSION_MAJOR}::Core LOCATION)
get_filename_component(qt_binray_dir ${QtCore_location} DIRECTORY)
message("Qt installation directory qt_binray_dir: ${qt_binray_dir}")            # C:/Qt/6.7.3/msvc2022_64/bin

set(lupdate_path "${qt_binray_dir}/lupdate")
set(lrelease_path "${qt_binray_dir}/lrelease")

message("--->architecture_bit:" ${architecture_bit}  "  build_x64bit:" ${build_x64bit}  "  ssl_dir:" ${ssl_dir} "  HOME_DIR:" ${HOME_DIR} "  env_os:" ${env_os})
message("lupdate_path:" ${lupdate_path})
message("lrelease_path:" ${lrelease_path})
message("EXECUTABLE_OUTPUT_PATH:" ${EXECUTABLE_OUTPUT_PATH})
message("CMAKE_BINARY_DIR:" ${CMAKE_BINARY_DIR})



#************************************************* update .ts, and generate .qm *******************************************************
set(ts_dir "${CMAKE_CURRENT_SOURCE_DIR}/translations")
set(qm_dir ${CMAKE_BINARY_DIR}/translations)

file(MAKE_DIRECTORY "${ts_dir}" "${qm_dir}")
file(GLOB ts_files "${ts_dir}/*.ts")

set(ts_files_name "")    # 获取文件名部分（不包括路径）
foreach(it ${ts_files})
    get_filename_component(ts_filename ${it} NAME_WE)
    list(APPEND ts_files_name ${ts_filename})
endforeach()

# 编译时更新 .ts 和生成 .qm, # 命令参数问题：确保命令的参数传递方式正确。有时候在使用 execute_process 时，参数的传递需要一些特殊的格式，比如将参数放到列表中传递。
foreach(it ${ts_files_name})
    # execute_process(COMMAND "${lupdate_path}" -locations none -no-obsolete "${CMAKE_CURRENT_SOURCE_DIR}" -ts "${ts_dir}/${it}.ts")
    execute_process(COMMAND "${lrelease_path}" "${ts_dir}/${it}.ts" -qm "${qm_dir}/${it}.qm")
endforeach()

file(GLOB_RECURSE qm_files "${qm_dir}/*.qm")

message("-->ts_files_name: ${ts_files_name}")
message("-->ts_files: ${ts_files}")
message("-->qm_files: ${qm_files}")
message("-->qm_dir: ${qm_dir}")
#***************************************************.ts 生成 .qm 结束**********************************************


include_directories(gui . activation common core ../../thirdparty)
file(GLOB_RECURSE soruce core/*.cpp common/*.cpp gui/*.cpp   *.h *.ui *.qrc *.ts *.rc)

add_executable(${NAME} ${soruce} )

if(QT_VERSION_MAJOR MATCHES "5")
    target_link_libraries(${NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network ) #  Qt${QT_VERSION_MAJOR}::OpenGL  -lopengl32
else()
    target_link_libraries(${NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core5Compat Qt${QT_VERSION_MAJOR}::OpenGL Qt${QT_VERSION_MAJOR}::OpenGLWidgets Qt${QT_VERSION_MAJOR}::Network)
endif()

########################控制台上不乱码#################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")             # 开启 MSVC 终端上的中文不乱码
add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
target_compile_definitions(${NAME} PRIVATE
    WIN32_LEAN_AND_MEAN                                      # Header files containing only the common APIs
    _CRT_SECURE_NO_WARNINGS                                  # Unsafe functions such as strcpy, scanf, etc. are allowed)
)
########################控制台上不乱码#################################


if(0)
    # # 确保可执行文件与imageProc.exe在同一目录
    # add_custom_command(TARGET ${NAME} POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E copy_if_different
    #         $<TARGET_FILE:imageProc>
    #         $<TARGET_FILE_DIR:${NAME}>
    #     COMMENT "Copying sd.exe to output directory"
    # )
endif()

#***************************************************************************************
if(0)
    set(windeployqt "${qt_binray_dir}/windeployqt.exe")
    add_custom_command(TARGET ${NAME} POST_BUILD COMMAND "${windeployqt}" "${EXECUTABLE_OUTPUT_PATH}/${NAME}.exe" --no-translations)
    message("--->windeployqt(or mac/linux): ${windeployqt}")
endif()
