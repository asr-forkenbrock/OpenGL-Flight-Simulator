/*
HUD.cpp

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


std::string Config::INT_TO_STRING(int num) {
    if (num == 0) return "0";
    std::string str = "";
    bool isNegative = (num < 0);
    long long n = std::abs((long long)num);
    while (n > 0) {
        str += char((n % 10) + '0');
        n /= 10;
    }
    if (isNegative) str += '-'; 
    std::reverse(str.begin(), str.end());
    return str;
}

void HUD::SetupControlPanel(){
    static bool ControlPanelSetup; 
    if(!ControlPanelSetup){
        PrintString_Batched("GPS", 1185, 144, 2, GUIBatchBuffer); 
        PrintString_Batched("Fuel Tanks: ", 550, 87-21, 1, GUIBatchBuffer); 
        PrintString_Batched("EPR", 730, 115, 1, GUIBatchBuffer); 
        PrintString_Batched("EGT", 730, 50, 1, GUIBatchBuffer); 
        PrintString_Batched("N1", 570, 115, 1, GUIBatchBuffer); 
        PrintString_Batched("Auto Pilot", 403, 30, 1, GUIBatchBuffer); 
        PrintString_Batched("Flaps:", 385, 136, 1, GUIBatchBuffer); 
        PrintString_Batched("Gear 1:", 380, 118-7, 1, GUIBatchBuffer); 
        PrintString_Batched("Gear 2:", 380, 93-7, 1, GUIBatchBuffer); 
        PrintString_Batched("Gear 3:", 380, 68-7, 1, GUIBatchBuffer); 
        PrintString_Batched("Altitude: ", 250, 140, 1, GUIBatchBuffer); 
        PrintString_Batched("Airspeed (knots): ", 250, 100, 1, GUIBatchBuffer); 
        PrintString_Batched("Vertical Speed: ", 250, 60, 1, GUIBatchBuffer); 
        PrintString_Batched("Traffic ", 55, 140-2, 1, GUIBatchBuffer);  
        PrintString_Batched("Pull Up ", 55, 125-2, 1, GUIBatchBuffer);
        PrintString_Batched("Low Fuel ", 55, 110-2, 1, GUIBatchBuffer); 
        PrintString_Batched("Over Rev ", 155, 140-2, 1, GUIBatchBuffer); 
        PrintString_Batched("Over Speed ", 155, 125-2, 1, GUIBatchBuffer); 
        PrintString_Batched("Over Heat ", 155, 110-2, 1, GUIBatchBuffer); 
        PrintString_Batched("V1 Decision: ", 20, 75, 1, GUIBatchBuffer); 
        PrintString_Batched("VR Decision: ", 90, 75, 1, GUIBatchBuffer); 
        PrintString_Batched("Mach: ", 90+70, 75, 1, GUIBatchBuffer); 
        PrintString_Batched("Bat Voltage: ", 20, 42, 1, GUIBatchBuffer); 
        PrintString_Batched("Settings", (gameConfig.window_x-85)+20, 700-11, 1, GUIBatchBuffer); 
        PrintString_Batched("Select Airport", (gameConfig.window_x-85)+4, 665-7, 1, GUIBatchBuffer); 
        PrintString_Batched("F1", (gameConfig.window_x-85)+35, 665-18, 1, GUIBatchBuffer); 
        PrintString_Batched("Master Caut", 22, 689-35, 1, GUIBatchBuffer); 
        PrintString_Batched("Master Warn", 22, 689, 1, GUIBatchBuffer); 
        PrintString_Batched("Select Plane", (gameConfig.window_x-85)+10, 630-7, 1, GUIBatchBuffer); 
        PrintString_Batched("F2", (gameConfig.window_x-85)+35, 630-18, 1, GUIBatchBuffer); 
        PrintString_Batched("Controls", (gameConfig.window_x-85)+20, 584, 1, GUIBatchBuffer); 
        PrintString_Batched("Tank 1: ", 550, 70-21, 1, GUIBatchBuffer); 
        PrintString_Batched("Tank 2: ", 550, 50-21, 1, GUIBatchBuffer); 
        PrintString_Batched("Current X: ", 1140, 115, 1, GUIBatchBuffer); 
        PrintString_Batched("Current Y: ", 1140, 100, 1, GUIBatchBuffer); 
        PrintString_Batched("Current Z: ", 1140, 85, 1, GUIBatchBuffer); 
        PrintString_Batched("Destination X: ", 1140, 55, 1, GUIBatchBuffer); 
        PrintString_Batched("Destination Y: ", 1140, 40, 1, GUIBatchBuffer); 
        PrintString_Batched("Destination Z: ", 1140, 25, 1, GUIBatchBuffer); 
        PrintString_Batched("Pitch: ", 1005, 23, 1, GUIBatchBuffer); 
        PrintString_Batched("Bank: ", 1010, 143, 1, GUIBatchBuffer); 
        PrintString_Batched("Rudder: ", 850, 23, 1, GUIBatchBuffer); 
        PrintString_Batched("10", 530-18, 115, 1, GUIBatchBuffer); 
        PrintString_Batched("6", 530-6, 115+15, 1, GUIBatchBuffer); 
        PrintString_Batched("0", 530+10, 115-5, 1, GUIBatchBuffer); 
        PrintString_Batched("10", 625-18, 115, 1, GUIBatchBuffer); 
        PrintString_Batched("6", 625-6, 115+15, 1, GUIBatchBuffer); 
        PrintString_Batched("0", 625+10, 115-5, 1, GUIBatchBuffer); 
        PrintString_Batched("2", 690-18, 115, 1, GUIBatchBuffer); 
        PrintString_Batched("1.4", 690-3, 115+15, 1, GUIBatchBuffer); 
        PrintString_Batched("1", 690+10, 115-5, 1, GUIBatchBuffer); 
        PrintString_Batched("2", 785-18, 115, 1, GUIBatchBuffer); 
        PrintString_Batched("1.4", 785-3, 115+15, 1, GUIBatchBuffer); 
        PrintString_Batched("1", 785+10, 115-5, 1, GUIBatchBuffer); 
        ControlPanelSetup = true; 
    }
}

void HUD::ControlCompass(Model& plane, float Airport_Heading_X, float Airport_Heading_Z){
    Add2DRect(830, 150, 970, 10, 45, 67, 102, 255); 
    std::string ALTSTR = std::to_string(((plane.yawPlane*RAD_TO_DEG))); PrintString(ALTSTR, 850+(8*5), 23, 1); 
    Add2DCircle(900, 88, 60, 16, 0, 0, 0, 255);
    for(int i=0; i<360; i+=30){
        float rad = (i-int(plane.yawPlane*RAD_TO_DEG)) * (3.14159265f / 180.0f);
        if(i==90){
            Add2DLine(900+(cos(rad)*60), 88+(sin(rad)*60), 900, 88, 255, 0, 0, 255); 
            Add2DLine(900+(cos(rad)*40), 88+(sin(rad)*40), 900, 88, 0, 0, 0, 255);
        }else{
            Add2DLine(900+(cos(rad)*60), 88+(sin(rad)*60), 900, 88, 255, 255, 255, 255); 
            Add2DLine(900+(cos(rad)*50), 88+(sin(rad)*50), 900, 88, 0, 0, 0, 255);
        }
    }
    float targetX = Airport_Heading_X;
    float targetZ = Airport_Heading_Z;
    float diff_x = targetX - plane.position.x;
    float diff_z = targetZ - plane.position.z;
    float angleRadians = std::atan2(diff_z, diff_x)+ M_PI; 
    float centerX = 900.0f;
    float centerY = 88.0f;
    float length = 60.0f;
    float tipX = centerX + (cos(angleRadians) * length);
    float tipY = centerY + (sin(angleRadians) * length); 
    Add2DLine(tipX, tipY, centerX, centerY, 0, 255, 0, 255); 
}


void HUD::ControlADI(Model& plane){
        Add2DRect(980, 150, 1120, 10, 45, 67, 102, 255); 
        Add2DRect(985, 80, 1115, 81, 0, 0, 0, 255); 
        Add2DRect(990, 105, 1110, 104, 0, 0, 0, 255); 
        Add2DRect(990, 55, 1110, 54, 0, 0, 0, 255); 

        if(int(plane.rollPlane) <= 50 && int(plane.rollPlane) >= -50){
            int chng_p1 = 80-((int(plane.pitchPlane))+(int(plane.rollPlane))); 
            int chng_p2 = 80-((int(plane.pitchPlane))-(int(plane.rollPlane))); 
            if(chng_p1 > 130){ chng_p1=130; } if(chng_p1 < 10){ chng_p1=10; }
            if(chng_p2 > 130){ chng_p2=130; } if(chng_p2 < 10){ chng_p2=10; }
            Add2DRect4P(1000, chng_p1, 1100, chng_p2, 1100, 130, 1000, 130, 13, 153, 191, 255); 
            Add2DRect4P(1000, 30, 1100, 30, 1100, chng_p2, 1000, chng_p1, 84, 61, 10, 255); 
        }else if((int(plane.rollPlane) >= 129 && int(plane.rollPlane) <= 180)){
            int chng_p1 = ((int(plane.pitchPlane))-(int(plane.rollPlane)))+180+80; 
            int chng_p2 = ((int(plane.pitchPlane))+(int(plane.rollPlane)))-100; 
            if(chng_p1 > 130){ chng_p1=130; } if(chng_p1 < 10){ chng_p1=10; }
            if(chng_p2 > 130){ chng_p2=130; } if(chng_p2 < 10){ chng_p2=10; }
            Add2DRect4P(1000, chng_p1, 1100, chng_p2, 1100, 130, 1000, 130, 84, 61, 10, 255); 
            Add2DRect4P(1000, 30, 1100, 30, 1100, chng_p2, 1000, chng_p1, 13, 153, 191, 255); 
        }else if((int(plane.rollPlane) <= -129 && int(plane.rollPlane) >= -180)){
            int chng_p1 = ((int(plane.pitchPlane))-(int(plane.rollPlane)))-100; 
            int chng_p2 = ((int(plane.pitchPlane))+(int(plane.rollPlane)))+180+80;
            if(chng_p1 > 130){ chng_p1=130; } if(chng_p1 < 10){ chng_p1=10; }
            if(chng_p2 > 130){ chng_p2=130; } if(chng_p2 < 10){ chng_p2=10; }
            Add2DRect4P(1000, chng_p1, 1100, chng_p2, 1100, 130, 1000, 130, 84, 61, 10, 255); 
            Add2DRect4P(1000, 30, 1100, 30, 1100, chng_p2, 1000, chng_p1, 13, 153, 191, 255);
        }else if(int(plane.rollPlane) >= 50 && int(plane.rollPlane) <= 129){ 
            int chng_p1 = ((int(plane.pitchPlane))+(int(plane.rollPlane)))-40; 
            int chng_p2 = ((int(plane.pitchPlane))-(int(plane.rollPlane)))+90+50; 
            Add2DRect4P(1000, 30, 1100, 30, 1100, 130, 1000, 130, 13, 153, 191, 255);
            if(chng_p1 > 100){ chng_p1=100; } if(chng_p1 < 0){ chng_p1=0; }
            if(chng_p2 > 100){ chng_p2=100; } if(chng_p2 < 0){ chng_p2=0; }
            Add2DRect4P(1000+(chng_p1), 30, 1100, 30, 1100, 130, 1000+(chng_p2), 130, 84, 61, 10, 255);
        }else if(int(plane.rollPlane) <= -50 && int(plane.rollPlane) >= -129){
            int chng_p1 = ((int(plane.pitchPlane))+(int(plane.rollPlane)))+90+50; 
            int chng_p2 = ((int(plane.pitchPlane))-(int(plane.rollPlane)))-40; 
            Add2DRect4P(1000, 30, 1100, 30, 1100, 130, 1000, 130, 84, 61, 10, 255);
            if(chng_p1 > 100){ chng_p1=100; } if(chng_p1 < 0){ chng_p1=0; }
            if(chng_p2 > 100){ chng_p2=100; } if(chng_p2 < 0){ chng_p2=0; }
            Add2DRect4P(1100-(chng_p2), 30, 1100, 30, 1100, 130, 1100-(chng_p1), 130, 13, 153, 191, 255);
        }

        /*
        Add2DRect4P(1000, chng_p1-20, 1100, chng_p2-20, 1100, chng_p2-19, 1000, chng_p1-19, 13,13,13, 255);
        Add2DRect4P(1000, chng_p1-10, 1100, chng_p2-10, 1100, chng_p2-9, 1000, chng_p1-9, 13,13,13, 255);
        Add2DRect4P(1000, chng_p1-1, 1100, chng_p2-1, 1100, chng_p2+1, 1000, chng_p1+1, 13,13,13, 255);
        Add2DRect4P(1000, chng_p1+9, 1100, chng_p2+9, 1100, chng_p2+10, 1000, chng_p1+10, 13,13,13, 255);
        Add2DRect4P(1000, chng_p1+19, 1100, chng_p2+19, 1100, chng_p2+20, 1000, chng_p1+20, 13,13,13, 255);
        */
        Add2DRect(980, 150, 1120, 130, 45, 67, 102, 255); 
        Add2DRect(980, 30, 1120, 10, 45, 67, 102, 255); 
        std::string ALTSTR; 
        ALTSTR = std::to_string((plane.pitchPlane)); PrintString(ALTSTR, 1005+(7*5), 23, 1); 
        ALTSTR = std::to_string((plane.rollPlane)); PrintString(ALTSTR, 1010+(6*5), 143, 1); 
}


