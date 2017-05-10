PROJECT(basic)

ADD_LIBRARY (basic STATIC
			 Align.hpp
             Assert.hpp
             BreakToDebug.hpp
			 CallConvention.hpp
			 Dll.hpp
			 ErrorCodes.hpp
			 Log.cpp
             Log.hpp
             PCH.cpp
             PCH.hpp
             )
