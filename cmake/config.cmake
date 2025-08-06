#Auto Generated Root CMake file by Sage
#None
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(STATIC_LINK "Enable static linking" ON)
option(ENABLE_TESTS "GTests" OFF)
if(STATIC_LINK)
  set(BUILD_SHARED_LIBS OFF)
  if (WIN32)
      set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  else()
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
  endif()
else()
  set(BUILD_SHARED_LIBS ON)
  if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
  endif()
endif()
set(COMPANY "None")
string(TIMESTAMP CURRENT_YEAR "%Y")
set(COPYRIGHT "Copyright(c) ${CURRENT_YEAR} ${COMPANY}.")
include_directories(${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/3rdparty)
configure_file(res/config.h.in BitcoinExRCconfig.h)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Enabling secure coding features for Clang")
    add_compile_options(
        -Xclang
        -Wall -Wextra -Wpedantic
        -Wshadow -Wold-style-cast
        -Wcast-align -Wnull-dereference
        -Wformat=2 -Wformat-security
        -fstack-protector-strong
        -D_FORTIFY_SOURCE=2
        -fno-common
        #-Werror
    )
endif()
