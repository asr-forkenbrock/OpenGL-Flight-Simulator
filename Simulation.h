/*
Simulation.h

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

#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <thread>
#include <random>
#include "game_config.h"
#include "Shaders.h"


class Buttons {
    private: 
        struct button_zone {
            int x1, y1, x2, y2; 
            bool clicked = false; 
            bool hover = false; 
        }; 
        std::vector<button_zone> Zones;
    public: 
        void RemoveLastButton(); 
        int Size(); 
        void AddButton(int x1, int x2, int y1, int y2); 
        bool CheckClick(unsigned int num); 
        bool CheckHover(unsigned int num); 
        void ScanButtons(double xpos, double ypos, bool click); 
}; 

struct Model {
    glm::vec3 position = glm::vec3(0, 50, 0); 
    glm::vec3 forward  = glm::vec3(0, 0, -1);
    glm::vec3 up       = glm::vec3(0, 1, 0);
    glm::vec3 right    = glm::vec3(1, 0, 0);
    glm::vec3 centerOffset; 
    std::vector<float> Vertices;
    unsigned int VAO, VBO;
    unsigned int texture; 
    float pitchPlane=0;
    float rollPlane=0;
    float yawPlane=0;
};

struct Fragment {
    std::vector<float> vertexData;
    glm::vec3 velocity;
    glm::vec3 position; 
    glm::vec3 rotationAxis;
    float rotationSpeed;
    bool grounded = false; 
};

struct obj_pos{
    int x, y, z; 
}; 


class Config {
    public: 
        int RANDOM(int min, int max); 
        std::string INT_TO_STRING(int num); 

    protected: 
        bool debug = false;                          //tracking mouse clicks to console
        bool environment_objects_enabled = false;    //Set true for houses, apartments, water, trees, etc  -  this will cost you FPS
        const GameConfig gameConfig = GameContext(); 
        GLFWwindow* window;         
        const std::vector<HEADING> headings = HeadingContext();
        const std::vector<AircraftConfig> PlaneConfigs = PlaneContext(); 
        const EnviConfig EnvironmentConfig = EnvironmentContext(); 
        const std::vector<CameraAnglesStruct> CamAngles = CameraContext();
        const TexturesConfig TEXTURES = TextureContext(); 
        const ModelsConfig MODELS = ModelsContext(); 

        const std::vector<NPCflightPlan> FlightPlanNPC = NPCflightContext(); 

        const float RAD_TO_DEG = 57.29577951f;
        bool force_close_program = false; 
}; 


class Graphics_2D : public Config {
    public:
        void Add2DRect(float x1, float y1, float x2, float y2, int R, int G, int B, int A); 
        void PrintString(std::string STRING, int x, int y, int scale, int R=255, int G=255, int B=255){
            for(int FCY=0; FCY<STRING.length(); FCY++){ TEXT(STRING[FCY], x+(FCY*(scale*5)), y, scale, R, G, B); }
        }
        void TEXT(char letter, int x, int y, int scale, int R, int G, int B); 
        void InitUI(); 
        void Add2DLine(float x1, float y1, float x2, float y2, int R, int G, int B, int A); 
        void Add2DCircleOutline(float centerX, float centerY, float radius, int segments, int R, int G, int B, int A); 
        void Add2DCircle(float centerX, float centerY, float radius, int segments, int R, int G, int B, int A); 
        void Add2DRect4P(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int R, int G, int B, int A); 
        void AppendRectVertices(std::vector<float>& vertexBuffer, float x1, float y1, float x2, float y2); 
        void TEXT_Batched(char letter, int x, int y, int scale, std::vector<float>& buffer); 
        void PrintString_Batched(std::string STRING, int x, int y, int scale, std::vector<float>& buffer); 
        void DrawUIElement(unsigned int textureID, float x, float y, float w, float h); 
        void Refresh2d(); 
        void setupWindow(); 

    protected:
        unsigned int uiShaderProgram;
        unsigned int uiVAO, uiVBO;
        unsigned int uiTextureShaderProgram;
        unsigned int uiTexVAO, uiTexVBO;
        int logger_load = 0;
        std::vector<std::string> Logger; 
        std::vector<float> uiBatchBuffer;
        std::vector<float> GUIBatchBuffer;
        unsigned int IntroImageTexture; 
        glm::mat4 projection2D;          // cached, set once in InitUI()
        GLint uiMVPLoc = -1;             // cached uniform locations
        GLint uiColorLoc = -1;
        GLint uiTexMVPLoc = -1;
        GLint uiTexSamplerLoc = -1;
}; 

class Graphics_3D : public Graphics_2D {
    public:
        unsigned int OBJTEXTUREloadTexture(const std::string& path); 
        void load_shaders();
        void MatrixSharing(Model& plane, const std::vector<CameraAnglesStruct>& CamAngles, int cam_ang);
        std::vector<float> loadSTL(const std::string& filename); 

    protected:
        unsigned int globalShaderProgram;
        unsigned int OBJTEXTUREglobalShaderProgram; 
        unsigned int shaderProgram; 
        unsigned int main_VBO, main_VAO;
        glm::mat4 view; 
        glm::mat4 projection; 
}; 

class HUD : public Graphics_3D {
    public: 
        void SetupControlPanel();
        void FPS_Meter(); 
        void ControlCompass(Model& plane, float Airport_Heading_X, float Airport_Heading_Z);
        void ControlADI(Model& plane); 
    protected: 
        double lastTime=0; 
};

class Menus : public HUD {
    public: 
        bool CheckOpenMenus(); 
        void MAIN_MENU();
        void ControlsMenu(); 
        
    protected: 
        bool main_menu_active = false; 
        bool settings_menu_active=false; 
        bool controls_menu_active = false; 
        float explosionTimer = 0.0f; 
        bool plane_exploded = false; 
        bool plane_drop_down; 
        bool airport_drop_down; 
        int drop_down_click_start; 
        int drop_down_click_current; 
        Buttons buttons; 
};

class FlightSimulation : public Menus {
    public: 
        FlightSimulation(){
            WATER_BODIES.resize(EnvironmentConfig.water_count); 
            HOUSES.resize(EnvironmentConfig.house_count);  
            APARTMENTS.resize(EnvironmentConfig.apartment_count); 
            TREES.resize(EnvironmentConfig.tree_count); 
        }
        void GetBankAngle(Model &plane); 
        void GetPitchAngle(Model &plane); 
        void pitch(Model& plane, float angleDegrees); 
        void roll(Model& plane, float angleDegrees); 
        void yaw(Model& plane, float angleDegrees); 
        void LoadPlane(std::string path, Model &plane, bool notsetup);
        void DrawPlane(Model &plane, float scale, unsigned int &shader, bool check_adjust); 
        void AUTO_PILOT(); 
        void LoadHeading(int num);
        void LoadHeadingWindow(); 
        void LoadNewPlane(int planeNum);
        void INPUT_HANDLER();
        void NPCauto();
        void Model_Runway(int x, int y, int z, bool LR); 
        void shatter(const std::vector<float>& planeData, const glm::vec3& impactVelocity);
        void PLANE_EXPLOSION(); 
        void LoadEnvironment(); 
        void LoadSimulator(); 
        void RunSimulator(); 
        //3d tools
        void load_objects();
        void load_textures();
        //menus
        void LoadingScreen();
        void StartMenu(); 
        void SETTINGS_MENU();
        //hud
        void ControlPanel(Model& plane); 
        void ControlPanelFlightData1(); 
        void ControlPanelFlightData2(); 
        void ControlPanelErrorLights(); 
        void ControlPanelEngines(); 
        void ControlPanelGPS(Model& plane); 
        
    private: 
        int active_npc = 1; 
        int heading_select=0; 
        int PlaneType = 3; 
        int cam_ang = 0; 
        float Airport_Heading_X;
        float Airport_Heading_Z;
        float Airport_Heading_RUNWAY_START_X; 
        float Airport_Heading_RUNWAY_START_Z;
        float Airport_Heading_RUNWAY_X; 
        float Airport_Heading_RUNWAY_Z;
        float BRAKES_AFTER_X; 
        bool flash_led_warns[10]; int flash_led_count[10]; 
        bool autopilot_engaged=false; 
        bool MAST_WARN=false; long warn_counter_flash=100000; bool warn_flash_ON=false; 
        bool MAST_CAUT = false; long caut_counter_flash=100000; bool caut_flash_ON=false; 
        
        bool flaps_active = true; 
        bool gear_active = true; 


        const float MAX_THROTTLE = 1.18; 
        float engine1_temp = 800.00;
        float engine2_temp = 800.00;
        float tank1 = 100.00; 
        float tank2 = 100.00;
        bool fall_test=false; 
        float fall_speed=0; 
        short bad_engine_test=0;  
        int engine1_power=0; 
        int engine2_power=0; 
        float airspeed=0; 

        float throttle = 0.0f; 
        int cam_pos=0; 
        int auto_pilot_stage = 0; 
        float auto_climb_angle = 0; 
        int NPCautopilot=0; 
        float NPCairspeed=0; 

        unsigned int groundTexture; 
        
        std::vector<obj_pos> WATER_BODIES; 
        std::vector<obj_pos> HOUSES; 
        std::vector<obj_pos> APARTMENTS; 
        std::vector<obj_pos> TREES; 
        std::vector<Fragment> planeFragments;

        Model NPCplanes[10]; 
        Model plane;
        Model terrian1; 
        Model house1;
        Model tree1; 
        Model terminal;         
        Model RunwaySection; 
        Model TOWER;
        Model Building1;
        Model Water1; 
        Model apartment1; 
}; 

#endif