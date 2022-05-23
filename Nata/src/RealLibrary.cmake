# Set up ARGoS compilation information
include_directories(${CMAKE_SOURCE_DIR} ${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIRS})

# Headers
set(AUTOMODE_HEADERS
	core/AutoMoDeController.h
	core/AutoMoDeFiniteStateMachine.h
	core/AutoMoDeFsmBuilder.h
	core/AutoMoDeFsmHistory.h
	# Behaviours
	modules/AutoMoDeBehaviour.h
	modules/AutoMoDeBehaviourGenome.h
	# Conditions
	modules/AutoMoDeCondition.h
	modules/AutoMoDeConditionBlackFloor.h
	modules/AutoMoDeConditionWhiteFloor.h
	modules/AutoMoDeConditionGrayFloor.h
	modules/AutoMoDeConditionNeighborsCount.h
	modules/AutoMoDeConditionInvertedNeighborsCount.h
	modules/AutoMoDeConditionFixedProbability.h
    modules/NataConditionProx.h
    modules/NataConditionFloor.h
    modules/NataConditionLight.h
    modules/NataConditionNeighborsCount.h
    modules/NataConditionNeighborsVector.h
	# NEAT
	NEAT/gene.h
	NEAT/genome.h
	NEAT/innovation.h
	NEAT/link.h
	NEAT/neat.h
	NEAT/network.h
	NEAT/nnode.h
	NEAT/trait.h)

# Sources
set(AUTOMODE_SOURCES
	core/AutoMoDeController.cpp
	core/AutoMoDeFiniteStateMachine.cpp
	core/AutoMoDeFsmBuilder.cpp
	core/AutoMoDeFsmHistory.cpp
	# Behaviours
	modules/AutoMoDeBehaviour.cpp
	modules/AutoMoDeBehaviourGenome.cpp
	# Conditions
	modules/AutoMoDeCondition.cpp
	modules/AutoMoDeConditionBlackFloor.cpp
	modules/AutoMoDeConditionWhiteFloor.cpp
	modules/AutoMoDeConditionGrayFloor.cpp
	modules/AutoMoDeConditionNeighborsCount.cpp
	modules/AutoMoDeConditionInvertedNeighborsCount.cpp
	modules/AutoMoDeConditionFixedProbability.cpp
    modules/NataConditionProx.cpp
    modules/NataConditionFloor.cpp
    modules/NataConditionLight.cpp
    modules/NataConditionNeighborsCount.cpp
    modules/NataConditionNeighborsVector.cpp
	# NEAT
	NEAT/gene.cpp
	NEAT/genome.cpp
	NEAT/innovation.cpp
	NEAT/link.cpp
	NEAT/neat.cpp
	NEAT/network.cpp
	NEAT/nnode.cpp
	NEAT/trait.cpp)


#add_executable(harlequin ${AUTOMODE_HEADERS} ${AUTOMODE_SOURCES})
#target_link_libraries(harlequin argos3plugin_${ARGOS_BUILD_FOR}_epuck argos3_demiurge_epuck_dao) 

add_executable(harlequin /home/aligot/Desktop/Arena/iridia-tracking-system/src/plugins/robots/e-puck/real_robot/real_epuck_its.h
		        /home/aligot/Desktop/Arena/iridia-tracking-system/src/plugins/robots/e-puck/real_robot/real_epuck_its_main.cpp
							${AUTOMODE_HEADERS} ${AUTOMODE_SOURCES})
target_link_libraries(harlequin argos3plugin_${ARGOS_BUILD_FOR}_epuck argos3_demiurge_epuck_dao
											argos3plugin_${ARGOS_BUILD_FOR}_genericvirtualsensorsandactuators)


