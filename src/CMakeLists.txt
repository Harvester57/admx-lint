# Prefer static libraries for Xerces and Boost
set(ORIG_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(Boost_USE_STATIC_LIBS ON)

find_package(XercesC REQUIRED)
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

# Restore so system libs can be found dynamically
set(CMAKE_FIND_LIBRARY_SUFFIXES ${ORIG_FIND_LIBRARY_SUFFIXES})

set(CURL_LIB "/usr/lib/x86_64-linux-gnu/libcurl-gnutls.so.4")
find_library(ICU_UC_LIB NAMES icuuc REQUIRED)
find_library(ICU_DATA_LIB NAMES icudata REQUIRED)
find_library(ICU_I18N_LIB NAMES icui18n REQUIRED)

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
    ${CURL_LIB}
    ${ICU_UC_LIB}
    ${ICU_DATA_LIB}
    ${ICU_I18N_LIB}
)