void FlightSimulation::ControlPanelGPS(Model& plane){
        std::string ALTSTR = INT_TO_STRING(int(plane.position.x)); 
        PrintString(ALTSTR, 1140+(11*5), 115, 1); 

        ALTSTR = INT_TO_STRING(int(plane.position.y)); 
        PrintString(ALTSTR, 1140+(11*5), 100, 1); 

        ALTSTR = INT_TO_STRING(int(plane.position.z)); 
        PrintString(ALTSTR, 1140+(11*5), 85, 1); 

        ALTSTR = INT_TO_STRING(int(Airport_Heading_X)); 
        PrintString(ALTSTR, 1140+(15*5), 55, 1); 

        ALTSTR = INT_TO_STRING(int(0)); 
        PrintString(ALTSTR, 1140+(15*5), 40, 1); 

        ALTSTR = INT_TO_STRING(int(Airport_Heading_Z)); 
        PrintString(ALTSTR, 1140+(15*5), 25, 1); 

}


void FlightSimulation::ControlPanelEngines(){
    Add2DRect(495, 150, 820, 10, 45, 67, 102, 255); 

        Add2DCircle(785, 115, 30, 16, 0, 0, 0, 255);
        Add2DCircleOutline(785, 115, 20, 16, 255, 255, 255, 255);
        for(int i=250; i>210; i-=1){ float rad = (i-30) * (3.14159265f / 180.0f); Add2DLine(785+(cos(rad)*27), 115+(sin(rad)*27), 785+(cos(rad)*22), 115+(sin(rad)*22), 255, 0, 0, 255); }
        float rad = ((((throttle)-(plane.pitchPlane/100))*200)-30) * (3.14159265f / 180.0f); 
        Add2DLine(785+(cos(rad)*27), 115+(sin(rad)*27), 785+(cos(rad)*5), 115+(sin(rad)*5), 155, 255, 0, 255); 
        Add2DRect(785-20, 115-15, 785+25, 115-30, 15, 167, 102, 255); 
        Add2DRect(785-19, 115-16, 785+24, 115-29, 0, 0, 0, 255);
        PrintString(std::to_string((1+throttle)-(plane.pitchPlane/100)), 785-18, 115-20, 1); 

        Add2DCircle(785, 45, 30, 16, 0, 0, 0, 255);
        Add2DCircleOutline(785, 45, 20, 16, 255, 255, 255, 255);
        for(int i=250; i>210; i-=1){ float rad = (i-30) * (3.14159265f / 180.0f); Add2DLine(785+(cos(rad)*27), 45+(sin(rad)*27), 785+(cos(rad)*22), 45+(sin(rad)*22), 255, 0, 0, 255); }
        rad = ((engine2_temp/10)-30) * (3.14159265f / 180.0f); 
        Add2DLine(785+(cos(rad)*27), 45+(sin(rad)*27), 785+(cos(rad)*5), 45+(sin(rad)*5), 155, 255, 0, 255); 
        Add2DRect(785-20, 45-15, 785+25, 45-30, 15, 167, 102, 255); 
        Add2DRect(785-19, 45-16, 785+24, 45-29, 0, 0, 0, 255);
        PrintString(INT_TO_STRING(int(engine2_temp))+" C", 785-18, 45-20, 1); 

        Add2DCircle(690, 115, 30, 16, 0, 0, 0, 255);
        Add2DCircleOutline(690, 115, 20, 16, 255, 255, 255, 255);
        for(int i=250; i>210; i-=1){ float rad = (i-30) * (3.14159265f / 180.0f); Add2DLine(690+(cos(rad)*27), 115+(sin(rad)*27), 690+(cos(rad)*22), 115+(sin(rad)*22), 255, 0, 0, 255); }
        rad = ((((throttle)-(plane.pitchPlane/100))*200)-30) * (3.14159265f / 180.0f); 
        Add2DLine(690+(cos(rad)*27), 115+(sin(rad)*27), 690+(cos(rad)*5), 115+(sin(rad)*5), 155, 255, 0, 255); 
        Add2DRect(690-20, 115-15, 690+25, 115-30, 15, 167, 102, 255); 
        Add2DRect(690-19, 115-16, 690+24, 115-29, 0, 0, 0, 255);
        PrintString(std::to_string((1+throttle)-(plane.pitchPlane/100)), 690-18, 115-20, 1); 

        Add2DCircle(690, 45, 30, 16, 0, 0, 0, 255);
        Add2DCircleOutline(690, 45, 20, 16, 255, 255, 255, 255);
        for(int i=250; i>210; i-=1){ float rad = (i-30) * (3.14159265f / 180.0f); Add2DLine(690+(cos(rad)*27), 45+(sin(rad)*27), 690+(cos(rad)*22), 45+(sin(rad)*22), 255, 0, 0, 255); }
        rad = ((engine1_temp/10)-30) * (3.14159265f / 180.0f); 
        Add2DLine(690+(cos(rad)*27), 45+(sin(rad)*27), 690+(cos(rad)*5), 45+(sin(rad)*5), 155, 255, 0, 255); 
        Add2DRect(690-20, 45-15, 690+25, 45-30, 15, 167, 102, 255); 
        Add2DRect(690-19, 45-16, 690+24, 45-29, 0, 0, 0, 255);
        PrintString(INT_TO_STRING(int(engine1_temp))+" C", 690-18, 45-20, 1); 

        Add2DCircle(625, 115, 30, 16, 0, 0, 0, 255);
        Add2DCircleOutline(625, 115, 20, 16, 255, 255, 255, 255);
        for(int i=250; i>210; i-=1){ float rad = (i-30) * (3.14159265f / 180.0f); Add2DLine(625+(cos(rad)*27), 115+(sin(rad)*27), 625+(cos(rad)*22), 115+(sin(rad)*22), 255, 0, 0, 255); }
        rad = ((throttle*205)-30) * (3.14159265f / 180.0f); 
        Add2DLine(625+(cos(rad)*27), 115+(sin(rad)*27), 625+(cos(rad)*5), 115+(sin(rad)*5), 155, 255, 0, 255); 
        Add2DRect(625-20, 115-15, 625+25, 115-30, 15, 167, 102, 255); 
        Add2DRect(625-19, 115-16, 625+24, 115-29, 0, 0, 0, 255);
        PrintString(INT_TO_STRING(int(throttle*100))+" %", 625-18, 115-20, 1); 

        Add2DCircle(530, 115, 30, 16, 0, 0, 0, 255);
        Add2DCircleOutline(530, 115, 20, 16, 255, 255, 255, 255);
        for(int i=250; i>210; i-=1){ float rad = (i-30) * (3.14159265f / 180.0f); Add2DLine(530+(cos(rad)*27), 115+(sin(rad)*27), 530+(cos(rad)*22), 115+(sin(rad)*22), 255, 0, 0, 255); }
        rad = ((throttle*205)-30) * (3.14159265f / 180.0f); 
        Add2DLine(530+(cos(rad)*27), 115+(sin(rad)*27), 530+(cos(rad)*5), 115+(sin(rad)*5), 155, 255, 0, 255); 
        Add2DRect(530-20, 115-15, 530+25, 115-30, 15, 167, 102, 255); 
        Add2DRect(530-19, 115-16, 530+24, 115-29, 0, 0, 0, 255);
        PrintString(INT_TO_STRING(int(throttle*100))+" %", 530-18, 115-20, 1); 

        Add2DRect(510, 70-16, 510+140, 70-32, 0, 0, 0, 255);
        if(tank1 < 0){ tank1=0; }
        Add2DRect(512, 70-18, 510+(tank1*1.38), 70-30, 0, 100, 0, 255);
        PrintString(INT_TO_STRING(int(tank1))+" %", 550+(8*5), 70-21, 1); 

        Add2DRect(510, 50-16, 510+140, 50-32, 0, 0, 0, 255);
        if(tank2 < 0){ tank2=0; }
        Add2DRect(512, 50-18, 510+(tank2*1.38), 50-30, 0, 100, 0, 255);
        PrintString(INT_TO_STRING(int(tank2))+" %", 550+(8*5), 50-21, 1); 
    
}


