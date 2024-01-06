cmake_minimum_required(VERSION 3.16)
project(COMP4300 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1)
FetchContent_MakeAvailable(SFML)

add_executable(COMP4300 src/main.cpp)
target_link_libraries(COMP4300 PRIVATE sfml-graphics)
target_compile_features(COMP4300 PRIVATE cxx_std_20)

if(WIN32)
    add_custom_command(
        TARGET COMP4300
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:COMP4300>
        VERBATIM)
endif()

add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}/bin/COMP4300
)

install(TARGETS COMP4300)
