add_executable(HydrogenDemo demo.cpp)
target_compile_features(HydrogenDemo PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(HydrogenDemo PRIVATE /W4 /WX)
else()
    target_compile_options(HydrogenDemo PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(HydrogenDemo PRIVATE Hydrogen glfw)

set(ASSET_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSET_DEST_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")

add_custom_command(
    TARGET HydrogenDemo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${ASSET_SOURCE_DIR} ${ASSET_DEST_DIR}
    DEPENDS ${ASSET_DEST_DIR}
    COMMENT "symbolic link resources folder from ${ASSET_SOURCE_DIR} => ${ASSET_DEST_DIR}"
)
