project(lrstar)
cmake_minimum_required(VERSION 2.8.0)

if (WIN32)
  add_definitions(WINDOWS)
else()
  add_definitions(-Wno-write-strings) # TODO: fix thise warnings instead of disabling
  add_definitions(-std=c++14)
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SRC
  ComGenerate.cpp
  ComGlobal.cpp
  ComMain.cpp
  LGActions.cpp
  LGAddExtra.cpp
  LGBuildStates.cpp
  LGCheckGrammar.cpp
  LGComputeLA.cpp
  LGCreateTables.cpp
  LGGenerateCode.cpp
  LGLexer.cpp
  LGMain.cpp
  LGOptimizeStates.cpp
  LGParser.cpp
  LGPrintGrammar.cpp
  LGPrintStates.cpp
  PGActions.cpp
  PGBuildLR0.cpp
  PGBuildLR1.cpp
  PGCheckGrammar.cpp
  PGComputeLA.cpp
  PGCreateTables.cpp
  PGLexer.cpp
  PGMain.cpp
  PGOptimizeStates.cpp
  PGParser.cpp
  PGPrintHtml.cpp
  PGPrintStates.cpp
)

add_executable(lrstar ${SRC})
set_target_properties(lrstar PROPERTIES COMPILE_DEFINITIONS LRSTAR)

add_executable(dfastar ${SRC})
set_target_properties(dfastar PROPERTIES COMPILE_DEFINITIONS DFASTAR)

install(TARGETS lrstar dfastar EXPORT utils RUNTIME DESTINATION bin)
install(EXPORT utils NAMESPACE lrstar:: DESTINATION lib/cmake/lrstar)

add_subdirectory(skels)
add_subdirectory(cmake)
