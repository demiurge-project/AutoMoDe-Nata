
if(ARGOS_BUILD_FOR_SIMULATOR)
       
    # Take all .cpp files and put it in the variable neat_sources
    FILE(GLOB neat_sources *.cpp)
    
    # Take all .h files and put it in the variable neat_header
    FILE(GLOB neat_header *.h)
    
    # Add the shared library neat
    # shared libraries are linked dynamically and loaded at runtime²
    add_library(neat SHARED ${neat_header} ${neat_sources})
else()

    set(neat_sources
        gene.cpp
	genome.cpp
	innovation.cpp
	link.cpp
	neat.cpp
	network.cpp
	nnode.cpp
	trait.cpp
    )
    set(neat_headers
        gene.h
	genome.h
	innovation.h
	link.h
	neat.h
	network.h
	nnode.h
	trait.h
    )
    
    add_library(neat SHARED ${neat_header} ${neat_sources})
endif()
