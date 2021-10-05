cmake_minimum_required(VERSION 3.8)

project("engine")

set(
	SOURCES
		"include/IEngine.hpp"
		"src/system/log.hpp"
		"src/system/keys.hpp"
		"src/system/types.hpp"
		"src/system/utils.hpp"
		"src/system/utils.cpp"
		"src/system/EDirection.hpp"
		"src/system/FramesTimer.hpp"
		"src/system/FramesTimer.cpp"
		"src/system/SecondsTimer.hpp"
		"src/system/SecondsTimer.cpp"
		"src/renderer/Font.hpp"
		"src/renderer/Font.cpp"
		"src/renderer/Text2D.hpp"
		"src/renderer/Text2D.cpp"
		"src/renderer/Sprite2D.hpp"
		"src/renderer/Sprite2D.cpp"
		"src/renderer/Animator.hpp"
		"src/renderer/Animator.cpp"
		"src/renderer/Texture2D.hpp"
		"src/renderer/Texture2D.cpp"
		"src/renderer/MainWindow.hpp"
		"src/renderer/MainWindow.cpp"
		"src/renderer/ShaderProgram.hpp"
		"src/renderer/ShaderProgram.cpp"
		"src/audio/SoundBuffer.hpp"
		"src/audio/SoundBuffer.cpp"
		"src/game/Core.hpp"
		"src/game/Core.cpp"
		"src/game/InputHandler.hpp"
		"src/game/InputHandler.cpp"
		"src/game/PhysicsEngine.hpp"
		"src/game/PhysicsEngine.cpp"
		"src/game/ResourcesManager.hpp"
		"src/game/ResourcesManager.cpp"
		"src/game/states/GameState.hpp"
		"src/game/states/LevelState.hpp"
		"src/game/states/Level.hpp"
		"src/game/states/Level.cpp"
		"src/game/states/Screen.hpp"
		"src/game/states/Screen.cpp"
		"src/game/states/LevelStart.hpp"
		"src/game/states/LevelStart.cpp"
		"src/game/states/LevelFinish.hpp"
		"src/game/states/LevelFinish.cpp"
		"src/game/states/StartScreen.hpp"
		"src/game/states/StartScreen.cpp"
		"src/game/states/PauseScreen.hpp"
		"src/game/states/PauseScreen.cpp"
		"src/game/states/GameOverScreen.hpp"
		"src/game/states/GameOverScreen.cpp"
		"src/game/objects/Ice.hpp"
		"src/game/objects/Ice.cpp"
		"src/game/objects/Tree.hpp"
		"src/game/objects/Tree.cpp"
		"src/game/objects/Title.hpp"
		"src/game/objects/Title.cpp"
		"src/game/objects/Water.hpp"
		"src/game/objects/Water.cpp"
		"src/game/objects/Bonus.hpp"
		"src/game/objects/Bonus.cpp"
		"src/game/objects/Eagle.hpp"
		"src/game/objects/Eagle.cpp"
		"src/game/objects/Bullet.hpp"
		"src/game/objects/Bullet.cpp"
		"src/game/objects/Border.hpp"
		"src/game/objects/Border.cpp"
		"src/game/objects/Enemies.hpp"
		"src/game/objects/Enemies.cpp"
		"src/game/objects/LevelMap.hpp"
		"src/game/objects/LevelMap.cpp"
		"src/game/objects/LevelGUI.hpp"
		"src/game/objects/LevelGUI.cpp"
		"src/game/objects/BrickWall.hpp"
		"src/game/objects/BrickWall.cpp"
		"src/game/objects/GameObject.hpp"
		"src/game/objects/GameObject.cpp"
		"src/game/objects/BigExplosion.hpp"
		"src/game/objects/BigExplosion.cpp"
		"src/game/objects/ConcreteWall.hpp"
		"src/game/objects/ConcreteWall.cpp"
		"src/game/objects/StaticObject.hpp"
		"src/game/objects/StaticObject.cpp"
		"src/game/objects/DynamicObject.hpp"
		"src/game/objects/DynamicObject.cpp"
		"src/game/objects/tanks/Tank.hpp"
		"src/game/objects/tanks/Tank.cpp"
		"src/game/objects/tanks/EnemyTank.hpp"
		"src/game/objects/tanks/EnemyTank.cpp"
		"src/game/objects/tanks/PlayerTank.hpp"
		"src/game/objects/tanks/PlayerTank.cpp"
		"src/game/objects/tanks/AIComponent.hpp"
		"src/game/objects/tanks/AIComponent.cpp"
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC "cxx_std_17")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

if(WIN32)
	set(SFML_STATIC_LIBRARIES TRUE)
	set(SFML_DIR "external/SFML-2.5.1/windows/SFML/lib/cmake/SFML")
	find_package(SFML 2.5 COMPONENTS "graphics" "audio" "main" REQUIRED)
	if(SFML_FOUND)
		target_link_libraries(
			${PROJECT_NAME} PUBLIC "sfml-graphics" "sfml-audio" "sfml-main"
		)
	endif(SFML_FOUND)
elseif(UNIX)
	find_package(SFML 2.5 COMPONENTS "graphics" "audio" REQUIRED)
	if(SFML_FOUND)
		target_link_libraries(
			${PROJECT_NAME} PUBLIC "sfml-graphics" "sfml-audio"
		)
	endif(SFML_FOUND)
endif(WIN32)

target_include_directories(${PROJECT_NAME} PUBLIC "include")
target_include_directories(
	${PROJECT_NAME} PRIVATE "src" "external/rapidjson/include"
)

add_subdirectory("external/spdlog")
target_link_libraries(${PROJECT_NAME} PRIVATE "spdlog")
