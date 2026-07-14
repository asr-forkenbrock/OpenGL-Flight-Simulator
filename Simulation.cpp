/*
Simulation.cpp

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

void FlightSimulation::GetBankAngle(Model &plane){
    glm::vec3 worldUp(0, 1, 0);
    glm::vec3 levelRight = glm::normalize(glm::cross(plane.forward, worldUp));
    glm::vec3 levelUp    = glm::cross(levelRight, plane.forward);
    float rollRad = atan2(glm::dot(plane.right, levelUp), glm::dot(plane.right, levelRight));
    float rollDeg = glm::degrees(rollRad);
    plane.rollPlane = rollDeg; 
}

void FlightSimulation::GetPitchAngle(Model &plane){ 
    glm::vec3 fwd = glm::normalize(plane.forward); 
    float pitchRad = asin(glm::clamp(fwd.y, -1.0f, 1.0f)); 
    plane.pitchPlane = glm::degrees(pitchRad);
}

void FlightSimulation::pitch(Model& plane, float angleDegrees) {
    float radians = glm::radians(angleDegrees);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), radians, plane.right);
    plane.forward = glm::normalize(glm::vec3(rotation * glm::vec4(plane.forward, 0.0f)));
}

void FlightSimulation::roll(Model& plane, float angleDegrees) {
    float radians = glm::radians(angleDegrees);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), radians, plane.forward);
    plane.up    = glm::normalize(glm::vec3(rotation * glm::vec4(plane.up, 0.0f)));
    plane.right = glm::normalize(glm::vec3(rotation * glm::vec4(plane.right, 0.0f)));
}

void FlightSimulation::yaw(Model& plane, float angleDegrees) {
    float radians = glm::radians(angleDegrees);
    plane.yawPlane += radians; 
    if(plane.yawPlane >= 360){ plane.yawPlane-=360; } 
    if(plane.yawPlane <= -360){ plane.yawPlane+=360; }
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), radians, plane.up);    
    plane.forward = glm::normalize(glm::vec3(rotation * glm::vec4(plane.forward, 0.0f)));
    plane.right   = glm::normalize(glm::vec3(rotation * glm::vec4(plane.right, 0.0f)));
}

bool Menus::CheckOpenMenus(){ //return true if open 
    if(settings_menu_active || controls_menu_active || main_menu_active || (explosionTimer >= 20.0 && plane_exploded)){
        return true; 
    }
    return false; 
}

void FlightSimulation::LoadHeading(int num){
    Airport_Heading_X = headings[num].Airport_Heading_X;
    Airport_Heading_Z = headings[num].Airport_Heading_Z;
    Airport_Heading_RUNWAY_START_X = Airport_Heading_X + 9000.0f; 
    Airport_Heading_RUNWAY_START_Z = Airport_Heading_Z;
    Airport_Heading_RUNWAY_X = Airport_Heading_X - 400; 
    Airport_Heading_RUNWAY_Z = Airport_Heading_Z;
    BRAKES_AFTER_X = Airport_Heading_RUNWAY_START_X-3000.0f; 
}

void FlightSimulation::LoadingScreen(){
    glDisable(GL_DEPTH_TEST); 
    glm::mat4 projection2D = glm::ortho(0.0f, gameConfig.window_x, 0.0f, gameConfig.window_y, -1.0f, 1.0f); 
    glUseProgram(globalShaderProgram); 
    glUniformMatrix4fv(glGetUniformLocation(globalShaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(projection2D));
    IntroImageTexture = OBJTEXTUREloadTexture((std::string("textures/IntroImageTexture.png")).c_str()); 
    Logger.push_back("Please Wait..."); 
    Logger.push_back("Loading Objects..."); Refresh2d(); 
    load_objects();
    Logger.push_back("  - Successfully Loaded Objects"); Refresh2d(); 
    Logger.push_back("Loading Textures..."); Refresh2d(); 
    load_textures(); 
    Logger.push_back("  - Successfully Loaded Textures"); Refresh2d(); 
    Logger.push_back("Loading Shaders..."); Refresh2d(); 
    load_shaders(); 
    Logger.push_back("  - Successfully Loaded Shaders"); Refresh2d(); 
}


void FlightSimulation::AUTO_PILOT(){
    if(autopilot_engaged){ 
        if(auto_pilot_stage == 0){ 
            if(throttle < 1.00){ throttle += 0.020; }
            if(airspeed > int(PlaneConfigs[PlaneType].plane_vr_speed/2)){ auto_pilot_stage = 1; }
        }else if(auto_pilot_stage == 1){ 
            if(auto_climb_angle < 15.00){ auto_climb_angle += 0.25; pitch(plane, 0.25f); }
            if(plane.position.y > 2500){ auto_pilot_stage = 2; }
        }else if(auto_pilot_stage == 2){ 
            if(auto_climb_angle > 0.00){ auto_climb_angle -= 0.25; pitch(plane, -0.25f); }else{ auto_pilot_stage = 3; } 
        }else if(auto_pilot_stage == 3){ 
            float targetX = Airport_Heading_RUNWAY_START_X;
            float targetZ = Airport_Heading_RUNWAY_START_Z; 
            float diff_x = targetX - plane.position.x;
            float diff_z = targetZ - plane.position.z;  
            float targetBearing = std::atan2(-diff_x, -diff_z) * (180.0f / M_PI);  
            float currentHeading = plane.yawPlane * RAD_TO_DEG;  
            float angleDelta = targetBearing - currentHeading; 
            while (angleDelta > 180)  angleDelta -= 360;
            while (angleDelta < -180) angleDelta += 360; 
            float smoothYaw = angleDelta * 0.01f; 
            yaw(plane, smoothYaw);
            if(plane.rollPlane >= 0.00){ roll(plane, plane.rollPlane); } 
            if(std::abs(diff_x) < 10000 && std::abs(diff_z) < 10000){ if(throttle > 0.10){ throttle -= 0.020; } }
            if(std::abs(diff_x) < 1000 && std::abs(diff_z) < 1000){ auto_pilot_stage = 5; }
        }else if(auto_pilot_stage == 5){ 
            if(auto_climb_angle < 15.00){ auto_climb_angle += 0.25; pitch(plane, -0.25f); }
            if(plane.rollPlane != 0.00){ roll(plane, plane.rollPlane); } 
            if((Airport_Heading_Z-280.00) < plane.position.z && plane.position.z < (Airport_Heading_Z+280.00)){
                float pathDX = Airport_Heading_RUNWAY_X - Airport_Heading_X;
                float pathDZ = Airport_Heading_RUNWAY_Z - Airport_Heading_Z; 
                float runwayBearing = std::atan2(-pathDX, -pathDZ) * (180.0f / M_PI); 
                float dist_x = plane.position.x - Airport_Heading_X;
                float dist_z = plane.position.z - Airport_Heading_Z; 
                float sideError = (dist_x * pathDZ - dist_z * pathDX) / std::sqrt(pathDX*pathDX + pathDZ*pathDZ); 
                float correction = sideError * -0.1f; 
                float targetBearing = runwayBearing + correction; 
                float currentHeading = plane.yawPlane * RAD_TO_DEG;
                float angleDelta = targetBearing - currentHeading; 
                while (angleDelta > 180)  angleDelta -= 360;
                while (angleDelta < -180) angleDelta += 360; 
                yaw(plane, angleDelta * 0.10f);
            }
            if(plane.position.x < BRAKES_AFTER_X){ auto_pilot_stage=6; }
        }else if(auto_pilot_stage == 6){ 
            if(plane.position.y < 240+PlaneConfigs[PlaneType].planes_height){ auto_pilot_stage = 7; }  
        }else if(auto_pilot_stage == 7){  
            if(auto_climb_angle > 3.25){ auto_climb_angle -= 0.35; pitch(plane, 0.35f); }else{ auto_pilot_stage = 8; } 
            if(throttle > 0.00){ throttle -= 0.020; }
        }else if(auto_pilot_stage == 8){  
            airspeed -= 0.10; 
            if(airspeed <= 0){ auto_pilot_stage = 9; airspeed=0; }
        }else if(auto_pilot_stage == 9){ 
            auto_climb_angle = 0; 
            auto_pilot_stage = 0; 
            autopilot_engaged = false; 
            MAST_WARN = MAST_CAUT = false; 
        }
    }
}




void FlightSimulation::LoadHeadingWindow(){
    LoadHeading(heading_select);
}

void FlightSimulation::LoadNewPlane(int planeNum){
    PlaneType=planeNum;
    plane.position.y = PlaneConfigs[PlaneType].planes_height + 1; 
    LoadPlane(PlaneConfigs[PlaneType].model_path, plane, true);
}

void FlightSimulation::INPUT_HANDLER(){
        double xpos, ypos; glfwGetCursorPos(window, &xpos, &ypos);
        buttons.ScanButtons(xpos, ypos, (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)); 
        if(buttons.CheckClick(0)){
            settings_menu_active = true; 
        }
        if(buttons.CheckClick(1) && settings_menu_active){
            settings_menu_active = false; 
        }
        if(buttons.CheckClick(2)){
            if(heading_select == headings.size()-1){ heading_select=0; }else{ heading_select++; }
        }
        if(buttons.CheckClick(3)){
            if(PlaneType == PlaneConfigs.size()-1){ PlaneType = 0; }else{ PlaneType++; }
            LoadNewPlane(PlaneType); 
        }
        if(buttons.CheckClick(4)){
            controls_menu_active = true; 
        }
        if(buttons.CheckClick(5) && controls_menu_active){
            controls_menu_active = false; 
        }

        if(buttons.CheckClick(8) && !CheckOpenMenus()){
            if(autopilot_engaged){ autopilot_engaged=false; }else{ autopilot_engaged=true; }
        }
        

        if(plane.position.y <= PlaneConfigs[PlaneType].planes_height && !plane_exploded){ 
            yaw(plane, plane.yawPlane); 
            plane.up       = glm::vec3(0, 1, 0);
            plane.forward  = glm::vec3(plane.forward.x, 0, plane.forward.z);
            plane.pitchPlane=0;
            plane.rollPlane=0;
            plane.yawPlane=0; 
            plane.position.y = PlaneConfigs[PlaneType].planes_height+1; 
        }

        //Debugging space - this will be removed before publishing
        if(debug && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            std::cout << "X: " << xpos << " Y: " << ypos << "\n"; std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
        }


        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !CheckOpenMenus()){ 
            if(main_menu_active){ main_menu_active=false; }else{ main_menu_active=true; } std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
        }

        if(glfwGetKey(window, GLFW_KEY_L    ) == GLFW_PRESS && !CheckOpenMenus()){ 
            if(autopilot_engaged){ autopilot_engaged=false; }else{ autopilot_engaged=true; } std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
        } 


        if(glfwGetKey(window, GLFW_KEY_F    ) == GLFW_PRESS && !CheckOpenMenus()){ 
            if(flaps_active){ flaps_active=false; }else{ flaps_active=true; } std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
        } 
        if(glfwGetKey(window, GLFW_KEY_G    ) == GLFW_PRESS && !CheckOpenMenus()){ 
            if(gear_active){ gear_active=false; }else{ gear_active=true; } std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
        } 


        if(glfwGetKey(window, GLFW_KEY_T    ) == GLFW_PRESS && !CheckOpenMenus()){ 
            bad_engine_test=1; 
        } 
        if(glfwGetKey(window, GLFW_KEY_W    ) == GLFW_PRESS && !CheckOpenMenus()){ 
            if(plane.position.y > PlaneConfigs[PlaneType].planes_height){
                pitch(plane, 0.5f);
            }else{
                if(airspeed > PlaneConfigs[PlaneType].plane_v1_speed/2){ pitch(plane, 0.5f); } 
            }
        }
        if(glfwGetKey(window, GLFW_KEY_S    ) == GLFW_PRESS && !CheckOpenMenus()){ 
            if(plane.position.y > PlaneConfigs[PlaneType].planes_height){ pitch(plane, -0.5f); } 
        }
        if(glfwGetKey(window, GLFW_KEY_A    ) == GLFW_PRESS && !CheckOpenMenus()){ 
            if(airspeed>0){ yaw(plane, 0.5f); } 
        }
        if(glfwGetKey(window, GLFW_KEY_D    ) == GLFW_PRESS && !CheckOpenMenus()){ 
            if(airspeed>0){ yaw(plane, -0.5f); } 
        }
        if(glfwGetKey(window, GLFW_KEY_UP   ) == GLFW_PRESS && !CheckOpenMenus()){ 
            throttle += 0.01; 
        }
        if(glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS && !CheckOpenMenus()){ 
            throttle -= 0.01; 
        }
        if(glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS && !CheckOpenMenus()){ 
            if(plane.position.y>PlaneConfigs[PlaneType].planes_height){ roll(plane, -0.5f); } 
        } 
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && !CheckOpenMenus()){ 
            if(plane.position.y>PlaneConfigs[PlaneType].planes_height){ roll(plane, 0.5f); } 
        } 
        if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !main_menu_active && !controls_menu_active && !settings_menu_active && plane.position.y<PlaneConfigs[PlaneType].planes_height+5 && airspeed > 0){ 
            airspeed -=0.05; 
            if(airspeed < 0.00){ airspeed=0; }
        } 
        if(glfwGetKey(window, GLFW_KEY_F1   ) == GLFW_PRESS && !CheckOpenMenus()){ 
            if(heading_select == headings.size()-1){ heading_select=0; }else{ heading_select++; } std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
        }
        if(glfwGetKey(window, GLFW_KEY_F2   ) == GLFW_PRESS && !CheckOpenMenus()){ 
            if(PlaneType == PlaneConfigs.size()-1){ PlaneType = 0; }else{ PlaneType++; }
            LoadNewPlane(PlaneType); 
            std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
        }
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
            if(cam_ang >= CamAngles.size()-1){ cam_ang = 0; }else{ cam_ang++; }
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        if(throttle < 0.00){ throttle=0.00; }
        if(throttle > MAX_THROTTLE){ throttle=MAX_THROTTLE; }
}

void FlightSimulation::NPCauto(){
    static float yaw_npc, pitch_npc, wait; 
    static bool check_pitch, check_yaw, check_airspeed, check_alt, check_wait; 
    static double time_wait; static bool time_wait_start = false; 
    if(!CheckOpenMenus()){
        if(!time_wait_start){

            if(int(NPCairspeed) != int(FlightPlanNPC[NPCautopilot].airspeedGoal)){ NPCairspeed += FlightPlanNPC[NPCautopilot].airspeed; check_airspeed=false; }else{ check_airspeed=true; }

            if(int(yaw_npc) != int(FlightPlanNPC[NPCautopilot].yawGoal)){ yaw(NPCplanes[1], FlightPlanNPC[NPCautopilot].yaw); yaw_npc+=FlightPlanNPC[NPCautopilot].yaw; check_yaw = false; }else{ check_yaw=true; }
            
            if(int(pitch_npc) != int(FlightPlanNPC[NPCautopilot].pitchGoal)){ 
                pitch(NPCplanes[1], FlightPlanNPC[NPCautopilot].pitch); pitch_npc+=FlightPlanNPC[NPCautopilot].pitch; check_pitch = false; 
            }else{ 
                check_pitch=true; 
            }

            if(FlightPlanNPC[NPCautopilot].pass_wait > 0){ if(wait > FlightPlanNPC[NPCautopilot].pass_wait){ check_wait = true; }else{ check_wait = false; } wait+=0.1; }else{ check_wait = true; }

            if(std::abs(NPCplanes[1].position.y - FlightPlanNPC[NPCautopilot].AltGoal) < 50){ check_alt = true; }else{ check_alt = false; }

            if(NPCplanes[1].position.y < 50){ NPCplanes[1].position.y = 50; check_alt = true; }

            NPCplanes[1].position += NPCplanes[1].forward * NPCairspeed;
        }

        if(check_airspeed && check_yaw && check_alt && check_wait && !time_wait_start && check_pitch){ //check_pitch
            if(FlightPlanNPC[NPCautopilot].pitchGoal == 0 && FlightPlanNPC[NPCautopilot].pitch != 0){
                GetPitchAngle(NPCplanes[1]); pitch(NPCplanes[1], (NPCplanes[1].pitchPlane*-1)); 
            }
            if(NPCautopilot < FlightPlanNPC.size()-1){
                NPCautopilot++; wait=0; 
            }else{
                NPCairspeed = 0; 
                if(!time_wait_start){ time_wait = glfwGetTime(); time_wait_start = true; }
            }
        }

                if(time_wait_start && time_wait+10 < glfwGetTime()){ 
                    NPCplanes[1].position = glm::vec3(0, 50, 0); 
                    NPCplanes[1].forward  = glm::vec3(0, 0, -1);
                    NPCplanes[1].up       = glm::vec3(0, 1, 0);
                    NPCplanes[1].right    = glm::vec3(1, 0, 0);
                    NPCplanes[1].pitchPlane = NPCplanes[1].yawPlane = NPCplanes[1].rollPlane = 0; 
                    NPCplanes[1].position.x = 550; NPCplanes[1].position.z = -500; NPCplanes[1].position.y = 60; 
                    yaw(NPCplanes[1], 90); 
                    NPCautopilot = yaw_npc = pitch_npc = wait = 0; 
                    check_pitch=check_yaw=check_airspeed=check_alt=check_wait=time_wait_start = false; 
                }
    }
}

void FlightSimulation::Model_Runway(int x, int y, int z, bool LR){ 
    if(LR){ yaw(RunwaySection, 90); }
    for(int i=0; i<18; i++){
        RunwaySection.position.y = y; 
        if(LR){ RunwaySection.position.x = (-550*i)+x; }else{ RunwaySection.position.x = x; }
        if(!LR){ RunwaySection.position.z = (-550*i)+z; }else{ RunwaySection.position.z = z; }
        DrawPlane(RunwaySection, 3.1f, OBJTEXTUREglobalShaderProgram, false);
        
        if(!LR){ RunwaySection.position.x = x+200; }
        if(LR){ RunwaySection.position.z = z+200; }
        DrawPlane(RunwaySection, 3.1f, OBJTEXTUREglobalShaderProgram, false);
    }
    if(LR){ yaw(RunwaySection, -90); }
}

void FlightSimulation::PLANE_EXPLOSION() {
    if (planeFragments.empty()) {
        shatter(plane.Vertices, plane.forward * airspeed); 
    }
    airspeed = 0; 
    plane_exploded = true; 
}



void FlightSimulation::StartMenu(){
    buttons.AddButton(1195, 1275, 25, 50); //Open settings button - 0
    buttons.AddButton(575, 705, 478, 505); //close settings button - 1
    buttons.AddButton(1195, 1275, 57, 84); //Change Airport heading - 2
    buttons.AddButton(1195, 1275, 92, 120); //Change Plane - 3
    buttons.AddButton(1195, 1275, 126, 153); //Open Controls menu - 4
    buttons.AddButton(577, 706, 386, 410); //Close Controls menu - 5
    buttons.AddButton(480, 602, 340, 380); //Resume game button - pause menu - 6
    buttons.AddButton(683, 802, 340, 380); //Exit game button - pause menu - 7

    buttons.AddButton(379, 481, 683, 707); //auto pilot button - 8

    buttons.AddButton(319, 488, 202, 217); //select plane - settings - 9
    buttons.AddButton(339, 507, 252, 273); //select airport - settings - 10

        float groundVertices[30] = {
            -500000.0f, 0.0f, -500000.0f,  0.0f,   0.0f,
            500000.0f, 0.0f, -500000.0f,  10000.0f, 0.0f,
            500000.0f, 0.0f,  500000.0f,  10000.0f, 10000.0f,

            -500000.0f, 0.0f, -500000.0f,  0.0f,   0.0f,
            500000.0f, 0.0f,  500000.0f,  10000.0f, 10000.0f,
            -500000.0f, 0.0f,  500000.0f,  0.0f,   10000.0f
        };

    glGenVertexArrays(1, &main_VAO);
    glGenBuffers(1, &main_VBO);
    glBindVertexArray(main_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, main_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
        
    plane.position.x -= 8000; 
    plane.position.y = 500; 
    plane.position.z -= 500; 
    pitch(plane, 5.0f); 

    LoadHeading(0); 

    while(!glfwWindowShouldClose(window)){
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        { 
        glUseProgram(shaderProgram);
        glm::mat4 gridMVP = projection * view; 
        glUniformMatrix4fv(glGetUniformLocation(globalShaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(gridMVP));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
        glBindVertexArray(main_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        }
        glUseProgram(globalShaderProgram);
        
        MatrixSharing(plane, CamAngles, cam_ang); 
    
        glEnable(GL_DEPTH_TEST); 
        
        
        Building1.position.x = -8050; Building1.position.y = 900; Building1.position.z = -2300; 
        DrawPlane(Building1, 3.5f, OBJTEXTUREglobalShaderProgram, false);

        Building1.position.x = -8050; Building1.position.y = 900; Building1.position.z = -6300; 
        DrawPlane(Building1, 3.5f, OBJTEXTUREglobalShaderProgram, false);

        DrawPlane(plane, PlaneConfigs[PlaneType].planeScale, OBJTEXTUREglobalShaderProgram, true);

        plane.position += plane.forward * 0.25f;

        glfwPollEvents();
        glDisable(GL_DEPTH_TEST); 
        glm::mat4 projection2D = glm::ortho(0.0f, gameConfig.window_x, 0.0f, gameConfig.window_y, -1.0f, 1.0f); 
        glUniformMatrix4fv(glGetUniformLocation(globalShaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(projection2D));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        std::vector<float> uiBatchBuffer;
        Add2DRect(40, 640, gameConfig.window_x-40, 560, 40, 40, 40, 165); 
        PrintString_Batched("Welcome to ASR Flight Simulator", 90, 620, 7, uiBatchBuffer); 

        Add2DRect(520, 150, gameConfig.window_x-520, 80, 255, 255, 225, 255); 
        Add2DRect(525, 145, gameConfig.window_x-525, 85, 46, 141, 225, 255); 
            PrintString("Get Flying!", 531, 130, 4); 

        if (!uiBatchBuffer.empty()) {
            glUseProgram(uiShaderProgram); 
            glm::mat4 projection2D = glm::ortho(0.0f, gameConfig.window_x, 0.0f, gameConfig.window_y, -1.0f, 1.0f);
            glUniformMatrix4fv(glGetUniformLocation(uiShaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(projection2D)); 
            glUniform4f(glGetUniformLocation(uiShaderProgram, "overlayColor"), 1.0f, 1.0f, 1.0f, 1.0f);
            glBindVertexArray(uiVAO);
            glBindBuffer(GL_ARRAY_BUFFER, uiVBO); 
            glBufferData(GL_ARRAY_BUFFER, uiBatchBuffer.size() * sizeof(float), uiBatchBuffer.data(), GL_DYNAMIC_DRAW); 
            glDrawArrays(GL_TRIANGLES, 0, uiBatchBuffer.size() / 3);
        }

        glfwSwapBuffers(window);
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){ 
            std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
            plane.position.x = 0;
            plane.position.y = PlaneConfigs[PlaneType].planes_height; 
            plane.position.z = 0; 
            pitch(plane, -5.0f); 
            glfwPollEvents(); return; 
        }
    }
}


void FlightSimulation::LoadSimulator(){
    setupWindow(); 
    InitUI(); 
    lastTime = glfwGetTime();
    LoadingScreen(); 
    LoadEnvironment(); 
    StartMenu(); 

    RunwaySection.position.y = 0; 
    terminal.position.x = 3750; terminal.position.y = 70; terminal.position.z = -1600; 
    TOWER.position.x = 550; TOWER.position.y = 210; TOWER.position.z = -1300;  
    Building1.position.x = -8050; Building1.position.y = 900; Building1.position.z = -2300;  
    Water1.position.x = -2550; Water1.position.y = 900; Water1.position.z = -2300; 
    NPCplanes[0].position.x = 1900; NPCplanes[0].position.z = -820; yaw(NPCplanes[0], -50); 
    NPCplanes[1].position.x = 550; NPCplanes[1].position.z = -500; yaw(NPCplanes[1], 90); 
    plane.position.y = PlaneConfigs[PlaneType].planes_height; 

    glEnable(GL_DEPTH_TEST);
    PlaneType=3;
}


void FlightSimulation::RunSimulator(){ 

    glDisable(GL_CULL_FACE);

    while (!glfwWindowShouldClose(window) && !force_close_program) {
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        { 
        glUseProgram(shaderProgram);
        glm::mat4 gridMVP = projection * view; 
        glUniformMatrix4fv(glGetUniformLocation(globalShaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(gridMVP));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
        glBindVertexArray(main_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        }
        glUseProgram(globalShaderProgram);

        
        MatrixSharing(plane, CamAngles, cam_ang); 
    
        glEnable(GL_DEPTH_TEST); 
        if(airspeed < 0){ airspeed=0; } 


        NPCauto(); 

        INPUT_HANDLER(); 

        AUTO_PILOT(); 

        GetBankAngle(plane); 
        GetPitchAngle(plane);        

        if(!plane_exploded && !main_menu_active){
            float engineForce = PlaneConfigs[PlaneType].plane_thrust * throttle;
            float drag = (airspeed * airspeed) * 0.0008f;
            float gravityForce = plane.pitchPlane * 0.055f;
            airspeed += ((engineForce - drag) - gravityForce);
            if (airspeed < 0.25){ airspeed = 0; } 
            plane.position += plane.forward * airspeed; 

            engine1_temp += (throttle*2) + (gravityForce*2); 
            engine2_temp += (throttle*2) + (gravityForce*2); 

            float tank_size = 225.00; 

            if(tank1 > 0){ tank1 -= (((throttle) + (gravityForce))/tank_size) + 0.0001f; }
            if(tank2 > 0){ tank2 -= (((throttle) + (gravityForce))/tank_size) + 0.0001f; } 

            if(engine1_temp > 800.00){ engine1_temp -= 2; } 
            if(engine2_temp > 800.00){ engine2_temp -= 2; } //cool down

            if(false){ //traffic not implemented
                if(flash_led_count[0] == 0){ flash_led_count[0] = 10; if(flash_led_warns[0]){ flash_led_warns[0] = false; }else{ flash_led_warns[0] = true; }} flash_led_count[0]--; 
            }else{ flash_led_warns[0]=false; }

            if(plane.position.y < PlaneConfigs[PlaneType].planes_height+2800 && plane.pitchPlane < -5){ 
                if(flash_led_count[1] == 0){ flash_led_count[1] = 10; if(flash_led_warns[1]){ flash_led_warns[1] = false; }else{ flash_led_warns[1] = true; }} flash_led_count[1]--; 
            }else{ flash_led_warns[1]=false; }

            if(tank1 < 10 || tank2 < 10){ 
                if(flash_led_count[2] == 0){ flash_led_count[2] = 10; if(flash_led_warns[2]){ flash_led_warns[2] = false; }else{ flash_led_warns[2] = true; }} flash_led_count[2]--; 
            }else{ flash_led_warns[2]=false; }

            if(((1+throttle)-(plane.pitchPlane/100)) > 2.2){ 
                if(flash_led_count[3] == 0){ flash_led_count[3] = 10; if(flash_led_warns[3]){ flash_led_warns[3] = false; }else{ flash_led_warns[3] = true; }} flash_led_count[3]--; 
            }else{ flash_led_warns[3]=false; }

            if((airspeed*2) > PlaneConfigs[PlaneType].plane_over_speed){ 
                if(flash_led_count[4] == 0){ flash_led_count[4] = 10; if(flash_led_warns[4]){ flash_led_warns[4] = false; }else{ flash_led_warns[4] = true; }} flash_led_count[4]--; 
            }else{ flash_led_warns[4]=false; }

            if(engine1_temp > 2100.00 or engine2_temp > 2100.00){ 
                if(flash_led_count[5] == 0){ flash_led_count[5] = 10; if(flash_led_warns[5]){ flash_led_warns[5] = false; }else{ flash_led_warns[5] = true; }} flash_led_count[5]--; 
            }else{ flash_led_warns[5]=false; }


        }

        if(airspeed>PlaneConfigs[PlaneType].stall_speed/2 && airspeed<PlaneConfigs[PlaneType].stall_warning_speed/2 && plane.position.y>PlaneConfigs[PlaneType].planes_height+40){
            if(caut_flash_ON){
                PrintString("Stall Warning", 320, 420, 10);
            }
            MAST_CAUT=true; 
        }

        if(plane.position.y == PlaneConfigs[PlaneType].planes_height){
            if(plane.rollPlane >= 0.00){ roll(plane, plane.rollPlane); } 
        }

        if((airspeed<PlaneConfigs[PlaneType].stall_speed/2 && plane.position.y>PlaneConfigs[PlaneType].planes_height+10)){ 
            MAST_WARN=true; 
            PrintString("You are in a stall!", 220, 420, 10); 
            if(airspeed > 0){ airspeed-=0.01; } 
            fall_speed = fall_speed + (9.81/50); 
            plane.position.y-=fall_speed; 
        }else{
            if(fall_speed > 0){
                fall_speed--; 
            }
        }

        if(RANDOM(0, INT32_MAX) == 8274524){ bad_engine_test=1; }
        if(RANDOM(0, INT32_MAX) == 3245894){ bad_engine_test=2; }


        if(bad_engine_test > 0){ 
            MAST_WARN=true; 
            if(bad_engine_test == 1){ roll(plane, -0.1); }
            if(bad_engine_test == 2){ roll(plane, 0.1); }
        }

        Model_Runway(212922, 0, -133013, 0); 
        Model_Runway(212922, 0, -133013, 1); 
        Building1.position.x = 209351; Building1.position.y = 900; Building1.position.z = -124416; 
        DrawPlane(Building1, 3.5f, OBJTEXTUREglobalShaderProgram, false);

        Model_Runway(-100, 0, (550*4), 0); 
        Model_Runway(-1200, 0, (550*2), 0); 

        Model_Runway(8300, 0, 1000, 1); 

        DrawPlane(terminal, 5.0f, OBJTEXTUREglobalShaderProgram, false);

        DrawPlane(TOWER, 2.5f, OBJTEXTUREglobalShaderProgram, false); 

        Building1.position.x = -8050; Building1.position.y = 1500; Building1.position.z = -300; 
        DrawPlane(Building1, 5.5f, OBJTEXTUREglobalShaderProgram, false);

        Building1.position.x = -8050; Building1.position.y = 1500; Building1.position.z = -6300; 
        DrawPlane(Building1, 5.5f, OBJTEXTUREglobalShaderProgram, false);

        Water1.position.y = 0;
        DrawPlane(Water1, 15.5f, OBJTEXTUREglobalShaderProgram, false);

        terrian1.position.y = -10; terrian1.position.z = -12000;
        DrawPlane(terrian1, 50.5f, OBJTEXTUREglobalShaderProgram, false);

        glm::vec3 forward = plane.forward; 
        glm::vec3 planePos = plane.position; 














    if(environment_objects_enabled){

        for(int i=0; i < EnvironmentConfig.house_count; i++) { 
            glm::vec3 ObjPos = glm::vec3(HOUSES[i].x, 15.0f, HOUSES[i].z);
            glm::vec3 toObj = ObjPos - planePos; 
            if(glm::dot(toObj, forward) > 0.0f){
                float distSq = (toObj.x * toObj.x) + (toObj.z * toObj.z);
                if(distSq < 100000000){ house1.position = ObjPos; DrawPlane(house1, 3.5f, OBJTEXTUREglobalShaderProgram, false); } 
            }
        }

        for(int i=0; i < EnvironmentConfig.water_count; i++) { 
            glm::vec3 ObjPos = glm::vec3(WATER_BODIES[i].x, 15.0f, WATER_BODIES[i].z); glm::vec3 toObj = ObjPos - planePos; 
            float dotResult = (toObj.x * forward.x) + (toObj.y * forward.y) + (toObj.z * forward.z);
            if(dotResult > 0){ 
                float distSq = (toObj.x * toObj.x) + (toObj.z * toObj.z);
                if(distSq < 100000000){ Water1.position = ObjPos; DrawPlane(Water1, 15.5f, OBJTEXTUREglobalShaderProgram, false); } 
            }
        }


        for(int i=0; i<EnvironmentConfig.apartment_count; i++){ 
            apartment1.position.y = 0; apartment1.position.z = APARTMENTS[i].z; apartment1.position.x = APARTMENTS[i].x; 
            if(10000 > abs(apartment1.position.x-plane.position.x) and 10000 > abs(apartment1.position.z-plane.position.z)){
                apartment1.position.y = 50; 
                DrawPlane(apartment1, 1.5f, OBJTEXTUREglobalShaderProgram, false); 
            }
        }

        for(int i=0; i<EnvironmentConfig.tree_count; i++){ 
            tree1.position.y = 0; tree1.position.z = TREES[i].z; tree1.position.x = TREES[i].x; 
            if(10000 > abs(tree1.position.x-plane.position.x) and 10000 > abs(tree1.position.z-plane.position.z)){
                DrawPlane(tree1, 0.5f, OBJTEXTUREglobalShaderProgram, false); 
            }
        }
    }


















        DrawPlane(NPCplanes[0], PlaneConfigs[0].planeScale, OBJTEXTUREglobalShaderProgram, false);
        DrawPlane(NPCplanes[1], PlaneConfigs[1].planeScale, OBJTEXTUREglobalShaderProgram, false);

        if(NPCplanes[2].position.x == 0){ yaw(NPCplanes[2], 65); } NPCplanes[2].position.x = 1720; NPCplanes[2].position.z = -1400; NPCplanes[2].position.y = 80; 
        DrawPlane(NPCplanes[2], PlaneConfigs[2].planeScale, OBJTEXTUREglobalShaderProgram, false);
        if(NPCplanes[3].position.x == 0){ yaw(NPCplanes[3], 0); } NPCplanes[3].position.x = 2200; NPCplanes[3].position.z = -620; 
        DrawPlane(NPCplanes[3], PlaneConfigs[1].planeScale, OBJTEXTUREglobalShaderProgram, false);
        if(NPCplanes[4].position.x == 0){ yaw(NPCplanes[4], 0); } NPCplanes[4].position.x = 2600; NPCplanes[4].position.z = -620; 
        DrawPlane(NPCplanes[4], PlaneConfigs[1].planeScale, OBJTEXTUREglobalShaderProgram, false);

        if(NPCplanes[5].position.x == 0){ yaw(NPCplanes[5], 0); } NPCplanes[5].position.x = 3100; NPCplanes[5].position.z = -460; 
        DrawPlane(NPCplanes[5], PlaneConfigs[1].planeScale, OBJTEXTUREglobalShaderProgram, false);
        if(NPCplanes[6].position.x == 0){ yaw(NPCplanes[6], 0); } NPCplanes[6].position.x = 3500; NPCplanes[6].position.z = -460; 
        DrawPlane(NPCplanes[6], PlaneConfigs[1].planeScale, OBJTEXTUREglobalShaderProgram, false);
        
        if(plane.position.y < PlaneConfigs[PlaneType].planes_height){ 
            if(airspeed > 35){ 
                plane_exploded = true; PLANE_EXPLOSION(); 
            }
        }

        if(plane.position.y+10 < PlaneConfigs[PlaneType].planes_height){ 
            if(fall_speed > 10){ 
                plane_exploded = true; PLANE_EXPLOSION(); 
            }
        }

        if(plane.position.y < PlaneConfigs[PlaneType].planes_height){ 
            plane.position.y = PlaneConfigs[PlaneType].planes_height; 
            fall_speed = 0; 
            if(plane.pitchPlane < 0){
                pitch(plane, abs(plane.pitchPlane)); plane.pitchPlane = 0; 
                
            }
        } 
        
        if(!plane_exploded){ 
            DrawPlane(plane, PlaneConfigs[PlaneType].planeScale, OBJTEXTUREglobalShaderProgram, true);
        }else{
            if(explosionTimer <= 30.0){
                glUseProgram(globalShaderProgram); 
                explosionTimer += 1.02; 
                for (int i = 0; i < (int)planeFragments.size(); i++) {
                    Fragment& f = planeFragments[i];
                    f.velocity.y -= 0.005f; 
                    

                    glm::mat4 fModel = glm::mat4(1.0f);
                    fModel = glm::translate(fModel, plane.position + (f.velocity * explosionTimer)); 
                    fModel = glm::rotate(fModel, explosionTimer * f.rotationSpeed, f.rotationAxis);
                    fModel = glm::scale(fModel, glm::vec3(PlaneConfigs[PlaneType].planeScale) * 0.75f);

                    glm::mat4 mvp = projection * view * fModel;
                    glUniformMatrix4fv(glGetUniformLocation(globalShaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
                    glUniform4f(glGetUniformLocation(globalShaderProgram, "overlayColor"), 1.0f, 0.4f, 0.0f, 1.0f); 

                    glBindVertexArray(plane.VAO);
                    glDrawArrays(GL_TRIANGLES, i * 3, 3);
                }
            }
        }


        glDisable(GL_DEPTH_TEST); 
        glm::mat4 projection2D = glm::ortho(0.0f, gameConfig.window_x, 0.0f, gameConfig.window_y, -1.0f, 1.0f); 
        glUniformMatrix4fv(glGetUniformLocation(globalShaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(projection2D));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if(explosionTimer >= 5.0 && plane_exploded){
            Add2DRect(318, 298, gameConfig.window_x-318, 502, 129, 139, 156, 255); 
            Add2DRect(320, 300, gameConfig.window_x-320, 500, 29, 39, 56, 255); 

            PrintString("You have crashed!", 350, 475, 7);

            if(buttons.CheckHover(6)){ Add2DRect(480, 340, 600, 380, 29+10, 39+10, 56+10, 255); }else{ Add2DRect(480, 340, 600, 380, 29+40, 39+40, 56+40, 255); }
            PrintString("Restart", 505, 367, 2);
            if(buttons.CheckClick(6)){ 
                plane_exploded = false; explosionTimer = 0; 
                throttle = 0; airspeed = 0; 
                engine1_temp = 800.00; engine2_temp = 800.00;
                tank1 = 100.00; tank2 = 100.00;
                cam_pos=0; auto_pilot_stage = 0; auto_climb_angle = 0; 
                NPCautopilot=0; NPCairspeed=0;
                autopilot_engaged=false; 
                plane.position = glm::vec3(0, PlaneConfigs[PlaneType].planes_height, 0); 
                plane.forward  = glm::vec3(0, 0, -1);
                plane.up       = glm::vec3(0, 1, 0);
                plane.right    = glm::vec3(1, 0, 0);
                plane.yawPlane = 0; 
                plane.rollPlane = 0; 
                GetPitchAngle(plane); 
                if(plane.pitchPlane < 0){
                    pitch(plane, abs(plane.pitchPlane)); plane.pitchPlane = 0; 
                }

                NPCplanes[1].position = glm::vec3(0, 50, 0); 
                NPCplanes[1].forward  = glm::vec3(0, 0, -1);
                NPCplanes[1].up       = glm::vec3(0, 1, 0);
                NPCplanes[1].right    = glm::vec3(1, 0, 0);
                NPCplanes[1].pitchPlane = 0; 
                NPCplanes[1].yawPlane = 0; 
                NPCplanes[1].rollPlane = 0; 
                NPCplanes[1].position.x = 550; NPCplanes[1].position.z = -500; NPCplanes[1].position.y = 60; 
                yaw(NPCplanes[1], 90); 
                
                planeFragments.clear();
            }
            
            if(buttons.CheckHover(7)){ Add2DRect(680, 340, 800, 380, 29+10, 39+10, 56+10, 255); }else{ Add2DRect(680, 340, 800, 380, 29+40, 39+40, 56+40, 255); }
            PrintString("Exit", gameConfig.window_x-(480+80), 367, 2); 
            if(buttons.CheckClick(7)){ glfwTerminate(); force_close_program = true; }

        }

        MAIN_MENU(); 
        ControlsMenu(); 
        SETTINGS_MENU(); 

        LoadHeadingWindow(); 
        
        ControlPanel(plane); 
        
        FPS_Meter(); 

        if (!GUIBatchBuffer.empty()) {
            glUseProgram(uiShaderProgram); 
            glm::mat4 projection2D = glm::ortho(0.0f, gameConfig.window_x, 0.0f, gameConfig.window_y, -1.0f, 1.0f);
            glUniformMatrix4fv(glGetUniformLocation(uiShaderProgram, "mvp"), 1, GL_FALSE, glm::value_ptr(projection2D)); 
            glUniform4f(glGetUniformLocation(uiShaderProgram, "overlayColor"), 1.0f, 1.0f, 1.0f, 1.0f);
            glBindVertexArray(uiVAO);
            glBindBuffer(GL_ARRAY_BUFFER, uiVBO); 
            glBufferData(GL_ARRAY_BUFFER, GUIBatchBuffer.size() * sizeof(float), GUIBatchBuffer.data(), GL_DYNAMIC_DRAW); 
            glDrawArrays(GL_TRIANGLES, 0, GUIBatchBuffer.size() / 3);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        plane.forward = glm::normalize(plane.forward);
        plane.right   = glm::normalize(glm::cross(plane.forward, plane.up));
        plane.up      = glm::normalize(glm::cross(plane.right, plane.forward));
    }
    glfwTerminate();
    return;
}



