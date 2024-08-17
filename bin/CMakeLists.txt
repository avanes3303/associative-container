add_executable(${PROJECT_NAME} main.cpp
        ../lib/bst.h
        ../lib/bst.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE BST)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})