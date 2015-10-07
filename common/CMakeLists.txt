PROJECT(common)

ADD_LIBRARY (common STATIC
			 Align.hpp
             Assert.hpp
             BreakToDebug.hpp
			 CallConvention.hpp
			 ErrorCodes.hpp
             Log.hpp
             PCH.cpp
             PCH.hpp
             )
