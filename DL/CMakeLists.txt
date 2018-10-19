# Configuration
IF(WIN32)
	ADD_SUBDIRECTORY(Windows)
ENDIF(WIN32)


GET_PROPERTY(DL_IMPL GLOBAL PROPERTY PROP_DL_IMPL)
MESSAGE("Dynamic library implementation: ${DL_IMPL}")



PROJECT ( DL )

ADD_LIBRARY ( DL STATIC
		      Library.cpp
			  Library.hpp
			  PCH.cpp
			  PCH.hpp )

TARGET_LINK_LIBRARIES ( DL )
