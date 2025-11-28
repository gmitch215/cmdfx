cmake_minimum_required(VERSION 3.30)

set(PROJECT_VERSION "0.3.3")
project(cmdfx
    LANGUAGES C CXX
    VERSION ${PROJECT_VERSION}
    DESCRIPTION "Lightweight game engine for your terminal"
    HOMEPAGE_URL "https://github.com/gmitch215/cmdfx"
)
set(PROJECT_DOCUMENTATION "https://gmitch215.github.io/cmdfx")

set(CMAKE_C_STANDARD 17)

# Sources
file(GLOB_RECURSE COMMON_SRC ${PROJECT_SOURCE_DIR}/src/common/*.c)
if (WIN32)
    file(GLOB_RECURSE PLATFORM_SRC ${PROJECT_SOURCE_DIR}/src/windows/*.c)

    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
else()
    file(GLOB_RECURSE PLATFORM_SRC ${PROJECT_SOURCE_DIR}/src/posix/*.c)

    if (APPLE)
        file(GLOB_RECURSE OS_SRC ${PROJECT_SOURCE_DIR}/src/macos/*.c)
    elseif (LINUX)
        file(GLOB_RECURSE OS_SRC ${PROJECT_SOURCE_DIR}/src/linux/*.c)
    endif()
endif()

add_library(cmdfx STATIC ${COMMON_SRC} ${PLATFORM_SRC} ${OS_SRC})
target_include_directories(cmdfx PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(cmdfx PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Libraries
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

    target_link_libraries(cmdfx PRIVATE m) # Math
endif()

if (WIN32)
    target_link_libraries(cmdfx PRIVATE winmm) # Windows Multimedia API
elseif (APPLE)
    target_link_libraries(cmdfx PRIVATE
        "-framework CoreGraphics"
        "-framework CoreFoundation"
        "-framework ApplicationServices"
        "-framework Carbon"
        "-framework IOKit"
        "-framework CoreAudio"
        "-framework AudioToolbox"
        "-framework AudioUnit"
    )
elseif (LINUX)
    # Find ALSA for Linux sound support
    find_package(PkgConfig)
    if (PkgConfig_FOUND)
        pkg_check_modules(ALSA alsa)
        if (ALSA_FOUND)
            target_link_libraries(cmdfx PRIVATE ${ALSA_LIBRARIES})
            target_include_directories(cmdfx PRIVATE ${ALSA_INCLUDE_DIRS})
            target_compile_definitions(cmdfx PRIVATE ${ALSA_CFLAGS_OTHER})
        else()
            message(WARNING "ALSA not found. Sound support will be limited on Linux.")
        endif()
    endif()
endif()

# Testing
option(TEST_CMDFX "Build tests for ${PROJECT_NAME}" ON)
if (TEST_CMDFX)
    enable_testing()
    add_subdirectory(test)
endif()

# Samples
option(SAMPLES_CMDFX "Build samples for ${PROJECT_NAME}" OFF)
if (SAMPLES_CMDFX)
    file(GLOB SAMPLES_SRC ${PROJECT_SOURCE_DIR}/samples/*.c)

    foreach(SAMPLE_SRC ${SAMPLES_SRC})
        get_filename_component(SAMPLE_NAME ${SAMPLE_SRC} NAME_WE)
        add_executable(${SAMPLE_NAME} ${SAMPLE_SRC})
        target_link_libraries(${SAMPLE_NAME} PRIVATE cmdfx)
        set_target_properties(${SAMPLE_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples
        )
    endforeach()
endif()

# Documentation
option(DOCS_CMDFX "Build API documentation with Doxygen" ON)

if (DOCS_CMDFX)
    find_package(Doxygen)

    if (DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.in)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
    else()
        add_custom_target(docs
            COMMAND ${CMAKE_COMMAND} -E echo "Doxygen not found, skipping documentation generation"
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
        EXPORT cmdfxTargets
        COMPONENT library
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION include
    )

    export(EXPORT cmdfxTargets
        FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
    )
    install(EXPORT cmdfxTargets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}
        COMPONENT library
    )

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )
    configure_package_config_file(
            "${PROJECT_SOURCE_DIR}/cmake/Config.cmake.in"
            "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
            INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
    )

    install(FILES
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}
        COMPONENT library
    )

    # Library Components
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/"
            DESTINATION include
            COMPONENT library
    )

    # Documentation Components
    if (DOCS_CMDFX)
        if (EXISTS "${CMAKE_BINARY_DIR}/docs/html")
            install(DIRECTORY
                "${CMAKE_BINARY_DIR}/docs/html"
                DESTINATION ${CMAKE_INSTALL_DOCDIR}
                COMPONENT htmlDocs
            )
        endif()

        if (EXISTS "${CMAKE_BINARY_DIR}/docs/latex")
            install(DIRECTORY
                "${CMAKE_BINARY_DIR}/docs/latex"
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
        install(DIRECTORY
            "${PROJECT_SOURCE_DIR}/assets/"
            DESTINATION ${CMAKE_INSTALL_DOCDIR}/assets
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
    set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/package")
    set(CPACK_PACKAGE_CHECKSUM SHA256)

    if (DEFINED PROJECT_VERSION_SUFFIX)
        set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}-${PROJECT_VERSION_SUFFIX}")
    else()
        set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
    endif()

    set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
    set(CPACK_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})
    set(CPACK_PACKAGE_HOMEPAGE_URL ${PROJECT_HOMEPAGE_URL})
    set(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
    set(CPACK_PACKAGE_VENDOR "Gregory Mitchell")
    set(CPACK_PACKAGE_CONTACT "me@gmitch215.xyz")

    set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/INSTALLER_README.txt")
    set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

    if (WIN32)
        set(CPACK_GENERATOR "ZIP;NSIS;TGZ;WIX")

        set(CPACK_SYSTEM_NAME "windows-${ARCHITECTURE}")

        # NSIS
        set(CPACK_NSIS_DISPLAY_NAME ${PROJECT_NAME})
        set(CPACK_NSIS_DISPLAY_NAME_SET ON)
        set(CPACK_NSIS_PACKAGE_NAME ${PROJECT_NAME})

        set(CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_SOURCE_DIR}/assets/cmdfx.ico")
        set(CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}/assets/cmdfx.ico")
        set(CPACK_NSIS_MUI_UNIICON "${PROJECT_SOURCE_DIR}/assets/cmdfx.ico")
        set(CPACK_NSIS_MENU_LINKS "GitHub" ${PROJECT_HOMEPAGE_URL} "Documentation" ${PROJECT_DOCUMENTATION})
        set(CPACK_NSIS_MODIFY_PATH ON)

        # WiX
        set(CPACK_WIX_VERSION 4)
        set(CPACK_WIX_PRODUCT_ICON "${PROJECT_SOURCE_DIR}/assets/cmdfx.ico")
        set(CPACK_WIX_LICENSE_RTF "${PROJECT_SOURCE_DIR}/LICENSE.rtf")

        set(CPACK_WIX_UPGRADE_GUID "42937a72-0b91-4a57-8f45-60d84df05e66")
        set(CPACK_WIX_CMAKE_PACKAGE_REGISTRY ${PROJECT_NAME})
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

        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR} <${CPACK_PACKAGE_CONTACT}>")

        set(CPACK_RPM_PACKAGE_LICENSE "MIT")
    endif()

    include(CPack)

    cpack_add_component(library DISPLAY_NAME "cmdfx Library" DESCRIPTION "The core library for cmdfx" REQUIRED ON)
    cpack_add_component(attribution DISPLAY_NAME "Attribution" DESCRIPTION "Attribution files from cmdfx" REQUIRED ON)

    cpack_add_component_group(docs DISPLAY_NAME "Documentation" DESCRIPTION "Documentation for cmdfx" EXPANDED ON)
    cpack_add_component(htmlDocs DISPLAY_NAME "HTML Documentation" DESCRIPTION "HTML documentation for cmdfx" GROUP docs DISABLED ON)
    cpack_add_component(latexDocs DISPLAY_NAME "LaTeX Documentation" DESCRIPTION "LaTeX documentation for cmdfx" GROUP docs DISABLED ON)
endif()

# Kotlin/Native Bindings

option(KN_CMDFX "Build Kotlin/Native bindings for ${PROJECT_NAME}" ON)
if (KN_CMDFX AND CMAKE_C_COMPILER_ID MATCHES "GNU|Clang" AND (APPLE OR WIN32 OR ARCHITECTURE STREQUAL "x64"))
    include(FetchContent)
    FetchContent_Declare(
        CMakeKt
        GIT_REPOSITORY https://github.com/gmitch215/cmakekt.git
        GIT_TAG        master
    )
    FetchContent_MakeAvailable(CMakeKt)
    list(APPEND CMAKE_MODULE_PATH "${cmakekt_SOURCE_DIR}")

    set(KN_CINTEROP_FOLDER "${CMAKE_BINARY_DIR}/cinterop")
    set(KN_DEFINITION_PACKAGE "dev.gmitch215.cmdfx")
    set(KN_INSTALL_GROUPID "dev.gmitch215")
    set(KN_DEFINITION_LIBRARIES "libcmdfx.a")

    if (DEFINED PROJECT_VERSION_SUFFIX)
        set(KN_PROJECT_VERSION "${PROJECT_VERSION}-${PROJECT_VERSION_SUFFIX}")
        set(MAVEN_REMOTE_URL "https://repo.calcugames.xyz/repository/maven-snapshots/")
    else()
        set(KN_PROJECT_VERSION "${PROJECT_VERSION}")
        set(MAVEN_REMOTE_URL "https://repo.calcugames.xyz/repository/maven-releases/")
    endif()
    set(MAVEN_REMOTE_ID "calcugames")

    include(CMakeKt)
    add_klib_binary(cmdfx)

    option(DEPLOY_CMDFX "Deploy Kotlin/Native bindings for ${PROJECT_NAME}" OFF)
    if (DEPLOY_CMDFX)
        add_maven_deployment(cmdfx "${MAVEN_REMOTE_URL}" "${MAVEN_REMOTE_ID}")
    endif()
endif()
