/*
game_config.cpp

Flight Simulator - A OpenGL flight simulator written in C++.
Copyright (C) 2026  Noah Forkenbrock <asr-forkenbrock> 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


//IMPORTANT: Replace "textures/BLUE.png" with your prefered texture!


#include "game_config.h"

const GameConfig &GameContext(){
    static const GameConfig gameConfig = {1280.0f, 720.0f}; 
    return gameConfig; 
}

const std::vector<HEADING> &HeadingContext(){
    static const std::vector<HEADING> headings = {
        {212922.0f, -133013.0f, "Airport 2"}, 
        {8300.0f, 1200.0f, "Airport 1"}, 
    }; 
    return headings; 
}

const std::vector<AircraftConfig> &PlaneContext(){
    static const std::vector<AircraftConfig> PlaneConfigs = {
        {1.5f,  6.8f,  60.0f,  155, 165, 25.2, 330, 135/2, 125/2, "models/A350.stl", "A350", "textures/BLUE.png"}, 
        {3.5f,  5.4f,  60.0f,  140, 145, 28.1, 350, 125/2, 115/2, "models/A320Neo.stl", "A320 Neo", "textures/BLUE.png"}, 
        {3.5f,  7.8f,  110.0f, 155, 165, 28.0, 365, 145/2, 135/2, "models/747.stl", "747", "textures/BLUE.png"}, 
        {0.55f, 11.5f, 60.0f,  135, 140, 28.2, 800, 145/2, 130/2, "models/F22.stl", "F22", "textures/BLUE.png"}, 
    }; 
    return PlaneConfigs; 
}

const EnviConfig &EnvironmentContext(){
    static const EnviConfig EnvironmentConfig = {800, 80000, 30000, 58000, "textures/land2.png"}; 
    return EnvironmentConfig; 
}

const std::vector<CameraAnglesStruct> &CameraContext(){
    static const std::vector<CameraAnglesStruct> CamAngles = {
        {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}, 
        {900.0f, 90.0f, 0.0f, 0.0f, 90.0f, 0.0f, 0.0f}, 
        {900.0f, 90.0f, 0.0f, 90.0f, 90.0f, 180.0f, 90.0f}, 
        {500.0f, 0.0f, 0.0f, 0.0f, 0.0f, -210.0f, 0.0f}, 
        {0.0f, 0.0f, 90.0f, 0.0f, 0.0f, 20000.0f, 0.0f}, 
    }; 
    return CamAngles; 
}

const TexturesConfig &TextureContext(){
    static const TexturesConfig textures = {
        "textures/BLUE.png", //Trees
        "textures/BLUE.png", //Water
        "textures/Building_5.jpg", //City
        "textures/BLUE.png", //Houses
        "textures/BLUE.png", //Apartments
        "textures/asphalt.jpg", //Runway
        "textures/BLUE.png", //Tower
        "textures/BLUE.png", //Terminal
        "textures/BLUE.png" //Placeholder
    }; 
    return textures; 
}


const ModelsConfig &ModelsContext(){
    static const ModelsConfig models = {
        "models/Tree.stl", //Trees
        "models/water.stl", //Water
        "models/WholeCity.stl", //City
        "models/House.stl", //Houses
        "models/Building_6.stl", //Apartments
        "models/Body4.stl", //Runway
        "models/Modern.Tower.1.400scale.stl", //Tower
        "models/KTPA_Termainal_F.stl", //Terminal
        "models/terrain.stl" //Land Terrain
    }; 
    return models; 
}

const std::vector<NPCflightPlan> &NPCflightContext(){
    static const std::vector<NPCflightPlan> flightplan = {
        {0.0, 0.0, -0.1, 0.1,    0.0, 0.0, -90.0, 1.0,   50,   0}, 
        {0.0, 0.0, 0.0, 1.0,    0.0, 0.0, -90.0, 35.0,   50,   0}, 
        {0.1, 0.0, 0.0, 0.0,    3.0, 0.0, -90.0, 35.0,   1550,   0}, 
        {-0.1, 0.0, 0.0, 0.0,    0.0, 0.0, -90.0, 35.0,   1550,   0}, 
        {0.0, 0.0, 0.0, 0.0,    0.0, 0.0, -90.0, 35.0,   1550,   0}, 
        {0.0, 0.0, -0.5, 0.0,    0.0, 0.0, -270.0, 35.0,   1550,   0}, 
        {0.0, 0.0, 0.0, 0.0,    0.0, 0.0, -270.0, 35.0,   1550,   150}, 
        {0.0, 0.0, -0.5, 0.0,    0.0, 0.0, -450.0, 35.0,   1550,   0}, 
        {0.0, 0.0, 0.0, -0.1,    0.0, 0.0, -450.0, 20.0,   1550,   0}, 
        {-0.1, 0.0, 0.0, 0.0,    -3.0, 0.0, -450.0, 20.0,   50,   0}, 
        {0.1, 0.0, 0.0, -0.1,    0.0, 0.0, -450.0, 0.0,   50,   0}
    }; 
    return flightplan; 
}
