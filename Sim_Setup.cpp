/*
Sim_Setup.cpp

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

#include "Simulation.h"

void Buttons::RemoveLastButton(){
    Zones.pop_back(); 
}

int Buttons::Size(){
    return Zones.size(); 
}

void Buttons::AddButton(int x1, int x2, int y1, int y2){
    Zones.push_back({x1, y1, x2, y2}); 
}

bool Buttons::CheckClick(unsigned int num){
    if(num >= Zones.size()) return false; 
    return Zones[num].clicked; 
}
bool Buttons::CheckHover(unsigned int num){
    if(num >= Zones.size()) return false; 
    return Zones[num].hover; 
}

void Buttons::ScanButtons(double xpos, double ypos, bool click){
    for(int i=0; i<Zones.size(); i++){
        if(xpos >= Zones[i].x1 && xpos <= Zones[i].x2 && ypos >= Zones[i].y1 && ypos <= Zones[i].y2){
            Zones[i].hover = true; 
            if(click){
                Zones[i].clicked = true; std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }else{
                Zones[i].clicked = false; 
            }
        }else{
            Zones[i].hover = false; 
            Zones[i].clicked = false; 
        }
    }
} 

void FlightSimulation::LoadPlane(std::string path, Model &plane, bool notsetup){
    std::vector<float>& planeData = plane.Vertices;  
    unsigned int& planeVAO = plane.VAO; 
    unsigned int& planeVBO = plane.VBO; 
    glm::vec3& centerOffset = plane.centerOffset; 

    Logger.push_back("  - "+path); if(!notsetup){ Refresh2d(); } 
    planeData = loadSTL(path);
    if (planeData.empty()){ std::cout << "FILE DIDN'T LOAD! \n"; return; }

    float minX = 1e10, maxX = -1e10, minY = 1e10, maxY = -1e10, minZ = 1e10, maxZ = -1e10;
    for (size_t i = 0; i < planeData.size(); i += 6) { 
        minX = std::min(minX, planeData[i]);   maxX = std::max(maxX, planeData[i]);
        minY = std::min(minY, planeData[i+1]); maxY = std::max(maxY, planeData[i+1]);
        minZ = std::min(minZ, planeData[i+2]); maxZ = std::max(maxZ, planeData[i+2]);
    }
    glm::vec3 centerOffsets((minX + maxX)/2.0f, (minY + maxY)/2.0f, (minZ + maxZ)/2.0f);
    centerOffset = centerOffsets; 

    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, planeData.size() * sizeof(float), planeData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}


void FlightSimulation::load_objects(){
    LoadPlane((std::string(MODELS.RunwayModel)).c_str(), RunwaySection, false);
    LoadPlane((std::string(MODELS.TerminalModel)).c_str(), terminal, false); 
    LoadPlane((std::string(MODELS.TowerModel)).c_str(), TOWER, false);
    LoadPlane((std::string(MODELS.CityModel)).c_str(), Building1, false); 
    LoadPlane((std::string(MODELS.WaterModel)).c_str(), Water1, false); 
    LoadPlane((std::string(MODELS.TerrianModel)).c_str(), terrian1, false); 
    LoadPlane((std::string(MODELS.HouseModel)).c_str(), house1, false); 
    LoadPlane((std::string(MODELS.ApartmentModel)).c_str(), apartment1, false); 
    LoadPlane((std::string(MODELS.TreeModel)).c_str(), tree1, false); 
    LoadPlane(PlaneConfigs[PlaneType].model_path, plane, false); //F22
    LoadPlane(PlaneConfigs[0].model_path, NPCplanes[0], false);  //A350
    LoadPlane(PlaneConfigs[1].model_path, NPCplanes[1], false);  //A320 Neo
    LoadPlane(PlaneConfigs[2].model_path, NPCplanes[2], false);  //B747
    for(int load=3; load<9; load++){
        NPCplanes[load].Vertices = NPCplanes[1].Vertices; 
        NPCplanes[load].VAO = NPCplanes[1].VAO; 
        NPCplanes[load].VBO = NPCplanes[1].VBO;
        NPCplanes[load].centerOffset = NPCplanes[1].centerOffset; 
    }
}

void FlightSimulation::load_textures(){
    groundTexture = terrian1.texture = OBJTEXTUREloadTexture(EnvironmentConfig.LandTexture); 
    house1.texture = OBJTEXTUREloadTexture((std::string(TEXTURES.HouseTexture)).c_str());
    tree1.texture = OBJTEXTUREloadTexture((std::string(TEXTURES.TreeTexture)).c_str());
    terminal.texture = OBJTEXTUREloadTexture((std::string(TEXTURES.TerminalTexture)).c_str());
    RunwaySection.texture = OBJTEXTUREloadTexture((std::string(TEXTURES.RunwayTexture)).c_str());
    TOWER.texture = OBJTEXTUREloadTexture((std::string(TEXTURES.TowerTexture)).c_str());
    Building1.texture = OBJTEXTUREloadTexture((std::string(TEXTURES.CityTexture)).c_str());
    Water1.texture = OBJTEXTUREloadTexture((std::string(TEXTURES.WaterTexture)).c_str()); 
    apartment1.texture = OBJTEXTUREloadTexture((std::string(TEXTURES.ApartmentTexture)).c_str()); 
    plane.texture = OBJTEXTUREloadTexture((std::string(PlaneConfigs[PlaneType].texture_path)).c_str()); 
    NPCplanes[0].texture = OBJTEXTUREloadTexture((std::string(PlaneConfigs[0].texture_path)).c_str()); 
    NPCplanes[1].texture = OBJTEXTUREloadTexture((std::string(PlaneConfigs[1].texture_path)).c_str()); 
    NPCplanes[2].texture = OBJTEXTUREloadTexture((std::string(PlaneConfigs[2].texture_path)).c_str()); 
    for(int load=3; load<9; load++){
        NPCplanes[load].texture = NPCplanes[1].texture; 
    }
}

int Config::RANDOM(int min, int max){
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}

void FlightSimulation::DrawPlane(Model &plane, float scale, unsigned int &shader, bool check_adjust=false){
    glm::vec3 centerOffset = plane.centerOffset; 
    std::vector<float> &planeData = plane.Vertices; 
    unsigned int &planeVAO = plane.VAO; 
    unsigned int& texture = plane.texture; 

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, plane.position);
        
        glm::mat4 orientation(1.0f);
        orientation[0] = glm::vec4(plane.right, 0.0f);
        orientation[1] = glm::vec4(plane.up, 0.0f);
        orientation[2] = glm::vec4(-plane.forward, 0.0f);
        model = model * orientation;
        
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
        if(PlaneType == 2 and check_adjust==true){
            model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 0, 1));
        }else{
            model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0, 0, 1));
        }
        model = glm::scale(model, glm::vec3(scale));
        model = glm::translate(model, -centerOffset);

        glUseProgram(shader);
        glm::mat4 planeMVP = projection * view * model;
        glUniformMatrix4fv(glGetUniformLocation(shader, "mvp"), 1, GL_FALSE, glm::value_ptr(planeMVP));
        glUniform4f(glGetUniformLocation(shader, "overlayColor"), 1.0f, 1.0f, 1.0f, 1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture); 
        glUniform1i(glGetUniformLocation(shader, "ourTexture"), 0);
        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, planeData.size() / 6);


}

void FlightSimulation::LoadEnvironment(){
    Logger.push_back("Generating House Coordinates..."); Refresh2d(); 
    for(int i=0; i<80000; i++){
        HOUSES[i].x = RANDOM(-500000, 500000);
        while(!(HOUSES[i].x < -4000 or 4000 < HOUSES[i].x)){ HOUSES[i].x = RANDOM(-500000, 500000); }
        HOUSES[i].y = 15;
        HOUSES[i].z = RANDOM(-500000, 500000);
        while(!(HOUSES[i].z < -4000 or 4000 < HOUSES[i].z)){ HOUSES[i].z = RANDOM(-500000, 500000); }
    }
    Logger.push_back("  - Successfully Generated House Coordinates"); Refresh2d(); 

    Logger.push_back("Generating Water Coordinates..."); Refresh2d(); 
    for(int i=0; i<800; i++){
        WATER_BODIES[i].x = RANDOM(-500000, 500000);
        while(!(WATER_BODIES[i].x < -4000 or 4000 < WATER_BODIES[i].x)){ WATER_BODIES[i].x = RANDOM(-500000, 500000); }
        WATER_BODIES[i].z = RANDOM(-500000, 500000);
        while(!(WATER_BODIES[i].z < -4000 or 4000 < WATER_BODIES[i].z)){ WATER_BODIES[i].z = RANDOM(-500000, 500000); }
    }
    Logger.push_back("  - Successfully Generated Water Coordinates"); Refresh2d(); 

    Logger.push_back("Generating Aparment Coordinates..."); Refresh2d(); 
    for(int i=0; i<30000; i++){
        APARTMENTS[i].x = RANDOM(-500000, 500000);
        while(!(APARTMENTS[i].x < -4000 or 4000 < APARTMENTS[i].x)){ APARTMENTS[i].x = RANDOM(-500000, 500000); }
        APARTMENTS[i].z = RANDOM(-500000, 500000);
        while(!(APARTMENTS[i].z < -4000 or 4000 < APARTMENTS[i].z)){ APARTMENTS[i].z = RANDOM(-500000, 500000); }
    }
    Logger.push_back("  - Successfully Generated Aparment Coordinates"); Refresh2d(); 

    Logger.push_back("Generating Tree Coordinates..."); Refresh2d(); 
    for(int i=0; i<58000; i++){
        TREES[i].x = RANDOM(-500000, 500000);
        while(!(TREES[i].x < -4000 or 4000 < TREES[i].x)){ TREES[i].x = RANDOM(-500000, 500000); }
        TREES[i].z = RANDOM(-500000, 500000);
        while(!(TREES[i].z < -4000 or 4000 < TREES[i].z)){ TREES[i].z = RANDOM(-500000, 500000); }
    }
    Logger.push_back("  - Successfully Generated Trees Coordinates"); Refresh2d(); 


    Logger.push_back("Starting flight simulator..."); Refresh2d();
}


void FlightSimulation::shatter(const std::vector<float>& planeData, const glm::vec3& impactVelocity) {
    for(size_t i = 0; i < planeData.size(); i += 18){ 
        Fragment f;
        float avgX = (planeData[i]   + planeData[i+6] + planeData[i+12]) / 3.0f;
        float avgY = (planeData[i+1] + planeData[i+7] + planeData[i+13]) / 3.0f;
        float avgZ = (planeData[i+2] + planeData[i+8] + planeData[i+14]) / 3.0f;
        f.position = plane.position + glm::vec3(avgX, avgY, avgZ);
        for(int j = 0; j<18; j+=6){
            f.vertexData.push_back(planeData[i + j]     - avgX); 
            f.vertexData.push_back(planeData[i + j + 1] - avgY);
            f.vertexData.push_back(planeData[i + j + 2] - avgZ); 
            f.vertexData.push_back(planeData[i + j + 3]);        
            f.vertexData.push_back(planeData[i + j + 4]);        
            f.vertexData.push_back(planeData[i + j + 5]);        
        }
        f.velocity = impactVelocity * -0.2f + glm::vec3(RANDOM(-10,10), RANDOM(0,20), RANDOM(-10,10));
        f.rotationAxis = glm::normalize(glm::vec3(RANDOM(-5,5), RANDOM(-5,5), RANDOM(-5,5)));
        f.rotationSpeed = RANDOM(0,10);
        f.grounded = false;

        planeFragments.push_back(f);
    }
}

void HUD::FPS_Meter(){
    static int fps_counter=0; 
    static int current_counter_fps=0; 
    std::string fps = "FPS: "; 
    fps += std::to_string(fps_counter);
    PrintString(fps, 600, 700, 2); 
    double currentTime = glfwGetTime();
    if(currentTime - lastTime >= 1.0){
        fps_counter = current_counter_fps; current_counter_fps = 0; lastTime = currentTime; 
    }else{
        current_counter_fps++; 
    }
}