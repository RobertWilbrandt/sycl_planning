find_package(Doxygen REQUIRED)

set(DOXYGEN_CONF ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
configure_file(Doxyfile.in ${DOXYGEN_CONF})

add_custom_target(doc_doxygen
  ALL
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONF}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Build documentation using doxygen"
  VERBATIM
)
