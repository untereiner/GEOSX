# Specify list of tests
set( dataRepository_tests
     testObjectCatalog.cpp
     testRestartBasic.cpp
     testRestartExtended.cpp
     testWrapperHelpers.cpp
     testGroupPath.cpp )

set( dependencyList ${parallelDeps} gtest )

if ( GEOS_BUILD_SHARED_LIBS )
  list( APPEND dependencyList geosx_core HDF5::HDF5 )
else()
  list( APPEND dependencyList ${geosx_core_libs} HDF5::HDF5 )
endif()

if (TARGET pugixml::pugixml)
  list( APPEND dependencyList pugixml::pugixml )
endif()

if (TARGET pugixml)
  list( APPEND dependencyList pugixml )
endif()

if (TARGET fmt::fmt-header-only)
  list( APPEND dependencyList fmt::fmt-header-only )
endif()

if (TARGET fmt)
  list( APPEND dependencyList fmt )
endif()

# Add gtest C++ based tests
foreach(test ${dataRepository_tests})
    get_filename_component( test_name ${test} NAME_WE )
    blt_add_executable( NAME ${test_name}
                        SOURCES ${test}
                        OUTPUT_DIR ${TEST_OUTPUT_DIRECTORY}
                        DEPENDS_ON ${dependencyList} )

    geos_add_test( NAME ${test_name}
                   COMMAND ${test_name} )
endforeach()
