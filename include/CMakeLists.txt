SET(${PROJECT_NAME}_HEADERS
    bci-interface/BackgroundSprite.h
    bci-interface/VisionServerBS.h
    bci-interface/DisplayObject.h
    bci-interface/square-function.h
    bci-interface/SSVEPStimulus.h
    bci-interface/BCIInterface.h
)

INSTALL(FILES ${${PROJECT_NAME}_HEADERS}
	DESTINATION include/${PROJECT_NAME}
	PERMISSIONS	OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE
)
