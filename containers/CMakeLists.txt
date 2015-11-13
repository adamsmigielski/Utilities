PROJECT(containers)

ADD_LIBRARY (containers STATIC
             IntrusiveList.hpp
			 PCH.hpp
			 PCH.cpp
			 PointerContainer.hpp
			 ReferenceCounted.cpp
			 ReferenceCounted.hpp
			 Singleton.hpp)

# Test
IF (BUILD_TESTS)

# Binaries
    ADD_EXECUTABLE (containers_test
    				${CMAKE_SOURCE_DIR}/src/Unit_Tests/main.cpp
    				PCH.cpp
    				PCH.hpp
					test.cpp)

# Setup math_test
	TARGET_COMPILE_DEFINITIONS (containers_test PUBLIC UNIT_TESTS_ENABLE)

	TARGET_LINK_LIBRARIES(containers_test
						  Unit_Tests
						  containers)
ENDIF (BUILD_TESTS)