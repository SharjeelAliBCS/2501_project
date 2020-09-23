cmake_minimum_required(VERSION 2.6)

# Name of project
project(Assault_on_Terra)

#Specify header files:
include_directories(${PROJECT_SOURCE_DIR}/include)

#Specify glm (Note: It is being included as a header directory due to glm being a header only library)
include_directories(${PROJECT_SOURCE_DIR}/lib)

# Specify project source files
set(SRCS

  ${PROJECT_SOURCE_DIR}/include/Audio.h
  ${PROJECT_SOURCE_DIR}/include/BossObject.h
  ${PROJECT_SOURCE_DIR}/include/EnemyObject.h
  ${PROJECT_SOURCE_DIR}/include/FileUtils.h
  ${PROJECT_SOURCE_DIR}/include/GameObject.h
  ${PROJECT_SOURCE_DIR}/include/Graph.h
  ${PROJECT_SOURCE_DIR}/include/HUD.h
  ${PROJECT_SOURCE_DIR}/include/Node.h
  ${PROJECT_SOURCE_DIR}/include/Particle.h
  ${PROJECT_SOURCE_DIR}/include/PlayerGameObject.h
  ${PROJECT_SOURCE_DIR}/include/PowerUpObject.h
  ${PROJECT_SOURCE_DIR}/include/ProjectileObject.h
  ${PROJECT_SOURCE_DIR}/include/shader.h
  ${PROJECT_SOURCE_DIR}/include/Text.h
  ${PROJECT_SOURCE_DIR}/include/TowerObject.h
  ${PROJECT_SOURCE_DIR}/include/UpgradeObject.h
  ${PROJECT_SOURCE_DIR}/include/Window.h

  ${PROJECT_SOURCE_DIR}/src/Audio.cpp
  ${PROJECT_SOURCE_DIR}/src/BossObject.cpp
  ${PROJECT_SOURCE_DIR}/src/EnemyObject.cpp
  ${PROJECT_SOURCE_DIR}/src/FileUtils.cpp
  ${PROJECT_SOURCE_DIR}/src/GameObject.cpp
  ${PROJECT_SOURCE_DIR}/src/Graph.cpp
  ${PROJECT_SOURCE_DIR}/src/HUD.cpp
  ${PROJECT_SOURCE_DIR}/src/Node.cpp
  ${PROJECT_SOURCE_DIR}/src/Particle.cpp
  ${PROJECT_SOURCE_DIR}/src/PlayerGameObject.cpp
  ${PROJECT_SOURCE_DIR}/src/PowerUpObject.cpp
  ${PROJECT_SOURCE_DIR}/src/ProjectileObject.cpp
  ${PROJECT_SOURCE_DIR}/src/Shader.cpp
  ${PROJECT_SOURCE_DIR}/src/Text.cpp
  ${PROJECT_SOURCE_DIR}/src/TowerObject.cpp
  ${PROJECT_SOURCE_DIR}/src/UpgradeObject.cpp
  ${PROJECT_SOURCE_DIR}/src/main.cpp

)

# Add executable based on the source files
add_executable(Assault_on_Terra ${SRCS})

# Require OpenGL library
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(Assault_on_Terra ${OPENGL_gl_LIBRARY})

# Other libraries needed
set(LIBRARY_PATH C:/Users/Sharjeel\ Ali/Desktop/Programming/game/Libraries)
include_directories(${LIBRARY_PATH}/include)
if(NOT WIN32)
    find_library(GLEW_LIBRARY GLEW)
    find_library(GLFW_LIBRARY glfw)
    find_library(SOIL_LIBRARY SOIL)
elseif(WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
    find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/lib)
    find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)
endif(NOT WIN32)
target_link_libraries(Assault_on_Terra ${GLEW_LIBRARY})
target_link_libraries(Assault_on_Terra ${GLFW_LIBRARY})
target_link_libraries(Assault_on_Terra ${SOIL_LIBRARY})

# The rules here are specific to Windows Systems
if(WIN32)
    # Avoid ZERO_CHECK target in Visual Studio
    set(CMAKE_SUPPRESS_REGENERATION TRUE)

    # This will use the proper libraries in debug mode in Visual Studio
    set_target_properties(Assault_on_Terra PROPERTIES DEBUG_POSTFIX _d)
endif(WIN32)
