/*
game_config.h

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

#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <vector>
#include "bitmap_font.h"

struct GameConfig {
    float window_x; 
    float window_y; 
};

struct HEADING {
    float Airport_Heading_X;
    float Airport_Heading_Z; 
    std::string name; 
}; 
  
struct AircraftConfig {
    float planeScale;
    float plane_thrust;
    float planes_height;
    int plane_v1_speed; 
    int plane_vr_speed; 
    float plane_battery_voltages; 
    int plane_over_speed; 
    int stall_warning_speed; 
    int stall_speed; 
    std::string model_path; 
    std::string name; 
    std::string texture_path; 
};

struct EnviConfig {
    int water_count;
    int house_count;
    int apartment_count; 
    int tree_count; 
    const char* LandTexture; 
};

struct TexturesConfig {
    std::string TreeTexture; 
    std::string WaterTexture; 
    std::string CityTexture; 
    std::string HouseTexture; 
    std::string ApartmentTexture; 
    std::string RunwayTexture; 
    std::string TowerTexture; 
    std::string TerminalTexture; 
    std::string EmptyTexture; //used when a texture is not found or needed 
};

struct ModelsConfig {
    std::string TreeModel; 
    std::string WaterModel; 
    std::string CityModel; 
    std::string HouseModel; 
    std::string ApartmentModel; 
    std::string RunwayModel; 
    std::string TowerModel; 
    std::string TerminalModel; 
    std::string TerrianModel; 
};

struct CameraAnglesStruct{
    float ds, FX, FY, FZ, cX, cY, cZ; 
};

struct NPCflightPlan {
    float pitch; 
    float roll; 
    float yaw; 
    float airspeed; 
    float pitchGoal; 
    float rollGoal; 
    float yawGoal; 
    float airspeedGoal; 
    float AltGoal; 
    int pass_wait; 
}; 

const GameConfig &GameContext(); 
const std::vector<HEADING> &HeadingContext();
const std::vector<AircraftConfig> &PlaneContext(); 
const EnviConfig &EnvironmentContext(); 
const std::vector<CameraAnglesStruct> &CameraContext();
const TexturesConfig &TextureContext(); 
const ModelsConfig &ModelsContext(); 
const std::vector<NPCflightPlan> &NPCflightContext(); 

#endif