void FlightSimulation::ControlPanelErrorLights(){
    Add2DRect(10, 150, 230, 90, 45, 67, 102, 255); 
    if(flash_led_warns[0]){ Add2DRect(20, 140, 50, 130, 255, 0, 0, 255);   }else{ Add2DRect(20, 140, 50, 130, 0, 0, 0, 255);   }
    if(flash_led_warns[1]){ Add2DRect(20, 125, 50, 115, 255, 0, 0, 255);   }else{ Add2DRect(20, 125, 50, 115, 0, 0, 0, 255);   }   
    if(flash_led_warns[2]){ Add2DRect(20, 110, 50, 100, 255, 0, 0, 255);   }else{ Add2DRect(20, 110, 50, 100, 0, 0, 0, 255);   } 
    if(flash_led_warns[3]){ Add2DRect(120, 140, 150, 130, 255, 0, 0, 255); }else{ Add2DRect(120, 140, 150, 130, 0, 0, 0, 255); } 
    if(flash_led_warns[4]){ Add2DRect(120, 125, 150, 115, 255, 0, 0, 255); }else{ Add2DRect(120, 125, 150, 115, 0, 0, 0, 255); } 
    if(flash_led_warns[5]){ Add2DRect(120, 110, 150, 100, 255, 0, 0, 255); }else{ Add2DRect(120, 110, 150, 100, 0, 0, 0, 255); } 


        if(MAST_WARN){ 
            if(warn_counter_flash>=10){ 
                warn_counter_flash=0; if(warn_flash_ON){ warn_flash_ON=false; }else{ warn_flash_ON=true; }
            } 
            if(warn_flash_ON){ Add2DRect(10, 700, 90, 670, 200, 0, 0, 255); }else{ Add2DRect(10, 700, 90, 670, 50, 0, 0, 120); }
            warn_counter_flash++; 
        }else{ Add2DRect(10, 700, 90, 670, 50, 0, 0, 120); }
        
        if(MAST_CAUT){ 
            if(caut_counter_flash>=10){ 
                caut_counter_flash=0; if(caut_flash_ON){ caut_flash_ON=false; }else{ caut_flash_ON=true; }
            } 
            if(caut_flash_ON){ Add2DRect(10, 665, 90, 635, 192, 182, 6, 255); }else{ Add2DRect(10, 665, 90, 635, 92, 82, 6, 120); }
            caut_counter_flash++; 
        }else{ Add2DRect(10, 665, 90, 635, 92, 82, 6, 120); }

}


