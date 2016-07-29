################################################################################
## INSTALL
################################################################################
set(CMAKE_INSTALL_SYSCONFDIR_ARANGO "${CMAKE_INSTALL_SYSCONFDIR}/arangodb3" CACHE PATH "read-only single-machine data (etc)")
set(CMAKE_INSTALL_FULL_SYSCONFDIR_ARANGO "${CMAKE_INSTALL_FULL_SYSCONFDIR}/arangodb3" CACHE PATH "read-only single-machine data (etc)")
set(CMAKE_INSTALL_FULL_DATAROOTDIR_ARANGO "${CMAKE_INSTALL_DATAROOTDIR}/arangodb3" CACHE PATH "read-only data (share)")
set(CMAKE_INSTALL_DATAROOTDIR_ARANGO "${CMAKE_INSTALL_FULL_DATAROOTDIR}/arangodb3" CACHE PATH "read-only data (share)")

# Global macros ----------------------------------------------------------------
macro (generate_root_config name)
  FILE(READ ${PROJECT_SOURCE_DIR}/etc/arangodb3/${name}.conf.in FileContent)
  STRING(REPLACE "@PKGDATADIR@" "@ROOTDIR@/share/arangodb3"
    FileContent "${FileContent}") 
  STRING(REPLACE "@LOCALSTATEDIR@" "@ROOTDIR@/var"
    FileContent "${FileContent}")
  STRING(REPLACE "@SBINDIR@" "@ROOTDIR@/bin"
    FileContent "${FileContent}")
  STRING(REPLACE "@LIBEXECDIR@/arangodb3" "@ROOTDIR@/bin"
    FileContent "${FileContent}")
  STRING(REPLACE "@SYSCONFDIR@" "@ROOTDIR@/${CMAKE_INSTALL_SYSCONFDIR_ARANGO}" 
    FileContent "${FileContent}")
  if (MSVC)
    STRING(REPLACE "@PROGRAM_SUFFIX@" ".exe"
      FileContent "${FileContent}")
    STRING(REGEX REPLACE "[\r\n]file =" "\n# file =" 
      FileContent "${FileContent}")
  endif ()
  FILE(WRITE ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_SYSCONFDIR_ARANGO}/${name}.conf "${FileContent}")
endmacro ()

#  generates config file using the configured paths ----------------------------
macro (generate_path_config name)
  FILE(READ "${PROJECT_SOURCE_DIR}/etc/arangodb3/${name}.conf.in" FileContent)
  STRING(REPLACE "@PKGDATADIR@" "${CMAKE_INSTALL_DATAROOTDIR_ARANGO}"
    FileContent "${FileContent}")
  STRING(REPLACE "@LOCALSTATEDIR@" "${CMAKE_INSTALL_FULL_LOCALSTATEDIR}" 
    FileContent "${FileContent}")
  FILE(WRITE ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_SYSCONFDIR_ARANGO}/${name}.conf "${FileContent}")
endmacro ()

# installs a config file -------------------------------------------------------
macro (install_config name)
  if (MSVC OR (DARWIN AND NOT HOMEBREW))
    generate_root_config(${name})
  else ()
    generate_path_config(${name})
  endif ()
  install(
    FILES ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_SYSCONFDIR_ARANGO}/${name}.conf
    DESTINATION ${CMAKE_INSTALL_FULL_SYSCONFDIR_ARANGO})
endmacro ()

# installs a readme file converting EOL ----------------------------------------
macro (install_readme input output)
  set(where "${CMAKE_INSTALL_FULL_DOCDIR}")
  if (MSVC)
    # the windows installer contains the readme in the top level directory:
    set(where ".")
  endif ()

  set(PKG_VERSION "")
  if (${USE_VERSION_IN_LICENSEDIR})
    set(PKG_VERSION "-${ARANGODB_VERSION}")
  endif ()

  FILE(READ ${PROJECT_SOURCE_DIR}/${input} FileContent)
  STRING(REPLACE "\r" "" FileContent "${FileContent}")
  if (MSVC)
    STRING(REPLACE "\n" "\r\n" FileContent "${FileContent}")
  endif ()
  FILE(WRITE ${PROJECT_BINARY_DIR}/${output} "${FileContent}")
  install(
    FILES ${PROJECT_BINARY_DIR}/${output}
    DESTINATION ${where}${PKG_VERSION})
endmacro ()

# installs a link to an executable ---------------------------------------------
macro (install_command_alias name where alias)
  if (MSVC)
    add_custom_command(
      TARGET ${name}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${name}>
	      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/${alias}.exe)
    install(
      PROGRAMS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/${alias}.exe
      DESTINATION ${where})
  else ()
    add_custom_command(
      TARGET ${name}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E create_symlink ${name}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${alias}) 
    install(
      PROGRAMS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${alias}
      DESTINATION ${where})
  endif ()
endmacro ()
