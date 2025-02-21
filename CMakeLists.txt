cmake_minimum_required(VERSION 3.30)

set(PROJECT_VERSION "0.1.2")
project(cmdfx 
    LANGUAGES C 
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "Lightweight game engine for your terminal"
    HOMEPAGE_URL "https://github.com/gmitch215/cmdfx"
)
set(PROJECT_DOCUMENTATION "https://gmitch215.github.io/cmdfx")

set(CMAKE_C_STANDARD 11)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Sources
file(GLOB_RECURSE COMMON_SRC ${PROJECT_SOURCE_DIR}/src/common/*.c)
if (WIN32)
    file(GLOB_RECURSE PLATFORM_SRC ${PROJECT_SOURCE_DIR}/src/windows/*.c)
else()
    file(GLOB_RECURSE PLATFORM_SRC ${PROJECT_SOURCE_DIR}/src/posix/*.c)

    if (APPLE)
        file(GLOB_RECURSE OS_SRC ${PROJECT_SOURCE_DIR}/src/macos/*.c)
    elseif (LINUX)
        file(GLOB_RECURSE OS_SRC ${PROJECT_SOURCE_DIR}/src/linux/*.c)
    endif()
endif()

add_library(cmdfx SHARED ${COMMON_SRC} ${PLATFORM_SRC} ${OS_SRC})
target_include_directories(cmdfx PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(cmdfx PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin
)

# Libraries
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

    target_link_libraries(cmdfx PRIVATE m) # Math
endif()

if (APPLE)
    target_link_libraries(cmdfx PRIVATE
        "-framework CoreGraphics"
        "-framework CoreFoundation"
        "-framework ApplicationServices"
        "-framework Carbon"
        "-framework IOKit"
    )
endif()

# Testing
enable_testing()

if (WIN32)
    add_subdirectory(test)
else()
    add_subdirectory(test EXCLUDE_FROM_ALL)
endif()

# Documentation
option(DOCS_CMDFX "Build API documentation with Doxygen" ON)

if (DOCS_CMDFX)
    find_package(Doxygen)

    if (DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/build/Doxyfile.in)
    
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    
        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
    endif()
endif()

# Installation
option(INSTALL_CMDFX "Install ${PROJECT_NAME}" ON)

if(WIN32)
    if(CMAKE_SIZEOF_VOID_P MATCHES "8")
        set(CMAKE_INSTALL_PREFIX "C:/Program Files/${CMAKE_PROJECT_NAME}")
    else()
        set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/${CMAKE_PROJECT_NAME}")
    endif()
elseif(APPLE)
    set(CMAKE_INSTALL_PREFIX "/usr/local")
else()
    set(CMAKE_INSTALL_PREFIX "/usr")
endif()

if (INSTALL_CMDFX)
    include(GNUInstallDirs)
   
    install(TARGETS cmdfx
        EXPORT cmdfx-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT library
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

    # Library Components

    ## Core Library
    install(EXPORT cmdfx-targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
        COMPONENT library
    )
    install(FILES "${PROJECT_BINARY_DIR}/Config.cmake"
                 "${PROJECT_BINARY_DIR}/ConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
            COMPONENT library
    )

    install(FILES "${PROJECT_SOURCE_DIR}/include/cmdfx.h"
            DESTINATION include
            COMPONENT library
    )
    install(DIRECTORY
            "${PROJECT_SOURCE_DIR}/include/cmdfx/core/"
            DESTINATION include/cmdfx/core
            COMPONENT library
    )

    ## Physics Engine
    install(DIRECTORY
            "${PROJECT_SOURCE_DIR}/include/cmdfx/physics/"
            DESTINATION include/cmdfx/physics
            COMPONENT library
    )
    
    # Documentation Components
    if (DOCS_CMDFX)
        if (EXISTS "${PROJECT_SOURCE_DIR}/build/docs/html")
            install(DIRECTORY
                "${PROJECT_SOURCE_DIR}/build/docs/html"
                DESTINATION ${CMAKE_INSTALL_DOCDIR}
                COMPONENT htmlDocs
            )
        endif()

        if (EXISTS "${PROJECT_SOURCE_DIR}/build/docs/latex")
            install(DIRECTORY
                "${PROJECT_SOURCE_DIR}/build/docs/latex"
                DESTINATION ${CMAKE_INSTALL_DOCDIR}
                COMPONENT latexDocs
            )
        endif()
        
        install(FILES
            "${PROJECT_SOURCE_DIR}/README.md"
            "${PROJECT_SOURCE_DIR}/INSTALLER_README.txt"
            "${PROJECT_SOURCE_DIR}/LICENSE"
            "${PROJECT_SOURCE_DIR}/LICENSE.rtf"
            "${PROJECT_SOURCE_DIR}/CONTRIBUTING.md"
            DESTINATION ${CMAKE_INSTALL_DOCDIR}
            COMPONENT attribution
        )
    endif()
endif()

# Packaging
option(PACKAGE_CMDFX "Package ${PROJECT_NAME}" ON)

if (PACKAGE_CMDFX)
    set(ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

    if (ARCHITECTURE STREQUAL "AMD64" OR ARCHITECTURE STREQUAL "x86_64")
        set(ARCHITECTURE "x64")
    endif()

    if (ARCHITECTURE STREQUAL "aarch64")
        set(ARCHITECTURE "arm64")
    endif()

    set(CPACK_THREADS 0) # Use all available threads
    set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/build/package")
    set(CPACK_PACKAGE_CHECKSUM SHA256)

    set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
    set(CPACK_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_PACKAGE_HOMEPAGE_URL ${PROJECT_HOMEPAGE_URL})
    set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
    set(CPACK_PACKAGE_VENDOR "Gregory Mitchell")
    set(CPACK_PACKAGE_CONTACT "Gregory Mitchell")

    set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/INSTALLER_README.txt")
    set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

    if (WIN32)
        set(CPACK_GENERATOR "ZIP;NSIS;TGZ;WIX")

        set(CPACK_SYSTEM_NAME "windows-${ARCHITECTURE}")

        set(CPACK_NSIS_DISPLAY_NAME ${PROJECT_NAME})
        set(CPACK_NSIS_DISPLAY_NAME_SET ON)

        set(CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_SOURCE_DIR}/assets/cmdfx.ico")
        set(CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}/assets/cmdfx.ico")
        set(CPACK_NSIS_MUI_UNIICON "${PROJECT_SOURCE_DIR}/assets/cmdfx.ico")
        set(CPACK_NSIS_MODIFY_PATH ON)
        set(CPACK_NSIS_MENU_LINKS "GitHub" ${PROJECT_HOMEPAGE_URL} "Documentation" ${PROJECT_DOCUMENTATION})

        set(CPACK_WIX_VERSION 4)
        set(CPACK_WIX_PRODUCT_ICON "${PROJECT_SOURCE_DIR}/assets/cmdfx.ico")
        set(CPACK_WIX_LICENSE_RTF "${PROJECT_SOURCE_DIR}/LICENSE.rtf")
        set(CPACK_WIX_UPGRADE_GUID "42937a72-0b91-4a57-8f45-60d84df05e66")
    elseif(APPLE)
        set(CPACK_GENERATOR "TGZ;ZIP;productbuild")

        set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")
        set(CPACK_SYSTEM_NAME "macOS-${ARCHITECTURE}")

        set(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}/assets/cmdfx.icns")
        set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.rtf")

        set(CPACK_PRODUCTBUILD_IDENTIFIER "dev.gmitch215.${PROJECT_NAME}")
    elseif(UNIX)
        set(CPACK_GENERATOR "TGZ;TXZ;DEB;RPM")

        set(CPACK_SYSTEM_NAME "linux-${ARCHITECTURE}")
        set(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}/assets/cmdfx.png")
    endif()

    include(CPack)

    cpack_add_component(library DISPLAY_NAME "cmdfx Library" DESCRIPTION "The core library for cmdfx" REQUIRED ON)
    cpack_add_component(attribution DISPLAY_NAME "Attribution" DESCRIPTION "Attribution files from cmdfx" REQUIRED ON)

    cpack_add_component_group(docs DISPLAY_NAME "Documentation" DESCRIPTION "Documentation for cmdfx" EXPANDED ON)
    cpack_add_component(htmlDocs DISPLAY_NAME "HTML Documentation" DESCRIPTION "HTML documentation for cmdfx" GROUP docs DISABLED ON)
    cpack_add_component(latexDocs DISPLAY_NAME "LaTeX Documentation" DESCRIPTION "LaTeX documentation for cmdfx" GROUP docs DISABLED ON)
endif()