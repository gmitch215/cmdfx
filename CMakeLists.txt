cmake_minimum_required(VERSION 3.30)

project(cmdfx 
    LANGUAGES C 
    VERSION 0.1.0
    DESCRIPTION "Lightweight game engine for your terminal"
    HOMEPAGE_URL "https://github.com/gmitch215/cmdfx"
)
set(CMAKE_C_STANDARD 11)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Sources
file(GLOB COMMON_SRC ${PROJECT_SOURCE_DIR}/src/common/*.c)
if (WIN32)
    file(GLOB PLATFORM_SRC ${PROJECT_SOURCE_DIR}/src/windows/*.c)
else()
    file(GLOB PLATFORM_SRC ${PROJECT_SOURCE_DIR}/src/posix/*.c)
endif()

add_library(cmdfx SHARED ${COMMON_SRC} ${PLATFORM_SRC})
target_include_directories(cmdfx PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(cmdfx PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin
)

# Testing
enable_testing()
add_subdirectory(test)

# Documentation
option(DOCS_CMDFX "Build API documentation with Doxygen" ON)

find_package(Doxygen)
if (DOXYGEN_FOUND AND DOCS_CMDFX)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/build/Doxyfile.in)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(docs ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()

# Installation
option(INSTALL_CMDFX "Install ${PROJECT_NAME}" ON)

if (INSTALL_CMDFX)
    include(GNUInstallDirs)
   
    install(TARGETS cmdfx
        EXPORT cmdfx-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file("ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )
    configure_package_config_file(
            "${PROJECT_SOURCE_DIR}/cmake/Config.cmake.in"
            "${PROJECT_BINARY_DIR}/Config.cmake"
            INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
    )

    install(EXPORT cmdfx-targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
    )
    install(FILES "${PROJECT_BINARY_DIR}/Config.cmake"
                 "${PROJECT_BINARY_DIR}/ConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
    )
    install(DIRECTORY
            "${PROJECT_SOURCE_DIR}/include/cmdfx"
            "${PROJECT_SOURCE_DIR}/include/"
            DESTINATION include
    )
endif()

# Packaging
option(PACKAGE_CMDFX "Package ${PROJECT_NAME}" ON)

if (PACKAGE_CMDFX)
    set(ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

    if (ARCHITECTURE STREQUAL "AMD64")
        set(ARCHITECTURE "x64")
    endif()

    set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/build/package")

    set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
    set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
    set(CPACK_PACKAGE_VENDOR "Gregory Mitchell")
    set(CPACK_PACKAGE_CONTACT "Gregory Mitchell")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

    if (WIN32)
        set(CPACK_GENERATOR "ZIP;NSIS;TGZ;WIX")
        set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-windows-${ARCHITECTURE}")

        set(CPACK_NSIS_DISPLAY_NAME ${PROJECT_NAME})
        set(CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}/assets/cmdfx.ico")
        set(CPACK_NSIS_MUI_UNIICON "${PROJECT_SOURCE_DIR}/assets/cmdfx.ico")

        set(CPACK_WIX_VERSION 4)
        set(CPACK_WIX_PRODUCT_ICON "${PROJECT_SOURCE_DIR}/assets/cmdfx.ico")
        set(CPACK_WIX_LICENSE_RTF "${PROJECT_SOURCE_DIR}/LICENSE.rtf")
        set(CPACK_WIX_UPGRADE_GUID "42937a72-0b91-4a57-8f45-60d84df05e66")
    elseif(APPLE)
        set(CPACK_GENERATOR "TGZ;ZIP;productbuild;Bundle")
        set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-macOS-${ARCHITECTURE}")

        set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/assets/cmdfx.icns")
    elseif(UNIX)
        set(CPACK_GENERATOR "TGZ;DEB;RPM")
        set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-linux-${ARCHITECTURE}")
    endif()

    include(CPack)
endif()