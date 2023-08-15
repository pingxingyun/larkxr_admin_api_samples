# linking
set(LINK_DIR 
    # LINK_DIR
    ${PROJECT_SOURCE_DIR}/third_party/libhv/lib/windows/x64/
)

link_directories(${LINK_DIR})

# windows
add_definitions(-DWIN32_LEAN_AND_MEAN)
# static lib hv
add_definitions(-DHV_STATICLIB)
# export dll
add_definitions(-DLARK_XR_EXPORT)

# source dir
set(src_dir ${CMAKE_CURRENT_SOURCE_DIR})

#设置库文件的输出目录
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs/larkxr_adminapi/lib)
    
add_library(larkxr_adminapi
    SHARED
    # WIN32
    larkxr_adminapi.h
    larkxr_adminapi.cpp
    slice_upload_client.h
    slice_upload_client.cpp
    utils.h
)

# include
target_include_directories(larkxr_adminapi PRIVATE
    ${src_dir}
    ${PROJECT_SOURCE_DIR}/third_party/libhv/include/
)

target_link_libraries(larkxr_adminapi
    hv_static
)