SET(${PROJECT_NAME}_HEADERS
	ssvp-interface/SSVPInterface.h
	ssvp-interface/SSVPInterfaceConfig.h
	ssvp-interface/FlickeringSquare.h
	ssvp-interface/square-function.h
    ssvp-interface/BackgroundSprite.h
    ssvp-interface/P300Interface.h
)

INSTALL(FILES ${${PROJECT_NAME}_HEADERS}
	DESTINATION include/${PROJECT_NAME}
	PERMISSIONS	OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE
)
