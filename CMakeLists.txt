cmake_minimum_required(VERSION 3.10)
project(Opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the directories for your project
include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/include/imgui) # Include the GLAD header
include_directories(${PROJECT_SOURCE_DIR}/libs/glm)
include_directories(${PROJECT_SOURCE_DIR}/libs/glfw/include) # GLFW include directory

add_subdirectory(${PROJECT_SOURCE_DIR}/libs/glfw)

# Find the required packages
find_package(OpenGL REQUIRED)

# Add the source files
file(GLOB PROJECT_SOURCES "src/*.cpp")
list(APPEND PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/libs/glad/glad.c") # Add this line
list(APPEND PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/libs/imgui/imgui.cpp")
list(APPEND PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/libs/imgui/imgui_draw.cpp")
list(APPEND PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/libs/imgui/imgui_tables.cpp")
list(APPEND PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/libs/imgui/imgui_widgets.cpp")
list(APPEND PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/libs/imgui/imgui_impl_opengl3.cpp")
list(APPEND PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/libs/imgui/imgui_impl_glfw.cpp")
list(APPEND PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/libs/imgui/imgui_demo.cpp")

# Add executable with project sources
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Link GLFW and OpenGL with your project
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)

# Copy shader files and images from "working" directory to build directory
file(COPY ${PROJECT_SOURCE_DIR}/working DESTINATION ${CMAKE_BINARY_DIR})