void FlightSimulation::ControlPanelFlightData1(){
    Add2DRect(375, 150, 485, 10, 45, 67, 102, 255); 
            Add2DRect(380, 40, 480, 15, 205, 15, 15, 255); 
            if(autopilot_engaged){ Add2DRect(383, 37, 477, 18, 225, 15, 15, 255); }else{ Add2DRect(383, 37, 477, 18, 65, 15, 15, 255); } 

            Add2DRect(420+1, 143-1, 465-1, 123+1, 6, 9, 2, 255); 
            if(flaps_active){ PrintString("DOWN", 430, 136, 1); }else{ PrintString(" UP ", 430, 136, 1); }

            if(gear_active){
                Add2DRect(420+1, 118-1, 465-1, 98+1, 6, 69, 2, 255); PrintString("LOCKED", 427, 111, 1); 
                Add2DRect(420+1, 93-1, 465-1, 73+1, 6, 69, 2, 255);  PrintString("LOCKED", 427, 86, 1); 
                Add2DRect(420+1, 68-1, 465-1, 48+1, 6, 69, 2, 255);  PrintString("LOCKED", 427, 61, 1); 
            }else{
                Add2DRect(420+1, 118-1, 465-1, 98+1, 106, 39, 2, 255); PrintString(" UP ", 430, 111, 1); 
                Add2DRect(420+1, 93-1, 465-1, 73+1, 106, 39, 2, 255);  PrintString(" UP ", 430, 86, 1); 
                Add2DRect(420+1, 68-1, 465-1, 48+1, 106, 39, 2, 255);  PrintString(" UP ", 430, 61, 1); 
            }
            
        Add2DRect(240, 150, 365, 10, 45, 67, 102, 255); 
        
        Add2DRect(250, 130, 350, 110, 95, 95, 95, 255); 
        int alt = int(plane.position.y - PlaneConfigs[PlaneType].planes_height); 
        for(int i=6; i>=0; i--){
            int val = (alt % 10); 
            Add2DRect(252+(i*14), 128, 264+(i*14), 112, 5, 5, 5, 255); 
            PrintString(INT_TO_STRING(val), 252+(i*14), 126, 2); 
            alt /= 10;
        }

        
        Add2DRect(250, 90, 350, 70, 95, 95, 95, 255); 
        int aspd = int(airspeed*2); 
        for(int i=6; i>=0; i--){
            int val = (aspd % 10); 
            Add2DRect(252+(i*14), 88, 264+(i*14), 72, 5, 5, 5, 255); 
            PrintString(INT_TO_STRING(val), 252+(i*14), 86, 2); 
            aspd /= 10;
        }

        
        Add2DRect(250, 50, 350, 30, 95, 95, 95, 255); 
        int vsi = int(fall_speed); 
        for(int i=6; i>=0; i--){
            int val = (vsi % 10); 
            Add2DRect(252+(i*14), 48, 264+(i*14), 32, 5, 5, 5, 255); 
            PrintString(INT_TO_STRING(val), 252+(i*14), 46, 2); 
            vsi /= 10;
        }
    
}


