# Usage: lrstar_grammar(output name SKEL|CUSTOM_SKEL skelpath)
# Generates C/C++ sources form lrstar and dfastar grammar descritpion.
# Generated sources are stored in the variable name passed with output
# parameter. The name parameter is usd as grammar name and there
# should be ${name}.lgr and ${name}.grm input files in the current
# sources dir.
macro(lrstar_grammar outVar grammarName skelType skelPath)
  if ("${skelType}" STREQUAL "SKEL")
    set(__skel_full_path ${LRSTAR_SKELS_PATH}/${skelPath})
  elseif("${skelType}" STREQUAL "CUSTOM_SKEL")
    set(__skel_full_path ${skelPath})
  endif()
  add_custom_command(
    OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}parser.h
      ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}parser.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}lexer.h
      ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}lexer.cpp
    DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/${grammarName}.grm
      ${CMAKE_CURRENT_SOURCE_DIR}/${grammarName}.lgr
      ${__skel_full_path}/parser.h.skl
      ${__skel_full_path}/parser.cpp.skl
      ${__skel_full_path}/lexer.cpp.skl
      ${__skel_full_path}/lexer.h.skl
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND lrstar::lrstar ${CMAKE_CURRENT_SOURCE_DIR}/${grammarName}.grm ${__skel_full_path}/parser.h.skl ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}parser.h ${__skel_full_path}/parser.cpp.skl ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}parser.cpp -exp=2
    COMMAND lrstar::dfastar ${CMAKE_CURRENT_SOURCE_DIR}/${grammarName}.lgr ${__skel_full_path}/lexer.h.skl ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}lexer.h ${__skel_full_path}/lexer.cpp.skl ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}lexer.cpp -col
  )
  set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}parser.h
    ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}parser.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}lexer.h
    ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}lexer.cpp
    PROPERTIES GENERATED 1
  )
  set(${outVar}
    ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}parser.h
    ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}parser.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}lexer.h
    ${CMAKE_CURRENT_BINARY_DIR}/${grammarName}lexer.cpp
  )
endmacro()
