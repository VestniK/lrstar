# TODO check if it's possible to symplify this file code using EXPORTED targets cmake feature.
get_target_property(BIN_PATH lrstar LOCATION)
get_filename_component(BIN_NAME ${BIN_PATH} NAME)
set(LRSTAR_PATH ${CMAKE_INSTALL_PREFIX}/bin/${BIN_NAME})

get_target_property(BIN_PATH dfastar LOCATION)
get_filename_component(BIN_NAME ${BIN_PATH} NAME)
set(DFASTAR_PATH ${CMAKE_INSTALL_PREFIX}/bin/${BIN_NAME})

set(SKELS_PATH ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/skels)

configure_file(lrstar-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/lrstar-config.cmake @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lrstar-config.cmake
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/${PROJECT_NAME}"
)
