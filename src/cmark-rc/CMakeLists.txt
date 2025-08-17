# In order to maintain compatibility with older platforms which may not have a
# recent version of CMake (i.e. are running CMake <3.3), we cannot simply use
# the `add_compile_options` with a generator expression.  This uses the
# `target_compile_options` with `PRIVATE` to add the flags only to the targets
# so that CMark may be used in projects with non-C languages.
function(cmark_add_compile_options target)
    if(MSVC)
        target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS)
    else()
        target_compile_options(
            ${target}
            PRIVATE
                -Wall
                -Wextra
                -pedantic
                $<$<COMPILE_LANGUAGE:C>:-Wstrict-prototypes>
        )
    endif()
    if(CMAKE_BUILD_TYPE STREQUAL Profile)
        target_compile_options(${target} PRIVATE -pg)
    endif()
    if(CMAKE_BUILD_TYPE STREQUAL Ubsan)
        target_compile_options(${target} PRIVATE -fsanitize=undefined)
    endif()
    if(CMARK_LIB_FUZZER)
        if(target MATCHES fuzz)
            target_compile_options(${target} PRIVATE -fsanitize=fuzzer)
            target_link_options(${target} PRIVATE -fsanitize=fuzzer)
        else()
            target_compile_options(${target} PRIVATE -fsanitize=fuzzer-no-link)
            target_link_options(${target} PRIVATE -fsanitize=fuzzer-no-link)
        endif()
    endif()
endfunction()

# Check integrity of node structure when compiled as debug
add_compile_options($<$<CONFIG:Debug>:-DCMARK_DEBUG_NODES>)

configure_file(
    cmark-rc_version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmark-rc_version.h
)
# configure_file(libcmark.pc.in
#   ${CMAKE_CURRENT_BINARY_DIR}/libcmark-rc.pc
#   @ONLY)

add_library(
    cmark-rc
    blocks.c
    buffer.c
    cmark.c
    cmark-rc.h
    cmark_ctype.c
    commonmark.c
    houdini_href_e.c
    houdini_html_e.c
    houdini_html_u.c
    html.c
    inlines.c
    iterator.c
    latex.c
    man.c
    node.c
    references.c
    render.c
    scanners.c
    scanners.re
    utf8.c
    xml.c
)
cmark_add_compile_options(cmark-rc)
set_target_properties(
    cmark-rc
    PROPERTIES
        OUTPUT_NAME "cmark-rc"
        # Avoid name clash between PROGRAM and LIBRARY pdb files.
        PDB_NAME libcmark-rc
        POSITION_INDEPENDENT_CODE YES
        # Include minor version and patch level in soname for now.
        SOVERSION ${PROJECT_VERSION}
        VERSION ${PROJECT_VERSION}
)
if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(cmark-rc PUBLIC CMARK_STATIC_DEFINE)
endif()
target_include_directories(
    cmark-rc
    INTERFACE
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

generate_export_header(cmark-rc BASE_NAME cmark-rc)

# FIXME(compnerd) this should be removed, but exists solely to allow a migration
# path for OSS Fuzz.
add_custom_target(cmark_static DEPENDS cmark-rc)

# add_executable(cmark_exe
#   main.c)
# cmark_add_compile_options(cmark_exe)
# set_target_properties(cmark_exe PROPERTIES
#   OUTPUT_NAME "cmark-rc"
#   INSTALL_RPATH "${Base_rpath}")
# target_link_libraries(cmark_exe PRIVATE
#   cmark-rc)

# install(TARGETS cmark_exe cmark-rc
#   EXPORT cmark-rc-targets
#   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#   )

# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libcmark-rc.pc
#   DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

# install(FILES
#   cmark-rc.h
#   ${CMAKE_CURRENT_BINARY_DIR}/cmark-rc_export.h
#   ${CMAKE_CURRENT_BINARY_DIR}/cmark-rc_version.h
#   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#   )

# # generate cmark-config.cmake and cmark-config-version.cmake files
# configure_package_config_file(
#   "cmarkConfig.cmake.in"
#   "${CMAKE_CURRENT_BINARY_DIR}/generated/cmark-rc-config.cmake"
#   INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cmark")
# write_basic_package_version_file(
#   "${CMAKE_CURRENT_BINARY_DIR}/generated/cmark-rc-config-version.cmake"
#   VERSION ${PROJECT_VERSION}
#   COMPATIBILITY SameMajorVersion)
# # install config and version file
# install(
#   FILES "${CMAKE_CURRENT_BINARY_DIR}/generated/cmark-rc-config.cmake"
#         "${CMAKE_CURRENT_BINARY_DIR}/generated/cmark-rc-config-version.cmake"
#   DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cmark-rc"
# )
# # install targets file
# install(
#   EXPORT "cmark-rc-targets"
#   NAMESPACE "cmark-rc::"
#   DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/cmark-rc"
# )
set_target_properties(
    cmark-rc
    PROPERTIES OUTPUT_NAME cmark-rc-copy VERSION "0.31.0" SOVERSION "0"
)

install(
    TARGETS cmark-rc ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
    LIBRARY NAMELINK_SKIP
)
