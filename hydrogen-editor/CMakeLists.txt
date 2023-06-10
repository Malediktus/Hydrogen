add_executable(HydrogenEditor
    include/Panels/AssetNavigatorPanel.hpp
    include/Panel.hpp
    src/EditorApp.cpp
)

target_compile_features(HydrogenEditor PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(HydrogenEditor PRIVATE /W4 /WX)
else()
    target_compile_options(HydrogenEditor PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(HydrogenEditor PRIVATE Hydrogen glfw)
target_include_directories(HydrogenEditor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(ASSET_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSET_DEST_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")

add_custom_command(
    TARGET HydrogenEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${ASSET_SOURCE_DIR} ${ASSET_DEST_DIR}
    DEPENDS ${ASSET_DEST_DIR}
    COMMENT "symbolic link resources folder from ${ASSET_SOURCE_DIR} => ${ASSET_DEST_DIR}"
)
