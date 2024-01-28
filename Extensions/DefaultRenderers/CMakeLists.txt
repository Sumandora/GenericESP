include_guard()

project(GenericESPDefaultRenderers)
add_library(GenericESPDefaultRenderers STATIC
        "${PROJECT_SOURCE_DIR}/Source/DefaultRenderers.cpp"
)
target_include_directories(GenericESPDefaultRenderers PUBLIC "${PROJECT_SOURCE_DIR}/Include")
target_compile_features(GenericESPDefaultRenderers PRIVATE cxx_std_20)
target_compile_options(GenericESPDefaultRenderers PRIVATE "-Ofast")

target_link_libraries(GenericESPDefaultRenderers GenericESP)
