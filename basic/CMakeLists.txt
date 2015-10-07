PROJECT(basic)

ADD_LIBRARY (basic STATIC
			 Align.hpp
             Assert.hpp
             BreakToDebug.hpp
			 ErrorCodes.hpp
             Log.hpp
             PCH.cpp
             PCH.hpp
             )
