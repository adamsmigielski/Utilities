PROJECT ( math )

ADD_LIBRARY (math STATIC
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

