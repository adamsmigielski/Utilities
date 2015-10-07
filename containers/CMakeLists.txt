PROJECT(containers)

ADD_LIBRARY (containers STATIC
             IntrusiveList.hpp
			 PCH.hpp
			 PCH.cpp
			 PointerContainer.hpp
			 ReferenceCounted.cpp
			 ReferenceCounted.hpp
			 SharedResource.hpp
			 Singleton.hpp)
