SET(${PROJECT_NAME}_HEADERS
	bci-interface/SSVEPInterface.h
	bci-interface/SSVEPInterfaceConfig.h
	bci-interface/FlickeringSquare.h
	bci-interface/MoovingCursor.h
	bci-interface/RandomPoint.h
	bci-interface/square-function.h
    bci-interface/BackgroundSprite.h
    bci-interface/P300Interface.h
    bci-interface/HybridInterface.h
    bci-interface/TestInterface.h
)

INSTALL(FILES ${${PROJECT_NAME}_HEADERS}
	DESTINATION include/${PROJECT_NAME}
	PERMISSIONS	OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE
)
