# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

if(DEFINED ENV{PICO_BOARD})
  set(PICO_BOARD $ENV{PICO_BOARD} CACHE STRING "Board type")
else()
  set(PICO_BOARD pico CACHE STRING "Board type")
endif()

cmake_minimum_required(VERSION 3.13)
project(GP2040-CE LANGUAGES C CXX ASM)

# Enable compile commands for IDE autocompletion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Pull in the SDK
include(pico_sdk_import.cmake)
pico_sdk_init()

# Versioning
find_package(Git QUIET)
if(GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty --abbrev=7
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_REPO_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${GIT_EXECUTABLE} describe --always --abbrev=7
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_REPO_BUILD_ID
        OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
    set(GIT_REPO_VERSION "unknown")
    set(GIT_REPO_BUILD_ID "unknown")
endif()

string(REGEX REPLACE "v([0-9]+\\.[0-9]+\\.[0-9]+).*" "\\1" CMAKE_GIT_REPO_VERSION ${GIT_REPO_VERSION})
string(REGEX REPLACE "^(.......-.*)|(.......)$" "0.0.0" CMAKE_GIT_REPO_VERSION ${CMAKE_GIT_REPO_VERSION})

configure_file("headers/version.h.in" "headers/version.h")
message("GIT_REPO_VERSION is ${GIT_REPO_VERSION}")
message("CMAKE_GIT_REPO_VERSION is ${CMAKE_GIT_REPO_VERSION}")
message("GIT_REPO_BUILD_ID is ${GIT_REPO_BUILD_ID}")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Optional: Board config
if(DEFINED ENV{GP2040_BOARDCONFIG})
  set(GP2040_BOARDCONFIG $ENV{GP2040_BOARDCONFIG})
else()
  set(GP2040_BOARDCONFIG Pico)
endif()

# Optional: Skip submodules
if(DEFINED ENV{SKIP_SUBMODULES})
  set(SKIP_SUBMODULES $ENV{SKIP_SUBMODULES})
else()
  set(SKIP_SUBMODULES FALSE)
endif()

# Optional: Skip web build
if(DEFINED ENV{SKIP_WEBBUILD})
  set(SKIP_WEBBUILD $ENV{SKIP_WEBBUILD})
else()
  set(SKIP_WEBBUILD FALSE)
endif()

# Update submodules if needed
if(NOT SKIP_SUBMODULES)
  find_package(Git QUIET)
  if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    message(STATUS "Submodules updated")
  endif()
endif()

# Optional web build
if(NOT SKIP_WEBBUILD)
  message(STATUS "Web build enabled")
  include(${CMAKE_SOURCE_DIR}/modules/FindNodeJS.cmake)
  include(${CMAKE_SOURCE_DIR}/modules/FindNPM.cmake)
  if(NODEJS_FOUND AND NPM_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/www/package.json")
    execute_process(COMMAND ${NPM_EXECUTABLE} ci
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/www)
    execute_process(COMMAND ${NPM_EXECUTABLE} run build
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/www)
    message(STATUS "Web assets built successfully")
  else()
    message(WARNING "NodeJS/NPM not found or package.json missing, skipping web build")
  endif()
endif()

# Board header path
set(PICO_BOARD_HEADER_DIRS ${CMAKE_SOURCE_DIR}/configs/${GP2040_BOARDCONFIG})

# Dependencies
include(FetchContent)
FetchContent_Declare(ArduinoJson
    GIT_REPOSITORY https://github.com/bblanchon/ArduinoJson.git
    GIT_TAG        v6.21.2
)
FetchContent_MakeAvailable(ArduinoJson)

# Compiler flags
add_compile_options(-Wall -Wtype-limits -Wno-format -Wno-unused-function)

# Link flags for section pruning
add_compile_options(-fdata-sections -ffunction-sections)
add_link_options(-Wl,--gc-sections)

# Proto compilation
include(compile_proto.cmake)
compile_proto()

# TinyUSB
set(PICO_TINYUSB_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/tinyusb")

# Subdirectories
add_subdirectory(lib)

# Debug build stack usage
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-fstack-usage -Wstack-usage=500)
  add_compile_definitions(PICO_USE_STACK_GUARDS=1)
endif()

add_compile_definitions(PICO_STACK_SIZE=0x1000)

# Main firmware build
add_executable(${PROJECT_NAME}
  src/main.cpp
  src/gp2040.cpp
  src/gp2040aux.cpp
  # ... Add all other source files here as needed
  ${PROTO_OUTPUT_DIR}/enums.pb.c
  ${PROTO_OUTPUT_DIR}/config.pb.c
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}_${CMAKE_PROJECT_VERSION}_${GP2040_BOARDCONFIG})

# Program metadata
pico_set_program_name(${PROJECT_NAME} "GP2040-CE")
pico_set_program_version(${PROJECT_NAME} ${GIT_REPO_VERSION})

# Link libraries
target_link_libraries(${PROJECT_NAME}
  pico_stdlib
  pico_bootsel_via_double_reset
  tinyusb_host
  tinyusb_pico_pio_usb
  ArduinoJson
  # Add other libraries here
)

# Includes
target_include_directories(${PROJECT_NAME} PUBLIC
  headers
  configs/${GP2040_BOARDCONFIG}
  ${PROTO_OUTPUT_DIR}
  ${CMAKE_BINARY_DIR}/headers
)

# Definitions
target_compile_definitions(${PROJECT_NAME} PUBLIC
  BOARD_CONFIG_FILE_NAME="$<TARGET_FILE_BASE_NAME:${PROJECT_NAME}>"
  GP2040_BOARDCONFIG="${GP2040_BOARDCONFIG}"
)

# Output formatting
pico_add_extra_outputs(${PROJECT_NAME})

# Install artifacts
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_${CMAKE_PROJECT_VERSION}_${GP2040_BOARDCONFIG}.uf2
  DESTINATION .
)

# Load optional custom modules
if (NOT DEFINED ENV{CI} AND EXISTS ${CMAKE_SOURCE_DIR}/modules/Custom.cmake)
  include(${CMAKE_SOURCE_DIR}/modules/Custom.cmake)
endif()
