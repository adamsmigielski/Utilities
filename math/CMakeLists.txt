PROJECT ( Math )

ADD_LIBRARY (Math STATIC
			 Float4.hpp
			 Float12.hpp
			 Float16.hpp
			 FloatTypes.hpp
			 Matrix.hpp
			 PCH.cpp
			 PCH.hpp
			 Quaternion.hpp
			 Vector.hpp )

SET_TARGET_PROPERTIES ( Math PROPERTIES DEFINE_SYMBOL "MATH_PROJECT_DLL" )

TARGET_LINK_LIBRARIES ( Math Common )