void FlightSimulation::ControlPanelFlightData2(){
Add2DRect(10, 80, 230, 10, 45, 67, 102, 255); 


        Add2DRect(20, 65, 78, 45, 95, 95, 95, 255); 
        {int vsi = int(PlaneConfigs[PlaneType].plane_v1_speed); 
        for(int i=3; i>=0; i--){
            int val = (vsi % 10); 
            Add2DRect(22+(i*14), 63, 34+(i*14), 47, 5, 5, 5, 255); 
            PrintString(INT_TO_STRING(val), 22+(i*14), 61, 2); 
            vsi /= 10;
        }}

        Add2DRect(90, 65, 148, 45, 95, 95, 95, 255); 
        {int vsi = int(PlaneConfigs[PlaneType].plane_vr_speed); 
        for(int i=3; i>=0; i--){
            int val = (vsi % 10); 
            Add2DRect(92+(i*14), 63, 104+(i*14), 47, 5, 5, 5, 255); 
            PrintString(INT_TO_STRING(val), 92+(i*14), 61, 2); 
            vsi /= 10;
        }}


        Add2DRect(90+70, 65, 148+70, 45, 95, 95, 95, 255); 
        {int vsi = int((airspeed * 0.00149984)*200); 
        for(int i=3; i>=0; i--){
            int val = (vsi % 10); 
            Add2DRect(92+70+(i*14), 63, 104+70+(i*14), 47, 5, 5, 5, 255); 
            if(i == 1){ PrintString(".", 92+70+(i*14), 61, 2); }else{ PrintString(INT_TO_STRING(val), 92+70+(i*14), 61, 2); }
            vsi /= 10;
        }}


        Add2DRect(20, 35, 78, 15, 95, 95, 95, 255); 
        {   
        float vsi = float(PlaneConfigs[PlaneType].plane_battery_voltages); 
        float var = vsi - int(vsi);  
        for(int i=3; i>=0; i--){ Add2DRect(22+(i*14), 33, 34+(i*14), 17, 5, 5, 5, 255); }

        for(int i=1; i>=0; i--){
            PrintString(INT_TO_STRING((int(vsi) % 10)), 22+(i*14), 31, 2); vsi /= 10;
        }

        PrintString(".", 22+(2*14), 31, 2); 

        PrintString(INT_TO_STRING(int(var*10)), 22+(3*14), 31, 2); 
        
        }
    
}


