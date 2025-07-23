set(Opt_QT 1)
if(Opt_QT)
  set(CMAKE_AUTOUIC ON)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_AUTORCC ON)
  find_package(
    QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Network Xml Multimedia MultimediaWidgets Widgets
  )# Widgets #WebEngineWidgets WebChannel Widgets LinguistTools Core Gui PrintSupport
  find_package(Qt${QT_VERSION_MAJOR} REQUIRED
               COMPONENTS Network Xml Multimedia MultimediaWidgets Widgets)
endif()

set(Opt_CUDA 0)
if(Opt_CUDA)
  set(CUDA_STANDARD_REQUIRED ON)
  # 设置项目的C++标准为C++20 set(CMAKE_CUDA_STANDARD 20)
  set(CMAKE_CUDA_STANDARD 17) # 使用cuda时,VS2019只能知道到c++17.
  find_package(CUDA REQUIRED) # 旧版特性
  message(STATUS "CUDA version: ${CUDA_VERSION}")

  enable_language("CUDA") # 新版特性
  set(CUDA_STANDARD_REQUIRED ON)
  set(CUDA_ARCHITECTURES "50;72;75;80;86") # Unsupported gpu architecture
                                           # 'compute_35'
  set(CMAKE_CUDA_ARCHITECTURES "50;72;75;80;86")
  set(CUDA_SEPARABLE_COMPILATION ON)
  set(CUDA_NVCC_FLAGS
      ${CUDA_NVCC_FLAGS}
      # -gencode arch=compute_20,code=sm_20 -gencode arch=compute_20,code=sm_21
      # -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35
      # -gencode arch=compute_50,code=sm_50
      -gencode
      arch=compute_61,code=sm_61
      -gencode
      arch=compute_60,code=sm_60
      # -gencode arch=compute_68,code=sm_68
      -gencode
      arch=compute_50,code=compute_50
      # -gencode arch=compute_86,code=compute_86
      -rdc=true)
endif( )

if(NOT CMAKE_BUILD_TYPE)
  # set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or
  # Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")

elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")

elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")

