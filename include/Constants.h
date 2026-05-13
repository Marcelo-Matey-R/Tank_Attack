#pragma once
#include <string>

//Ventana
const int WINDOW_WIDTH      = 1000;
const int WINDOW_HEIGHT     = 800;
const std::string GAME_TITLE = "Tank Attack!";
const int FRAMERATE_LIMIT   = 60;

//Cuadrícula 
const int CELL_SIZE         = 40;
const int GRID_ROWS         = 20;
const int GRID_COLS         = 20;
const int MAP_WIDTH         = CELL_SIZE * GRID_COLS;   // 800
const int MAP_HEIGHT        = CELL_SIZE * GRID_ROWS;   // 800
const int HUD_WIDTH         = WINDOW_WIDTH - MAP_WIDTH; // 200

//HUD
const int HUD_FONT_SIZE         = 16;
const int HUD_TITLE_FONT_SIZE   = 20;
const int HUD_PADDING           = 10;

//Menú
const int MENU_BUTTON_WIDTH     = 200;
const int MENU_BUTTON_HEIGHT    = 50;
const int MENU_BUTTON_SPACING   = 20;
const int MENU_TITLE_FONT_SIZE  = 48;
const int MENU_BUTTON_FONT_SIZE = 24;

//Rutas de assets
const std::string ASSETS_PATH   = "assets/";
const std::string TEXTURES_PATH = ASSETS_PATH + "textures/";
const std::string FONTS_PATH    = ASSETS_PATH + "fonts/";

//Texturas 
const std::string TEX_TANK_BLUE   = TEXTURES_PATH + "tank_blue.png";
const std::string TEX_TANK_CYAN   = TEXTURES_PATH + "tank_cyan.png";
const std::string TEX_TANK_RED    = TEXTURES_PATH + "tank_red.png";
const std::string TEX_TANK_YELLOW = TEXTURES_PATH + "tank_yellow.png";
const std::string TEX_TILE_FLOOR  = TEXTURES_PATH + "tile_floor.png";
const std::string TEX_TILE_WALL   = TEXTURES_PATH + "tile_wall.png";
const std::string TEX_BULLET      = TEXTURES_PATH + "bullet.png";
const std::string TEX_POWERUP     = TEXTURES_PATH + "powerup.png";

//Fuentes 
const std::string FONT_MAIN = FONTS_PATH + "main_font.ttf";

//Colores de tanques
const int COLOR_TANK_BLUE[3]   = {0,   0,   255};
const int COLOR_TANK_CYAN[3]   = {0,   255, 255};
const int COLOR_TANK_RED[3]    = {255, 0,   0};
const int COLOR_TANK_YELLOW[3] = {255, 255, 0};

//Ruta trazada del pathfinding
const int PATH_COLOR_ALPHA  = 150;  // transparencia
const int PATH_CELL_PADDING = 8;    // para que el dibujado no cubra absolutamente toda la celda