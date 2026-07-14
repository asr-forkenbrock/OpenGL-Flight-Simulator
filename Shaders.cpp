/*
Shaders.cpp

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

#include "Shaders.h"

const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    uniform mat4 mvp;
    out vec3 fragNormal;
    void main() {
        gl_Position = mvp * vec4(aPos, 1.0);
        fragNormal = aNormal; 
    }
)glsl";

const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    in vec3 fragNormal;
    uniform vec4 overlayColor; 
    void main() {
        vec3 sunDir = normalize(vec3(0.5, 1.0, 0.3));
        float diff = max(dot(normalize(fragNormal), sunDir), 0.2);
        FragColor = vec4(overlayColor.rgb * diff, overlayColor.a); 
    }
)glsl";

const char* uiVertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 mvp;
    void main() {
        gl_Position = mvp * vec4(aPos, 1.0);
    }
)glsl";

const char* uiFragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    uniform vec4 overlayColor;
    void main() {
        FragColor = overlayColor; 
    }
)glsl";


const char* TEXTUREvertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    uniform mat4 mvp;
    out vec2 TexCoord;

    void main() {
        gl_Position = mvp * vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)glsl";

const char* TEXTUREfragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    in vec2 TexCoord;

    uniform sampler2D ourTexture;

    void main() {
        FragColor = texture(ourTexture, TexCoord * 0.015);
    }
)glsl";



const char* OBJTEXTUREvertexShaderSourcePOSITION = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    
    uniform mat4 mvp;
    
    out vec3 fragNormal;
    out vec3 localPos; 

    void main() {
        gl_Position = mvp * vec4(aPos, 1.0);
        fragNormal = aNormal;
        localPos = aPos;   
    }
)glsl";

const char* OBJTEXTUREfragmentShaderSourcePOSITION = R"glsl(
    #version 330 core
    out vec4 FragColor;
    
    in vec3 fragNormal;
    in vec3 localPos; 
    
    uniform vec4 overlayColor; 

    void main() {
        vec3 sunDir = normalize(vec3(0.5, 1.0, 0.3));
        float diff = max(dot(normalize(fragNormal), sunDir), 0.2);
        
        vec3 finalColor = vec3(0.8, 0.8, 0.8);

        if (localPos.z > 45.0) {
            finalColor = vec3(0.1, 0.1, 0.1); 
        }

        if (abs(localPos.y - 1.5) < 0.3) {
            finalColor = vec3(0.0, 0.2, 0.8); 
        }
        
        float pattern = sin(localPos.z * 0.5) * cos(localPos.x * 0.5);
        if (pattern > 0.8) {
            finalColor *= 0.9; 
        }

        FragColor = vec4(finalColor * diff, 1.0); 
    }
)glsl";



const char* OBJTEXTUREvertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;

    uniform mat4 mvp;
    
    out vec3 FragPos;
    out vec3 Normal;
    out vec3 localPos; 

    void main() {
        gl_Position = mvp * vec4(aPos, 1.0);
        FragPos = aPos;    
        localPos = aPos;   
        Normal = aNormal;  
    }
)glsl";

const char* OBJTEXTUREfragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;

    in vec3 FragPos;
    in vec3 Normal;
    in vec3 localPos;

    uniform sampler2D ourTexture;
    uniform vec4 overlayColor; 
    uniform float mini; 

    void main() {
        float scale = 0.01; 
        vec3 blending = abs(normalize(Normal));
        blending /= (blending.x + blending.y + blending.z);

        vec4 xTex = texture(ourTexture, FragPos.yz * scale);
        vec4 yTex = texture(ourTexture, FragPos.xz * scale);
        vec4 zTex = texture(ourTexture, FragPos.xy * scale);
        vec4 texColor = xTex * blending.x + yTex * blending.y + zTex * blending.z;

        vec3 finalColor = texColor.rgb * overlayColor.rgb;
        
        if (mini > 0.5) {
            if (localPos.z > 90.0) {
                finalColor = vec3(0.1, 0.1, 0.1); 
            }
            if (abs(localPos.y - 1.5) < 0.3) {
                finalColor = vec3(0.0, 0.2, 0.8); 
            }
        }


        vec3 sunDir = normalize(vec3(0.5, 1.0, 0.3));
        float diff = max(dot(normalize(Normal), sunDir), 0.2);
        
        FragColor = vec4(finalColor * diff, 1.0); 
    }
)glsl";


const char* uiTexVertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoords;
    out vec2 TexCoords;
    uniform mat4 mvp;
    void main() {
        gl_Position = mvp * vec4(aPos, 0.0, 1.0);
        TexCoords = aTexCoords;
    }
)glsl";

const char* uiTexFragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    in vec2 TexCoords;
    uniform sampler2D uiTexture;
    void main() {
        FragColor = texture(uiTexture, TexCoords);
    }
)glsl";




unsigned int createShaderProgram() {
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, NULL);
    glCompileShader(vs);
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSource, NULL);
    glCompileShader(fs);
    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

unsigned int OBJTEXTUREcreateShaderProgram() {
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &OBJTEXTUREvertexShaderSource, NULL);
    glCompileShader(vs);
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &OBJTEXTUREfragmentShaderSource, NULL);
    glCompileShader(fs);
    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

unsigned int TEXTUREcreateShaderProgram() {
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &TEXTUREvertexShaderSource, NULL);
    glCompileShader(vs);
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &TEXTUREfragmentShaderSource, NULL);
    glCompileShader(fs);
    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}