void FlightSimulation::ControlPanel(Model& plane){
    Add2DRect(1130, 150, 1270, 10, 45, 67, 102, 255); 
    SetupControlPanel(); 
    ControlPanelGPS(plane); 
    ControlADI(plane); 
    ControlCompass(plane, Airport_Heading_X, Airport_Heading_Z);  
    ControlPanelFlightData1();
    ControlPanelFlightData2();
    ControlPanelEngines(); 
    ControlPanelErrorLights(); 
    Add2DRect(gameConfig.window_x-85, 700, gameConfig.window_x-5, 670, 50, 100, 100, 180); //settings button
    Add2DRect(gameConfig.window_x-85, 665, gameConfig.window_x-5, 635, 50, 100, 100, 180); //select airport 
    Add2DRect(gameConfig.window_x-85, 630, gameConfig.window_x-5, 600, 50, 100, 100, 180); //select plane
    Add2DRect(gameConfig.window_x-85, 595, gameConfig.window_x-5, 565, 50, 100, 100, 180); //controls button
}


void Menus::ControlsMenu(){
    if(controls_menu_active){
        Add2DRect(348, 298, gameConfig.window_x-348, 522, 129, 139, 156, 255); 
        Add2DRect(350, 300, gameConfig.window_x-350, 520, 29, 39, 56, 255); 
        PrintString("Controls", 360, 510, 3); 

        PrintString("F: Flaps", 380, 470, 2); 
        PrintString("Arrow up/down: Throttle", 380, 450, 2); 
        PrintString("Arrow left/right: Bank", 380, 430, 2); 
        PrintString("A/D: Rudder", 380, 410, 2); 
        PrintString("W/S: Pitch", 380, 390, 2); 
        PrintString("Space: Camera Angle", 380, 370, 2); 
        
        PrintString("G: Landing Gear", 690, 470, 2); 
        PrintString("Esc: Pause Game", 690, 450, 2); 
        PrintString("B: Brakes", 690, 430, 2); 
        PrintString("L: Engage Autopilot", 690, 410, 2); 
        PrintString("F1: Change Heading", 690, 390, 2); 
        PrintString("F2: Change Plane", 690, 370, 2); 

        if(buttons.CheckHover(5)){
            Add2DRect(575, 310, gameConfig.window_x-575, 335, 29+10, 39+10, 56+10, 255); 
        }else{
            Add2DRect(575, 310, gameConfig.window_x-575, 335, 29+40, 39+40, 56+40, 255); 
        }
        PrintString("Close", 610, 330, 2);
    }
}


