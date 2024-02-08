cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME gameOfLife)
set(SOURCE main.cpp)
set(LIBRARY_DIR libs)
set(LIBRARY Cell CommandState ConsoleParser Field FileParser Game GameState Message)

project(${PROJECT_NAME} CXX)

project(${PROJECT_NAME} ${SOURCE})

foreach(LIBRARY ${LIBRARY}) 
	add_subdirectory(${LIBRARY_DIR} ${LIBRARY})
	message(STATUS "Add subdirectory ${LIBRARY_DIR}/${LIBRARY}")
endforeach(LIBRARY)

target_link_libraries(${PROJECT_NAME} ${LIBRARY})