cmake_minimum_required(VERSION 3.13)

if(NOT CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

project(naughtyfication)

set(SOURCES
    naughtyfication.cpp
    shortcutcreator.cpp
    win32_api.cpp
)

add_executable(naughtyfication ${SOURCES})
target_include_directories(naughtyfication
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/../include
)

find_library(shlwapi_LIBRARY shlwapi libshlwapi libshlwapi.a HINTS ${CMAKE_C_IMPLICIT_LINK_DIRECTORIES})
if(NOT shlwapi_LIBRARY)
    message(FATAL_ERROR "shlwapi library not found")
endif(NOT shlwapi_LIBRARY)

find_library(propsys_LIBRARY propsys libpropsys libpropsys.a HINTS ${CMAKE_C_IMPLICIT_LINK_DIRECTORIES})
if(NOT propsys_LIBRARY)
    message(FATAL_ERROR "propsys library not found")
endif(NOT propsys_LIBRARY)

find_library(ntdll_LIBRARY ntdll libntdll libntdll.a HINTS ${CMAKE_C_IMPLICIT_LINK_DIRECTORIES})
if(NOT ntdll_LIBRARY)
    message(FATAL_ERROR "ntdll library not found")
endif(NOT ntdll_LIBRARY)

target_link_libraries(naughtyfication PUBLIC ${shlwapi_LIBRARY} ${propsys_LIBRARY} ${ntdll_LIBRARY} -static)
target_compile_features(naughtyfication PUBLIC cxx_std_20)
target_compile_options(naughtyfication PUBLIC -municode -DUNICODE -fcoroutines -iquote ../../include/winrt/yolort_impl)