else()
  message(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
endif()

#set(CMAKE_PREFIX_PATH
#    "D:\\Qt\\Qt5.12.12;D:\\binWin10\\LLVM;D:\\binWin10\\llvm-mingw-20220906-msvcrt-x86_64"
#)

message(STATUS "------------> CMAKE_SYSTEM_NAME= ${CMAKE_SYSTEM_NAME}")

set(toolChainMy "MinGWTCh")
if(0)
  # if (CMAKE_GENERATOR MATCHES "MinGW") message(STATUS "CMake is using MinGW")
  # set(toolChainMy "MinGWTCh") elseif (CMAKE_GENERATOR MATCHES "MSVC")
  # set(toolChainMy "MSVCTCh") elseif (CMAKE_GENERATOR MATCHES "CYGWIN")
  # set(toolChainMy "CYGWINTCh") elseif (CMAKE_GENERATOR MATCHES "GNU")
  # set(toolChainMy "GNUTCh") else () message(STATUS "CMake is not kowningwhat.")
  # set(toolChainMy "UnkownTCh")
 # endif ()

  message(STATUS "_________________________________")
  # 判断CMAKE_C_COMPILER的类型
  if(CMAKE_CXX_COMPILER MATCHES "gcc")
    if(CMAKE_SYSTEM_NAME MATCHES "Windows")
      message(STATUS "Cygwin/MinGW Compiler")
      set(toolChainMy "CygwinMinGWTCh")
    elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
      message(STATUS "GNU Compiler")
      set(toolChainMy "GNUTCh")
    else()
      message(STATUS "other Compiler")
      set(toolChainMy "OtherTCh")
    endif()
  elseif(CMAKE_CXX_COMPILER MATCHES "clang")
    message(STATUS "Clang Compiler")
    set(toolChainMy "ClangTCh")
  elseif(CMAKE_CXX_COMPILER MATCHES "cl")
    message(STATUS "MSVC Compiler")
    set(toolChainMy "MSVC")
  else()
    message(STATUS "Unknown Compiler")
    set(toolChainMy "UnknownTCh")
  endif()

#   # if (CMAKE_CXX_PLATFORM_ID MATCHES "MinGW")
#  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU") message(STATUS "MinGW Compiler")
#  set(toolChainMy "${CMAKE_CXX_PLATFORM_ID}_${CMAKE_CXX_COMPILER_ID}")
# endif()
# endif()
endif()
set(toolChainMy "${CMAKE_CXX_PLATFORM_ID}_${CMAKE_CXX_COMPILER_ID}")
message(STATUS "toolChainMy = ${toolChainMy}")

message(STATUS "_________________________________")
if(0)
  include(CheckCXXSourceCompiles)
  set(CMAKE_REQUIRED_DEFINITIONS -D_MSC_VER)
  check_cxx_source_compiles(
    "
        #include <Windows.h>
        int main() {
        #ifdef _MSC_VER
        // 如果定义了_MSC_VER，这里的代码将会被编译
        #endif
        return 0;
        }"
    MSCRT_SUPPORTED)

  if(MSCRT_SUPPORTED)
    message(STATUS "MS CRT is supported")
  else()
    message(STATUS "MS CRT is not supported")
  endif()
  message(STATUS "_________________________________")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  message(STATUS "current platform: Linux ")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
  message(STATUS "current platform: Windows")
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  message(STATUS "current platform: Mac OS X")
else()
  message(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

#cmake_host_system_information(RESULT PRETTY_NAME QUERY DISTRIB_PRETTY_NAME)
#message(STATUS "${PRETTY_NAME}")

#cmake_host_system_information(RESULT DISTRO QUERY DISTRIB_INFO)
foreach(VAR IN LISTS DISTRO)
  message(STATUS "${VAR}=`${${VAR}}`")
endforeach()

if(0)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  if(MSVC)
    set(CompilerFlags
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE)
    foreach(CompilerFlag ${CompilerFlags})
      string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
  endif(MSVC)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_DEBUG_POSTFIX "d")
endif()

if(MSVC)
  # set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_DEBUG_POSTFIX "d")
  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_DEBUG_POSTFIX "")
  endif()
  # add_compile_options( /bigobj  /Ob2) # /Ob2 is must for this program. :-1: error: LNK1189: 超过 65535 对象的库限制
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8 /bigobj  /Ob2")
  # add_compile_options( /bigobj /Ob2) # /Ob2 is must for this program. :-1: error: LNK1189: 超过 65535 对象的库限制
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8 /bigobj /Ob2")
  # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /utf-8 /bigobj /Ob2")
  # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /utf-8 /bigobj  /Ob2")
  # set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /utf-8 /bigobj /Ob2")
  # set (CMAKE_CXX_STANDARD 11)
  # set (CMAKE_CXX_FLAGS_DEBUG  "-Wall -WX -O0 -g") #要小心这个选项，可能会使你的程序在windows下不能“正常运行”，实际上是警告会被认为错误。极其严格。
  # set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O2")
  # set (CMAKE_BUILD_TYPE Debug CACHE STRING "Build Type" FORCE)
  # set_property (CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release")
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fexec-charset=utf-8 /source-charset=utf-8 /execution-charset=utf-8")
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fexec-charset=utf-8 /source-charset:utf-8 /execution-charset:utf-8")
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexec-charset=GBK")
  # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fexec-charset=GBK")
else()
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexec-charset=utf-8")
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
endif()

