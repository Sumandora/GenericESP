cmake_minimum_required(VERSION 3.20)

include_guard()

project(GenericESP)

if(NOT DEFINED IMGUI_DEPENDENCY_OVERRIDE)
    if(NOT PROJECT_IS_TOP_LEVEL)
        message(WARNING "ImGui dependency was not set by top level project, this is probably unintended")
    endif ()
    set(IMGUI_DEPENDENCY_OVERRIDE imgui)
endif()

if(PROJECT_IS_TOP_LEVEL)
    include(FetchContent)

    FetchContent_Declare(${IMGUI_DEPENDENCY_OVERRIDE}
            GIT_REPOSITORY https://github.com/ocornut/imgui
            GIT_PROGRESS TRUE
            GIT_TAG v1.90
            BUILD_COMMAND ""
            INSTALL_COMMAND "")
    FetchContent_MakeAvailable(${IMGUI_DEPENDENCY_OVERRIDE})

    file(GLOB IMGUI_SOURCE_FILES "${imgui_SOURCE_DIR}/*.cpp")
    list(APPEND IMGUI_SOURCE_FILES "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp")
    list(APPEND IMGUI_SOURCE_FILES "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp")

    add_library(${IMGUI_DEPENDENCY_OVERRIDE} STATIC ${IMGUI_SOURCE_FILES})
    target_include_directories(${IMGUI_DEPENDENCY_OVERRIDE} PUBLIC ${${IMGUI_DEPENDENCY_OVERRIDE}_SOURCE_DIR})
    target_link_libraries(${IMGUI_DEPENDENCY_OVERRIDE} SDL2 GL)
    target_include_directories(${IMGUI_DEPENDENCY_OVERRIDE} PUBLIC "/usr/include/SDL2")
endif()

file(GLOB_RECURSE GENERIC_ESP_FILES "Source/*.cpp")
add_library(GenericESP STATIC ${GENERIC_ESP_FILES})
target_link_libraries(GenericESP PUBLIC ${IMGUI_DEPENDENCY_OVERRIDE})
target_include_directories(GenericESP PUBLIC "${PROJECT_SOURCE_DIR}/Include")
target_compile_options(GenericESP PRIVATE "-Ofast")
target_compile_features(GenericESP PUBLIC cxx_std_23)

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory("Extensions/DefaultRenderers")
    add_executable(GenericESPExample "Example/Example.cpp" "Example/ImGuiImpl.cpp")
    target_compile_features(GenericESPExample PUBLIC cxx_std_23)
    target_link_libraries(GenericESPExample GenericESP imgui SDL2 GenericESPDefaultRenderers)
endif()

