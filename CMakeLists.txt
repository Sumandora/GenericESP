cmake_minimum_required(VERSION 3.20)

include_guard()

project(GenericESP)

add_library(GenericESP INTERFACE)
target_include_directories(GenericESP INTERFACE "${PROJECT_SOURCE_DIR}/Include")

if(PROJECT_IS_TOP_LEVEL)
    include(FetchContent)

    FetchContent_Declare(imgui
            GIT_REPOSITORY https://github.com/ocornut/imgui
            GIT_PROGRESS TRUE
            GIT_TAG v1.90
            BUILD_COMMAND ""
            INSTALL_COMMAND "")
    FetchContent_MakeAvailable(imgui)

    file(GLOB IMGUI_SOURCE_FILES "${imgui_SOURCE_DIR}/*.cpp")
    list(APPEND IMGUI_SOURCE_FILES "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp")
    list(APPEND IMGUI_SOURCE_FILES "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.cpp")

    add_library(imgui STATIC ${IMGUI_SOURCE_FILES})
    target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
    target_link_libraries(imgui SDL2 GL)
    target_include_directories(imgui PUBLIC "/usr/include/SDL2")

    add_executable(GenericESPExample "Example/Example.cpp" "Example/ImGuiImpl.cpp")
    target_compile_features(GenericESPExample PUBLIC cxx_std_23)
    target_link_libraries(GenericESPExample GenericESP imgui SDL2)
endif()

