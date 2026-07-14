/*
graphic_tools.cpp

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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void Graphics_3D::load_shaders(){
    shaderProgram = TEXTUREcreateShaderProgram();
    Logger.push_back("  - texture shader created"); Refresh2d(); 
    globalShaderProgram = createShaderProgram();
    Logger.push_back("  - global shader created"); Refresh2d(); 
    OBJTEXTUREglobalShaderProgram = OBJTEXTUREcreateShaderProgram();
    Logger.push_back("  - object shader created"); Refresh2d(); 
}

std::vector<float> Graphics_3D::loadSTL(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return {};
    file.seekg(80);
    uint32_t numTriangles;
    file.read(reinterpret_cast<char*>(&numTriangles), 4);
    std::vector<float> vertices;
    for (uint32_t i = 0; i < numTriangles; ++i) {
        float n[3];
        file.read(reinterpret_cast<char*>(n), 12);
        for (int v = 0; v < 3; ++v) {
            float pos[3];
            file.read(reinterpret_cast<char*>(pos), 12);
            vertices.push_back(pos[0]); vertices.push_back(pos[1]); vertices.push_back(pos[2]);
            vertices.push_back(n[0]);   vertices.push_back(n[1]);   vertices.push_back(n[2]);
        }
        file.seekg(2, std::ios::cur);
    }
    return vertices;
}

unsigned int Graphics_3D::OBJTEXTUREloadTexture(const std::string& path) {
    Logger.push_back("  - "+path); Refresh2d(); 
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    
    if(data){
        GLenum format = (nrComponents == 4) ? GL_RGBA : GL_RGB;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }else{
        std::cout << "Texture failed to load: " << path << "\n";
        stbi_image_free(data);
    }
    return textureID;
}


void Graphics_3D::MatrixSharing(Model& plane, const std::vector<CameraAnglesStruct>& CamAngles, int cam_ang){ 
    projection = glm::perspective(glm::radians(45.0f), gameConfig.window_x/gameConfig.window_y, 1.0f, 20000.0f);
    glm::vec3 direction = plane.forward;
    if(std::abs(direction.y) > 0.99f){ direction = plane.up; }
    glm::vec3 flatForward = glm::normalize(glm::vec3(direction.x+CamAngles[cam_ang].FX, CamAngles[cam_ang].FY, direction.z+CamAngles[cam_ang].FZ)); 
    glm::vec3 camPos = plane.position - (flatForward * (600.0f + CamAngles[cam_ang].ds)) + (glm::vec3(0+CamAngles[cam_ang].cX, 150.0f+CamAngles[cam_ang].cY, CamAngles[cam_ang].cZ)); 
    glm::vec3 lookTarget = plane.position + (plane.forward * 50.0f);
    view = glm::lookAt(camPos, lookTarget, glm::vec3(0, 1, 0));
}



void Graphics_2D::InitUI(){ 
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &uiVertexShaderSource, NULL);
    glCompileShader(vShader);

    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &uiFragmentShaderSource, NULL);
    glCompileShader(fShader);

    uiShaderProgram = glCreateProgram();
    glAttachShader(uiShaderProgram, vShader);
    glAttachShader(uiShaderProgram, fShader);
    glLinkProgram(uiShaderProgram);

    glGenVertexArrays(1, &uiVAO);
    glGenBuffers(1, &uiVBO);
    glBindVertexArray(uiVAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 6, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    uiTextureShaderProgram = glCreateProgram();
    glGenVertexArrays(1, &uiTexVAO);
    glGenBuffers(1, &uiTexVBO);
    glBindVertexArray(uiTexVAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiTexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int vtShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vtShader, 1, &uiTexVertexShaderSource, NULL);
    glCompileShader(vtShader);

    unsigned int ftShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(ftShader, 1, &uiTexFragmentShaderSource, NULL);
    glCompileShader(ftShader);

    glAttachShader(uiTextureShaderProgram, vtShader);
    glAttachShader(uiTextureShaderProgram, ftShader);
    glLinkProgram(uiTextureShaderProgram);

    projection2D = glm::ortho(0.0f, gameConfig.window_x, 0.0f, gameConfig.window_y, -1.0f, 1.0f);
    uiMVPLoc = glGetUniformLocation(uiShaderProgram, "mvp");
    uiColorLoc = glGetUniformLocation(uiShaderProgram, "overlayColor");
    uiTexMVPLoc = glGetUniformLocation(uiTextureShaderProgram, "mvp");
    uiTexSamplerLoc = glGetUniformLocation(uiTextureShaderProgram, "uiTexture");
}

void Graphics_2D::setupWindow(){
    if(!glfwInit()) return;
    window = glfwCreateWindow(gameConfig.window_x, gameConfig.window_y, "ASR Flight Simulator", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewInit();
}

void Graphics_2D::Add2DRect(float x1, float y1, float x2, float y2, int R, int G, int B, int A){
    glUseProgram(uiShaderProgram); 

    glUniformMatrix4fv(uiMVPLoc, 1, GL_FALSE, glm::value_ptr(projection2D));

    float uiVertices[] = {
        x1, y1, 0.0f, x2, y1, 0.0f, x2, y2, 0.0f,
        x1, y1, 0.0f, x2, y2, 0.0f, x1, y2, 0.0f
    };

    glUniform4f(uiColorLoc, R/255.0f, G/255.0f, B/255.0f, A/255.0f);
    
    glBindVertexArray(uiVAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uiVertices), uiVertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Graphics_2D::TEXT(char letter, int x, int y, int scale, int R, int G, int B){ 
    if(letter >= 128) return;
    for(int FCY = 0; FCY <= 7; FCY++){ 
        for(int FCX = 0; FCX <= 5; FCX++){
            if(FONT6x8[letter][FCX] & (1 << (FCY))){
                int varx = x + FCX*scale; int vary = y - FCY*scale; 
                Add2DRect((varx-scale), vary-scale, varx, vary, R, G, B, 255); 
            }
        }
    }
}

void Graphics_2D::Add2DLine(float x1, float y1, float x2, float y2, int R, int G, int B, int A){
    glUseProgram(uiShaderProgram);
    glUniformMatrix4fv(uiMVPLoc, 1, GL_FALSE, glm::value_ptr(projection2D));

    float uiVertices[] = {
        x1, y1, 0.0f,
        x2, y2, 0.0f
    };

    glUniform4f(uiColorLoc, R/255.0f, G/255.0f, B/255.0f, A/255.0f);
    glBindVertexArray(uiVAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uiVertices), uiVertices);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}


void Graphics_2D::Add2DCircleOutline(float centerX, float centerY, float radius, int segments, int R, int G, int B, int A){
    glUseProgram(uiShaderProgram);
    glUniformMatrix4fv(uiMVPLoc, 1, GL_FALSE, glm::value_ptr(projection2D));
    std::vector<float> vertices;

    for(int i = 0; i < segments; i++){
        float angle = glm::radians((float)i / segments * 360.0f);
        float x = centerX + cos(angle) * radius;
        float y = centerY + sin(angle) * radius;

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }

    glUniform4f(uiColorLoc, R/255.0f, G/255.0f, B/255.0f, A/255.0f);
    glBindVertexArray(uiVAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
    glDrawArrays(GL_LINE_LOOP, 0, segments);
    glBindVertexArray(0);
}



void Graphics_2D::Add2DCircle(float centerX, float centerY, float radius, int segments, int R, int G, int B, int A){
    glUseProgram(uiShaderProgram);
    glUniformMatrix4fv(uiMVPLoc, 1, GL_FALSE, glm::value_ptr(projection2D));
    std::vector<float> vertices;

    vertices.push_back(centerX);
    vertices.push_back(centerY);
    vertices.push_back(0.0f);

    for(int i=0; i<=segments; i++){
        float angle = glm::radians((float)i / segments * 360.0f);
        float x = centerX + cos(angle) * radius;
        float y = centerY + sin(angle) * radius;
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }

    glUniform4f(uiColorLoc, R/255.0f, G/255.0f, B/255.0f, A/255.0f);
    glBindVertexArray(uiVAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);

    glBindVertexArray(0);
}







void Graphics_2D::Add2DRect4P(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int R, int G, int B, int A){
    glUseProgram(uiShaderProgram);
    glUniformMatrix4fv(uiMVPLoc, 1, GL_FALSE, glm::value_ptr(projection2D));
    float uiVertices[] = {
        x1, y1, 0.0f,
        x2, y2, 0.0f,
        x3, y3, 0.0f,
        x1, y1, 0.0f,
        x3, y3, 0.0f,
        x4, y4, 0.0f
    };
    glUniform4f(uiColorLoc, R/255.0f, G/255.0f, B/255.0f, A/255.0f);
    glBindVertexArray(uiVAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(uiVertices), uiVertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Graphics_2D::AppendRectVertices(std::vector<float>& vertexBuffer, float x1, float y1, float x2, float y2){
    float rect[] = {
        x1, y1, 0.0f, x2, y1, 0.0f, x2, y2, 0.0f,
        x1, y1, 0.0f, x2, y2, 0.0f, x1, y2, 0.0f
    };
    vertexBuffer.insert(vertexBuffer.end(), rect, rect + 18);
}

void Graphics_2D::TEXT_Batched(char letter, int x, int y, int scale, std::vector<float>& buffer){
    if(letter >= 128) return;
    for(int FCY = 0; FCY <= 7; FCY++){ 
        for(int FCX = 0; FCX <= 5; FCX++){
            if(FONT6x8[letter][FCX] & (1 << (FCY))){
                int varx = x + FCX * scale; 
                int vary = y - FCY * scale; 
                AppendRectVertices(buffer, varx - scale, vary - scale, varx, vary); 
            }
        }
    }
}

void Graphics_2D::PrintString_Batched(std::string STRING, int x, int y, int scale, std::vector<float>& buffer){
    for(int i = 0; i < STRING.length(); i++){
        TEXT_Batched(STRING[i], x + (i * (scale * 5)), y, scale, buffer); 
    }
}

void Graphics_2D::DrawUIElement(unsigned int textureID, float x, float y, float w, float h){
    if(textureID == 0) std::cout << "Warning: Texture ID is 0 (not loaded)" << "\n";
    glUseProgram(uiTextureShaderProgram); 
    glUniformMatrix4fv(uiTexMVPLoc, 1, GL_FALSE, glm::value_ptr(projection2D));

    float vertices[] = {
        x,     y + h,   0.0f, 1.0f, 
        x,     y,       0.0f, 0.0f, 
        x + w, y,       1.0f, 0.0f, 
        x,     y + h,   0.0f, 1.0f, 
        x + w, y,       1.0f, 0.0f, 
        x + w, y + h,   1.0f, 1.0f  
    };

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(uiTexSamplerLoc, 0);

    glBindVertexArray(uiTexVAO);
    glBindBuffer(GL_ARRAY_BUFFER, uiTexVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Graphics_2D::Refresh2d(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT); 
    Add2DRect(0, gameConfig.window_y, gameConfig.window_x, 0, 35, 0, 60, 255); 
    
    for(int i=0; i<Logger.size(); i++){
        PrintString_Batched(Logger[i], 0, 710-(i*12), 1, uiBatchBuffer); 
    }

    if(!uiBatchBuffer.empty()){
        glUseProgram(uiShaderProgram);
        glUniformMatrix4fv(uiMVPLoc, 1, GL_FALSE, glm::value_ptr(projection2D));
        glUniform4f(uiColorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        glBindVertexArray(uiVAO);
        glBindBuffer(GL_ARRAY_BUFFER, uiVBO); 
        glBufferData(GL_ARRAY_BUFFER, uiBatchBuffer.size() * sizeof(float), uiBatchBuffer.data(), GL_DYNAMIC_DRAW); 
        glDrawArrays(GL_TRIANGLES, 0, uiBatchBuffer.size() / 3);
    }
    DrawUIElement(IntroImageTexture, 500, 0, 800, 800); 
    PrintString("Loading Flight Simulator...", 550, 620, 5); 

    glfwSwapBuffers(window);
    glfwPollEvents(); 
};
