set(SKELS
  lexer.h.skl
  lexer.cpp.skl
  parser.h.skl
  parser.cpp.skl
)

install(FILES ${SKELS} DESTINATION share/${PROJECT_NAME}/skels/pbmann)
