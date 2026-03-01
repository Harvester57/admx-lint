# Prefer static libraries for Xerces and Boost
set(ORIGINAL_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(Boost_USE_STATIC_LIBS ON)

find_package(XercesC REQUIRED)
find_package(Boost 1.83 COMPONENTS program_options REQUIRED)

# Restore default suffixes for CURL and ICU
set(CMAKE_FIND_LIBRARY_SUFFIXES ${ORIGINAL_CMAKE_FIND_LIBRARY_SUFFIXES})

find_package(PkgConfig REQUIRED)
pkg_search_module(CURL REQUIRED IMPORTED_TARGET libcurl)
pkg_search_module(ICU REQUIRED IMPORTED_TARGET icu-uc icu-i18n)

set(HEADERS
    basetypes.h
    policydefinitionfiles.h
    policydefinitions.h
)

set(SOURCES
    main.cpp
    basetypes.cpp
    policydefinitionfiles.cpp
    policydefinitions.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME admx-lint)
install(TARGETS ${PROJECT_NAME} DESTINATION bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    XercesC::XercesC 
    Boost::program_options
    PkgConfig::CURL
    PkgConfig::ICU
)
