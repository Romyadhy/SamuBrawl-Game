# About this repo

This is my 2D game with **raylib** and I make this using c++.

# Structure File

```
SamuBrawl/
  ├── CMakeLists.txt/
  ├── libs/
  ├── src/
```

CMakeLists.txt: Cmake file that help to mapping files, libs, and so on.  
libs folder: Libs that contain raylib library and the others header folder.  
src: Main folder assets and contain main.cpp.

# Installing and Setup

_About this repo game:_

- First you need to make sure has intalling _raylib_, _CMake_, Compiler CPP, and also LSP for CPP.
- Clone repo:

```
git clone https://github.com/Romyadhy/SamuBrawl-Game.git
```

- Next create build folder inside your folder game.

```
cd SamuBrawl-Game
mkdir build
```

- Next init and build using cmake:

```
cmake -S . -B build "CMakeLists"
```

Run the game:

```
build\app.exe
```

# How to play

This game has 2 player, Player 1 (Left green one) and Player 2(Right red one).  
Player 1:  
 Move Key: WASD  
 Attack Key: J and K

Player 2:  
 Move Key: Arrow Key  
 Attack Key: X and C

Additional Keys:  
R: Reset game if one player has win  
F: Fullscreen  
B: Debug Hithox Draw

# Assets Rights

This assets is made by _LuizMelo_ u can access it for free into this link: [LuizMelo](https://nastanliev.itch.io/samurai)

#### Copyright (c) 2026 Romyadhy. All Rights Reserved.
