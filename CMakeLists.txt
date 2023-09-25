cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME top_adam)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(CMAKE_BUILD_TYPE Debug)

project(${PROJECT_NAME})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include/top_adam)

include_directories(${INCLUDE_DIRS})

file(GLOB_RECURSE LIB_SRC_FILES include/*/*.c)

add_library(${PROJECT_NAME}_LIB STATIC ${LIB_SRC_FILES})

install(TARGETS ${PROJECT_NAME}_LIB DESTINATION lib)

file(GLOB_RECURSE LIB_INC_FILES include/*/*.h)
install(FILES ${LIB_INC_FILES} DESTINATION include)

set(SOURCE_FILES src/main.c)
set(TEST_FILES test/test.c)

add_executable(top_adam_main ${SOURCE_FILES})
add_executable(top_adam_test ${TEST_FILES})

target_link_libraries(top_adam_main ${PROJECT_NAME}_LIB)
target_link_libraries(top_adam_test ${PROJECT_NAME}_LIB)

file(COPY ${PROJECT_SOURCE_DIR}/test_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})