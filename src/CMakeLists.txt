project(myProject)
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

find_package(OpenGL REQUIRED)

include_directories ("${PROJECT_SOURCE_DIR}/../dependencies/Qt/include" "${PROJECT_SOURCE_DIR}/../dependencies/glew/include" "${PROJECT_SOURCE_DIR}/../dependencies/glm" "${PROJECT_SOURCE_DIR}/../dependencies/OpenMesh/include")

link_directories("${PROJECT_SOURCE_DIR}/../dependencies/Qt/lib" "${PROJECT_SOURCE_DIR}/../dependencies/glew/lib" "${PROJECT_SOURCE_DIR}/../dependencies/OpenMesh/lib")

add_definitions(-D_USE_MATH_DEFINES)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE sources *.cpp *.h)

add_executable(main ${sources})
target_link_libraries(
    main
    ${OPENGL_gl_LIBRARY}
	debug Qt5Cored
	debug Qt5Guid
	debug Qt5OpenGld
	debug Qt5Widgetsd
	debug OpenMeshCored
	optimized Qt5Core
	optimized Qt5Gui
	optimized Qt5OpenGl
	optimized Qt5Widgets
	optimized OpenMeshCore
	glew32
	glew32s
	)