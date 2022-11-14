# TestProject

Welcome to the my demo project! <br />To run it you need:

  1. Install Unreal Engine 4.27 and Visual Studio 2019 <br />
  2. Run the TestProject.uproject file and build the project <br />
		2.1 Optional: To read the source code, you need to right-click on TestProject.uproject and select "Generate Visual Sturio project files". After that run                               TestProject.sln file. Source code will be opened by Visual Studio. <br />
  3. When Unreal Editor builds, MainMenu map will open automaticly (if it willn't, open the map MainMenu by path "/FirstPersonCPP/Maps/MainMenu" in Editor) <br />
  4. Click on Play button. <br />
  5. Enjoy! <br />
  
# Some information about the project:
  
  · You need to kill all enemys on the map to win <br />
  · At the first playable level you can find some tutorial inscriptions <br /> 
  · The project have 3 maps: MainMenu, FirstPersonExampleMap and Hangar. Maps folder is "Content/FirstPersonCPP/Maps/" <br />
  · Almost all logic in C++, but there are also a few blueprints. <br />
  · Many assets i have got from asset store, but only visual (models, animations, sounds, vfx). All logic i have written myself.
  
# Architecture

  All C++ code is in "/Source/TestProject" folder (or "C++ classes/TestProject", if you are looking from editor) <br />
  · The "Cheats" folder contains a custom Cheat manager and a widget for cheats. <br />
  · The "Components" folder contains a InventoryManager's class. This component exist in the PlayerState. <br />
  · The "GameObjects/Characters" folder contains all Character's classes in project. TestProjectCharacter is general character's class (parent), PlayerCharacter is character's class for player, AICharacter is character's class for AI <br />
  · The "GameObjects/Items" folder contains all Item's classes in project. DefaultItem is general item's class, PickableItem is general class for pickable items (child of DefaultItem) <br />
  · The "GameObjects/Items/Weapons" folder contains a general class for weapons that the character is holding (child of DefaultItem). <br />
  · The "MainClasses" folder contains all main classes such as GameMode, HUD, PlayerController, etc <br />
  · The "MainClasses" folder contains classes for Save files. SettingsSave is save for settings, ProgressSave is save for saving game progress. <br />
  · The "UI" folder contains all widget classes. <br />
  · The "Utilities" folder contains all technical classes and files for structures. Also here is UseInterface. It is interface for all usable items and objects. <br />
