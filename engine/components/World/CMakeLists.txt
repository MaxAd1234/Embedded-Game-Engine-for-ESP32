idf_component_register(SRCS "World.cpp" "Game.cpp"
                       REQUIRES Vectors GameObject Tiles ObjectRegister Input TileMap main assets esp_timer GameProperties audio
                       INCLUDE_DIRS "includes")