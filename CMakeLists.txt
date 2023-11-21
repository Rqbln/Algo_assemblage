cmake_minimum_required(VERSION 3.23)
project(Algo_assemblage C)
set(CMAKE_C_STANDARD 23)

# Définir les chemins pour GLPK (uniquement pour Windows 64 bits)
set(GLPK_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/windows/glpk-4.65/w64")
set(GLPK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/windows/glpk-4.65/src")

# Ajouter le répertoire des fichiers d'en-tête et des bibliothèques
include_directories("${GLPK_INCLUDE_DIR}")
link_directories("${GLPK_LIB_DIR}")

# Spécifier les fichiers sources
file(GLOB SOURCES "src/*.c")

add_executable(Algo_assemblage ${SOURCES})

# Lier avec la bibliothèque GLPK
target_link_libraries(Algo_assemblage glpk_4_65)
