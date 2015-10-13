PROJECT ( math )

ADD_LIBRARY (math STATIC
			 Float.hpp
			 Float4.hpp
			 Float12.hpp
			 Float16.hpp
			 FloatTypes.hpp
			 Matrix.hpp
			 PCH.cpp
			 PCH.hpp
			 Quaternion.hpp
			 Vector.hpp )

SET_TARGET_PROPERTIES ( math PROPERTIES DEFINE_SYMBOL "MATH_PROJECT_DLL" )

TARGET_LINK_LIBRARIES ( math Common )

# Test
IF (BUILD_TESTS)

# Binaries
    ADD_EXECUTABLE (math_test
    				${CMAKE_SOURCE_DIR}/src/Unit_Tests/main.cpp
    				PCH.cpp
    				PCH.hpp
					test.cpp)

# Setup math_test
	TARGET_COMPILE_DEFINITIONS (math_test PUBLIC UNIT_TESTS_ENABLE)

	TARGET_LINK_LIBRARIES(math_test
						  Unit_Tests
						  math)
ENDIF (BUILD_TESTS)

