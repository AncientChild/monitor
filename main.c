/*
    Copyright <2019> <Leon Fretter>

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define monitorCount 4

typedef struct Vector2 {
    int x;
    int y;
} Vector2;

typedef struct monitor {
    const char* label;
    const char* interface;
    Vector2 resolution;
} monitor;

const monitor monitors[monitorCount] = {
    {
        .label = "HANNS-G",
        .resolution = {.x = 1280, .y=1024},
        .interface = "DVI-D-0"
    },
    {
        .label = "HUION",
        .resolution = {.x = 1440, .y=900},
        .interface = "DP-3"
    },
    {
        .label = "acer",
        .resolution = {.x = 1920, .y=1080},
        .interface = "HDMI-0"
    },
    {
        .label = "LG",
        .resolution = {.x =1280, .y=1024},
        .interface = "DP-5"
    }
};

const int verticalPositions[monitorCount] = {-300,500,0,100};

Vector2 calcPosition(int index, char* args) {
    int posX = 0;
    int posY = verticalPositions[index];

    for(int i=0; i<index; i++) {
        if(args[i] == '1') {
            posX += monitors[i].resolution.x;
        }
    }

    Vector2 position = {
        .x = posX,
        .y = posY
    };

    return position;
}

int main(int argc, char** argv) {
    bool onlyZero = true;

    char monitorArgs[monitorCount];

    if(argc != monitorCount+1) {
        printf("Expected exactly %d arguments\n", monitorCount+1);
        return 1;
    }

    for(int i=1; i<argc; i++) {
        if(strcmp(argv[i], "0") == 0) {
            monitorArgs[i-1] = '0';
        }
        else if(strcmp(argv[i], "1") == 0){
            onlyZero = false;
            monitorArgs[i-1] = '1';
        }
        else {
            printf("arguments provided must be of value 0 or 1\n");
            return 1;
        }
    }
    if(onlyZero == true) {
        printf("At least one monitor needs to be activated!\n");
        return 1;
    }

    char command[1024] = "xrandr ";
    char initCommand[1024] = "xrandr ";

    for(int i=0; i<monitorCount; i++) {
        Vector2 position = calcPosition(i, monitorArgs);
        const char* interface = monitors[i].interface;

        sprintf(command + strlen(command), "--output %s ", interface);
        sprintf(initCommand + strlen(initCommand), "--output %s ", interface);

        if(monitorArgs[i] == '0') {
            strcat(command, "--off ");    
            strcat(initCommand, "--off ");
        } else {
            sprintf(command + strlen(command), "--pos %dx%d ", position.x, position.y);
            strcat(initCommand, "--auto ");
        }
    }

    printf("initCommand: %s\n\n", initCommand);
    printf("command: %s\n\n", command);
    
    system(initCommand);
    system(command);

    return 0;
}