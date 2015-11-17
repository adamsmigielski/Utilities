PROJECT ( text )

ADD_LIBRARY (text STATIC
			 Font.cpp
			 Font.hpp
			 Glyph.hpp
			 Glyph.cpp
			 Layout.hpp
			 Layout.cpp
			 PCH.cpp
			 PCH.hpp )

SET_TARGET_PROPERTIES ( text PROPERTIES DEFINE_SYMBOL "TEXT_PROJECT_DLL" )

TARGET_LINK_LIBRARIES ( text Common memory )

# Test
IF (BUILD_TESTS)

# Binaries
    ADD_EXECUTABLE (text_test
    				${CMAKE_SOURCE_DIR}/src/Unit_Tests/main.cpp
    				PCH.cpp
    				PCH.hpp
					test.cpp)

# Setup math_test
	TARGET_COMPILE_DEFINITIONS (text_test PUBLIC UNIT_TESTS_ENABLE)

	TARGET_LINK_LIBRARIES(text_test
						  Unit_Tests
						  text)
ENDIF (BUILD_TESTS)

