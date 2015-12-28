set(SKELS_PATH ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/skels)

configure_file(lrstar-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/lrstar-config.cmake @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lrstar-config.cmake
  DESTINATION lib/cmake/lrstar
)
install(FILES lrstar-macro.cmake
  DESTINATION lib/cmake/lrstar
)