# link_directories(${CMAKE_BINARY_DIR} )
message("CMAKE_HOST_SYSTEM = ${CMAKE_HOST_SYSTEM}")
message("CMAKE_HOST_SYSTEM_NAME = ${CMAKE_HOST_SYSTEM_NAME}")
message("CMAKE_SYSTEM_NAME= ${CMAKE_SYSTEM_NAME}")
message("CMAKE_GENERATOR= ${CMAKE_GENERATOR}")
message("CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}")
message("CMAKE_C_COMPILER_VERSION = ${CMAKE_C_COMPILER_VERSION}")
message("CMAKE_C_COMPILER_ID = ${CMAKE_C_COMPILER_ID}")
message("CMAKE_C_COMPILER_ABI = ${CMAKE_C_COMPILER_ABI}")
message("CMAKE_C_COMPILER_TARGET = ${CMAKE_C_COMPILER_TARGET}")
# message("CMAKE_C_COMPILER_ARCHITECTURE_ID=
# ${CMAKE_C_COMPILER_ARCHITECTURE_ID}")
message("CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}")
# message("CMAKE_CXX_COMPILER_TARGET= ${CMAKE_CXX_COMPILER_TARGET}")
message("CMAKE_CXX_PLATFORM_ID = ${CMAKE_CXX_PLATFORM_ID}")
message("CMAKE_CXX_COMPILER_TARGET = ${CMAKE_CXX_COMPILER_TARGET}")
message("CMAKE_CXX_COMPILER_AR = ${CMAKE_CXX_COMPILER_AR}")
message("CMAKE_CXX_COMPILER_ABI= ${CMAKE_CXX_COMPILER_ABI}")
message(
  "CMAKE_CXX_COMPILER_EXTERNAL_TOOLCHAIN= ${CMAKE_CXX_COMPILER_EXTERNAL_TOOLCHAIN}"
)
message("CMAKE_CXX_COMPILER_ID= ${CMAKE_CXX_COMPILER_ID}")
message("CMAKE_CXX_COMPILER_VERSION=${CMAKE_CXX_COMPILER_VERSION}")
message(
  "CMAKE_CXX_COMPILER_ARCHITECTURE_ID= ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
# message("CMAKE_HOST_SYSTEM_NAME= ${CMAKE_HOST_SYSTEM_NAME}")
message("CMAKE_Fortran_FORMAT= ${CMAKE_Fortran_FORMAT}")
message("CMAKE_Fortran_COMPILER= ${CMAKE_Fortran_COMPILER}")
message("CMAKE_Fortran_COMPILER_VERSION= ${CMAKE_Fortran_COMPILER_VERSION}")
message("CMAKE_Fortran_COMPILER_ID= ${CMAKE_Fortran_COMPILER_ID}")
message("CMAKE_VS_PLATFORM_NAME = ${CMAKE_VS_PLATFORM_NAME}")
message("CMAKE_VS_PLATFORM_TOOLSET = ${CMAKE_VS_PLATFORM_TOOLSET}")

if(Opt_CUDA)
  enable_language("CUDA")
  set(CUDA_STANDARD_REQUIRED ON)
  set(CUDA_ARCHITECTURES "50;72;75;80;86") # Unsupported gpu architecture
                                           # 'compute_35'
  set(CMAKE_CUDA_ARCHITECTURES "50;72;75;80;86")
  set(CUDA_SEPARABLE_COMPILATION ON)
endif()

message(STATUS "----------------------->")
message(STATUS "cuda version: " ${CUDA_VERSION_STRING})
message(STATUS "CUDA_VERSION: " ${CUDA_VERSION})
message(STATUS "CUDA_STANDARD: " ${CUDA_STANDARD})
message(STATUS "CMAKE_CUDA_FLAGS: " ${CMAKE_CUDA_FLAGS})
message(STATUS "CUDA_ARCHITECTURES: " ${CUDA_ARCHITECTURES})
message(STATUS "CMAKE_CUDA_ARCHITECTURES: " ${CMAKE_CUDA_ARCHITECTURES})
message(STATUS "CUDA_STANDARD_REQUIRED: " ${CUDA_STANDARD_REQUIRED})
message(STATUS "CMAKE_CUDA_HOST_COMPILER: " ${CMAKE_CUDA_HOST_COMPILER})
message(STATUS "CMAKE_CUDA_EXTENSIONS: " ${CMAKE_CUDA_EXTENSIONS})
message(STATUS "CUDACXX: ${CUDACXX}")
message(STATUS "CUDA_CUBIN_COMPILATION: ${CUDA_CUBIN_COMPILATION}")
message(STATUS "CUDAARCHS: ${CUDAARCHS}")
# message(STATUS "CUDA_ARCHITECTURES: ${CUDA_ARCHITECTURES}")
message(STATUS "CUDAHOSTCXX: ${CUDAHOSTCXX}")
message(STATUS "CUDA_COMPILER_ID: ${CUDA_COMPILER_ID}")
message(STATUS "MAKE_CUDA_KNOWN_FEATURES: ${MAKE_CUDA_KNOWN_FEATURES}")
message(STATUS "CUDA_RUNTIME_LIBRARY: ${CUDA_RUNTIME_LIBRARY}")
message(STATUS "CUDA_COMPILER_VERSION : ${CUDA_COMPILER_VERSION}")
message(
  STATUS "CMAKE_VS_PLATFORM_TOOLSET_CUDA: ${CMAKE_VS_PLATFORM_TOOLSET_CUDA}")
message(
  STATUS
    "CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES: ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}"
)
message(STATUS "CMAKE_CUDA_TOOLKIT_LIBRARIES: ${CMAKE_CUDA_TOOLKIT_LIBRARIES}")
message(STATUS "MAKE_CUDA_COMPILE_FEATURES: ${MAKE_CUDA_COMPILE_FEATURES}")
message(STATUS "CMAKE_CUDA_TOOLKIT_VERSION: " ${CMAKE_CUDA_TOOLKIT_VERSION})
message(
  STATUS
    "CMAKE_CUDA_TOOLKIT_VERSION_STRING: ${CMAKE_CUDA_TOOLKIT_VERSION_STRING}")
message(
  STATUS
    "CMAKE_VS_PLATFORM_TOOLSET_CUDA_CUSTOM_DIR: ${CMAKE_VS_PLATFORM_TOOLSET_CUDA_CUSTOM_DIR}"
)
message(
  STATUS
    "CMAKE_CUDA_RESOLVE_DEVICE_SYMBOLS: ${CMAKE_CUDA_RESOLVE_DEVICE_SYMBOLS}")
message("CUDA_NVCC_FLAGS=${CUDA_NVCC_FLAGS}")
message(STATUS "<-----------------------")

# Kepler GeForce GTX 770, GTX 760, GT 740 ARCH= -gencode
# arch=compute_30,code=sm_30

# For Jetson TX1, Tegra X1, DRIVE CX, DRIVE PX - uncomment: ARCH= -gencode
# arch=compute_53,code=[sm_53,compute_53]

# GP100/Tesla P100 - DGX-1 ARCH= -gencode arch=compute_60,code=sm_60

# For Jetson Tx2 or Drive-PX2 uncomment: ARCH= -gencode
# arch=compute_62,code=[sm_62,compute_62]

# Tesla V100 ARCH= -gencode arch=compute_70,code=[sm_70,compute_70]

# Jetson XAVIER ARCH= -gencode arch=compute_72,code=[sm_72,compute_72]

# GeForce RTX 2080 Ti, RTX 2080, RTX 2070, Quadro RTX 8000, Quadro RTX 6000,
# Quadro RTX 5000, Tesla T4, XNOR Tensor Cores ARCH= -gencode
# arch=compute_75,code=[sm_75,compute_75]

# GTX 1080, GTX 1070, GTX 1060, GTX 1050, GTX 1030, Titan Xp, Tesla P40, Tesla
# P4 ARCH= -gencode arch=compute_61,code=sm_61 -gencode
# arch=compute_61,code=compute_61

# Tesla A100 (GA100), DGX-A100, RTX 3080 ARCH= -gencode
# arch=compute_80,code=[sm_80,compute_80]

# For Tesla GA10x cards, RTX 3090, RTX 3080, RTX 3070, RTX A6000, RTX A40
# uncomment: ARCH= -gencode arch=compute_86,code=[sm_86,compute_86] GeForce RTX
# 3070, 3080, 3090 ARCH= -gencode arch=compute_86,code=[sm_86,compute_86]

message("----->" ${CMAKE_CUDA_ARCHITECTURES})
# string(REGEX MATCHALL "[0-9A-Za-z_]*.cpp" tmp1 ${var})
if(Opt_CUDA)
  string(REGEX MATCHALL "[0-9_];[0-9_]" tmp1 ${CMAKE_CUDA_ARCHITECTURES}) # this
endif()
# message(${tmp1}) string(REGEX REPLACE "[0-9];[0-9]" "[0-9][0-9]"
# CMAKE_CUDA_ARCHITECTURES_MY ${tmp1}) string(REGEX REPLACE
# CMAKE_CUDA_ARCHITECTURES ";" CMAKE_CUDA_ARCHITECTURES_MY)
# set(CMAKE_CUDA_ARCHITECTURES_MY ${CMAKE_CUDA_ARCHITECTURES})
message("----->CMAKE_CUDA_ARCHITECTURES_MY = " ${CMAKE_CUDA_ARCHITECTURES_MY})

foreach(item ${CMAKE_CUDA_ARCHITECTURES})
  # string(MERGE ) set(CMAKE_CUDA_ARCHITECTURES_MY
  # "${CMAKE_CUDA_ARCHITECTURES_MY}_${item}")
endforeach()

message("======> LANGUAGES = " ${LANGUAGES})

set(prefix_my "${CMAKE_SOURCE_DIR}/../${PROJECT_NAME}_${PROJECT_VERSION}")
if(Opt_QT)
  set(prefix_my
      "${prefix_my}_QT_${QT_VERSION_MAJOR}_${QT_VERSION_MINOR}_${QT_VERSION_PATCH}"
  )
endif()

set(prefix_my "${prefix_my}_${toolChainMy}")

if(CMAKE_C_COMPILER_VERSION)
  set(prefix_my "${prefix_my}_C_${CMAKE_C_COMPILER_VERSION}")
endif()

if(CMAKE_CXX_COMPILER_VERSION)
  # CMAKE_CXX_COMPILER_AR = D:/msys64/ucrt64/bin/gcc-ar.exe
  string(FIND "${CMAKE_CXX_COMPILER_AR}" "ucrt" pos)
  if(pos GREATER -1)
    message("")
    set(prefix_my "${prefix_my}_ucrt")
  endif()
  string(FIND "${CMAKE_CXX_COMPILER_AR}" "clang" pos)
  if(pos GREATER -1)
    message("")
    set(prefix_my "${prefix_my}_clang")
  endif()
  string(FIND "${CMAKE_CXX_COMPILER_AR}" "mingw" pos)
  if(pos GREATER -1)
    message("")
    set(prefix_my "${prefix_my}_mingw")
  endif()

  if(1)
    set(prefix_my
      "${prefix_my}_CC_${CMAKE_CXX_COMPILER_VERSION}_${CMAKE_CXX_STANDARD}_${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}"
    )
endif()
endif()

if(0)
set(prefix_my ${prefix_my}_${CMAKE_HOST_SYSTEM})

if(CMAKE_Fortran_COMPILER_VERSION)
  set(prefix_my "${prefix_my}_F_${CMAKE_Fortran_COMPILER_VERSION}")
endif()

if(Opt_CUDA)
  set(prefix_my
      "${prefix_my}_cuda_${CUDA_VERSION}_${CMAKE_CUDA_ARCHITECTURES_MY}")
endif()
endif()


set(CMAKE_INSTALL_PREFIX ${prefix_my}_install)


set(OUTPUT_DIR_KENT ${prefix_my}_bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR_KENT})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR_KENT})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR_KENT})

# install
include(CMakePackageConfigHelpers)

# # aux_source_directory(./  sourceAll )
# file(GLOB_RECURSE  ./*.cpp all_source_files )
# file(GLOB_RECURSE sources  *.cpp *.h)
# file(GLOB_RECURSE sourcesBuild  build/*.cpp  )
# list(REMOVE_ITEM sources ${sourcesBuild})
