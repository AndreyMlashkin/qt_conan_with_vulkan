set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.16)
project(hellovulkantriangle LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(${CMAKE_BINARY_DIR}/conan-dependencies/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

message(CONAN_TARGETS: ${CONAN_TARGETS})

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/vulkan/hellovulkantriangle")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Vulkan REQUIRED)

qt_add_executable(hellovulkantriangle
    shared/trianglerenderer.cpp shared/trianglerenderer.h
    main.cpp
)
set_target_properties(hellovulkantriangle PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

#conan_target_link_libraries(hellovulkantriangle)

target_link_libraries(hellovulkantriangle PUBLIC
Qt6::Core Qt6::Gui CONAN_LIB::vulkan-loader_vulkan
)


# Resources:
set_source_files_properties("shared/color_frag.spv"
    PROPERTIES QT_RESOURCE_ALIAS "color_frag.spv"
)
set_source_files_properties("shared/color_vert.spv"
    PROPERTIES QT_RESOURCE_ALIAS "color_vert.spv"
)
set(hellovulkantriangle_resource_files
    "shared/color_frag.spv"
    "shared/color_vert.spv"
)

qt6_add_resources(hellovulkantriangle "hellovulkantriangle"
    PREFIX
        "/"
    FILES
        ${hellovulkantriangle_resource_files}
)

install(TARGETS hellovulkantriangle
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
