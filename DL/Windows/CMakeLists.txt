SET_PROPERTY(GLOBAL PROPERTY PROP_DL_IMPL "DL_Windows")

PROJECT ( DL_Windows )

ADD_LIBRARY ( DL_Windows STATIC
		      Library.cpp
			  Library.hpp
			  PCH.cpp
			  PCH.hpp )

TARGET_LINK_LIBRARIES (DL_Windows
					   DL )

