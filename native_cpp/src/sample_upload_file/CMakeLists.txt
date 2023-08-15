# linking
set(LINK_DIR 
    # LINK_DIR
    ${PROJECT_SOURCE_DIR}/third_party/libhv/lib/windows/x64/
    ${CMAKE_BINARY_DIR}/libs/larkxr_adminapi/lib
)

link_directories(${LINK_DIR})

add_definitions(-DHV_STATICLIB)

# source dir
set(src_dir ${CMAKE_CURRENT_SOURCE_DIR})

# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    
add_executable(test_api_upload_file 
    # WIN32
    main.cpp
)

# include
target_include_directories(test_api_upload_file PRIVATE
    ${src_dir}
    ${PROJECT_SOURCE_DIR}/src/lib_larkxradminapi/
    ${PROJECT_SOURCE_DIR}/third_party/libhv/include/
)

target_link_libraries(test_api_upload_file
    larkxr_adminapi
)