void Menus::MAIN_MENU(){
    if(main_menu_active){
        Add2DRect(398, 298, gameConfig.window_x-398, 502, 129, 139, 156, 255); 
        Add2DRect(400, 300, gameConfig.window_x-400, 500, 29, 39, 56, 255); 
        PrintString("Game Paused", 470, 480, 6); 

        if(buttons.CheckHover(6)){ Add2DRect(480, 340, 600, 380, 29+10, 39+10, 56+10, 255); }else{ Add2DRect(480, 340, 600, 380, 29+40, 39+40, 56+40, 255); }
        PrintString("Resume", 510, 367, 2);
        if(buttons.CheckClick(6)){ main_menu_active = false; }

        if(buttons.CheckHover(7)){ Add2DRect(680, 340, 800, 380, 29+10, 39+10, 56+10, 255); }else{ Add2DRect(680, 340, 800, 380, 29+40, 39+40, 56+40, 255); }
        PrintString("Exit", gameConfig.window_x-(480+80), 367, 2); 
        if(buttons.CheckClick(7)){ glfwTerminate(); force_close_program = true; }
    }
}


void FlightSimulation::SETTINGS_MENU(){
    if(settings_menu_active){
        std::string tmp; 
        Add2DRect(198, 198, gameConfig.window_x-198, 602, 129, 139, 156, 255); 
        Add2DRect(200, 200, gameConfig.window_x-200, 600, 29, 39, 56, 255); 

        Add2DRect(639, 270, 640, 590, 129, 139, 156, 255); 

        Add2DRect(210, 260-1, gameConfig.window_x-210, 260+1, 129, 139, 156, 255); 

        PrintString("Settings", 210, 580, 4); 

        PrintString("Plane: ", 245, 515, 2); 
        if((buttons.CheckHover(9) && !airport_drop_down) or plane_drop_down){ Add2DRect(315, 500, 485, 520, 29+10, 39+10, 56+10, 255); }else{ Add2DRect(315, 500, 485, 520, 29+40, 39+40, 56+40, 255); }
        PrintString(PlaneConfigs[PlaneType].name, 320, 515, 2); 
        PrintString("^", 471, 510, 1);
        if(buttons.CheckClick(9) && !airport_drop_down){ 
            if(!plane_drop_down){ 
                plane_drop_down = true; 
                drop_down_click_start = drop_down_click_current = buttons.Size(); 
                for(int i=0; i<PlaneConfigs.size(); i++){ buttons.AddButton(319, 488, 202+20+(i*20), 217+20+(i*20)); } 
                drop_down_click_current = buttons.Size(); 
            }else{ 
                while(drop_down_click_start != drop_down_click_current){ buttons.RemoveLastButton(); drop_down_click_current--; }
                plane_drop_down = false; drop_down_click_current = drop_down_click_start = 0; 
            } 
        }

        PrintString("Airport: ", 245, 465, 2); 
        if((buttons.CheckHover(10) && !plane_drop_down) or airport_drop_down){ Add2DRect(335, 450, 505, 470, 29+10, 39+10, 56+10, 255); }else{ Add2DRect(335, 450, 505, 470, 29+40, 39+40, 56+40, 255); }
        PrintString(headings[int(heading_select)].name, 345, 465, 2); 
        PrintString("^", 490, 460, 1);
        if(buttons.CheckClick(10) && !plane_drop_down){ 
            if(!airport_drop_down){ 
                airport_drop_down = true; 
                drop_down_click_start = drop_down_click_current = buttons.Size(); 
                for(int i=0; i<headings.size(); i++){ buttons.AddButton(339, 507, 252+20+(i*20), 273+20+(i*20)); } 
                drop_down_click_current = buttons.Size(); 
            }else{ 
                while(drop_down_click_start != drop_down_click_current){ buttons.RemoveLastButton(); drop_down_click_current--; }
                airport_drop_down = false; drop_down_click_current = drop_down_click_start = 0; 
            } 
        }

        if(plane_drop_down){
            Add2DRect(315, 500, 485, 500-(PlaneConfigs.size()*20), 29+40, 39+40, 56+40, 255);
            for(int p=drop_down_click_start; p<drop_down_click_current; p++){
                if(buttons.CheckHover(p)){ Add2DRect(317, 499-((p-drop_down_click_start)*20), 483, 481-((p-drop_down_click_start)*20), 3, 78, 252, 255); }
                if(buttons.CheckClick(p)){
                    PlaneType = p - drop_down_click_start; LoadNewPlane(PlaneType); 
                    while(drop_down_click_start != drop_down_click_current){ buttons.RemoveLastButton(); drop_down_click_current--; }
                    plane_drop_down = false; drop_down_click_current = drop_down_click_start = 0; 
                }
            }
            for(int i=0; i<PlaneConfigs.size(); i++){ PrintString(PlaneConfigs[i].name, 320, 495-(i*20), 2); }
        }





        tmp = "Current Plane Scale: " + std::to_string(PlaneConfigs[PlaneType].planeScale); 
        PrintString(tmp, 245, 405, 2); 
        tmp = "Current Plane Thrust: " + std::to_string(PlaneConfigs[PlaneType].plane_thrust); 
        PrintString(tmp, 245, 375, 2); 
        tmp = "Current ground offset: " + std::to_string(PlaneConfigs[PlaneType].planes_height); 
        PrintString(tmp, 245, 345, 2); 

        tmp = "Active NPCs: " + std::to_string(active_npc); 
        PrintString(tmp, 245, 305, 2); 


        PrintString("Configuration", 650, 580, 4); 
        tmp = "Window Resolution: " + std::to_string(int(gameConfig.window_x)) + " x " + std::to_string(int(gameConfig.window_y)); 
        PrintString(tmp, 685, 515, 2);

        tmp = "House Count: " + std::to_string(EnvironmentConfig.house_count); 
        PrintString(tmp, 685, 470, 2);
        tmp = "Water Bodies Count: " + std::to_string(EnvironmentConfig.water_count); 
        PrintString(tmp, 685, 440, 2);
        tmp = "Apartments Count: " + std::to_string(EnvironmentConfig.apartment_count); 
        PrintString(tmp, 685, 410, 2);
        tmp = "Plants Count: " + std::to_string(EnvironmentConfig.tree_count); 
        PrintString(tmp, 685, 380, 2);
        tmp = "Cities Count: " + std::to_string(headings.size()); 
        PrintString(tmp, 685, 350, 2);

        PrintString("Version: 29.18", 685, 295, 2);



        if(airport_drop_down){
            Add2DRect(335, 450, 505, 450-(headings.size()*20), 29+40, 39+40, 56+40, 255);
            for(int p=drop_down_click_start; p<drop_down_click_current; p++){
                if(buttons.CheckHover(p)){ Add2DRect(337, 449-((p-drop_down_click_start)*20), 503, 431-((p-drop_down_click_start)*20), 3, 78, 252, 255); }
                if(buttons.CheckClick(p)){
                    heading_select = p - drop_down_click_start; 
                    while(drop_down_click_start != drop_down_click_current){ buttons.RemoveLastButton(); drop_down_click_current--; }
                    airport_drop_down = false; drop_down_click_current = drop_down_click_start = 0; 
                }
            }
            for(int i=0; i<headings.size(); i++){ PrintString(headings[i].name, 345, 445-(i*20), 2); }
        }



        if(buttons.CheckHover(1)){
            Add2DRect(575, 220, gameConfig.window_x-575, 245, 29+10, 39+10, 56+10, 255); 
        }else{
            Add2DRect(575, 220, gameConfig.window_x-575, 245, 29+40, 39+40, 56+40, 255); 
        }
        PrintString("Close", 610, 240, 2);
    }
}