#pragma once
#include "HouseScene.h"
#include <iostream>

using std::cout;
using std::cin;

#define NEWFLOOR 1
#define NEWDOOR 2
#define NEWWINDOW 3

enum class Selection: int {
    NewFloor,
    NewDoor,
    NewWindow
};

inline void ClearTerminal() { cout << "\x1B[2J\x1B[H"; }

inline void PromptUser() {
    cout << "1: New floor\t" << "2: Add door\t" << "3: Add window\n";
}

void DrawOnConsole(std::shared_ptr<CharBuffer> image) {
    for(int y=0; y < image->size.height;  ++y) {
        for(int x=0; x < image->size.width; ++x) {
            cout << image->data[x + y*image->size.width];
        }
        cout << '\n';
    }
}

void start() {
    ClearTerminal();
    
    std::unique_ptr<HouseScene> house(new HouseScene(40, 18));
    
    int width = 0;
    int height = 0;
    // cout << "First floor creation\n"
        //  << "Width: ";
    // cin >> width;
    // cout << "Height: ";
    // cin >> height; 
    house->AddNewFloor();
    
    house->Render();
    std::shared_ptr<CharBuffer> image = house->Render();
    DrawOnConsole(image);
    
    PromptUser();
    int selection;
    
    bool isRunning = true;
    while(isRunning) {
        cin >> selection;
        int selectedFloor = -1;
        switch(selection) {
            case NEWFLOOR: {
                // cout << "Width: ";
                // cin >> width;
                // cout << "Height: ";
                // cin >> height;
                house->AddNewFloor();
                break;
            }
            case NEWDOOR: {
                int x;
                cout << "Which floor? ";
                cin >> selectedFloor;
                // cout << "Width: ";
                // cin >> width;
                // cout << "Height: ";
                // cin >> height;
                cout << "Position: ";
                cin >> x;
                house->AddDoor(selectedFloor, x);
                break;
            }
            case NEWWINDOW: {
                vec2i position;
                cout << "Which floor? ";
                cin >> selectedFloor;
                // cout << "Width: ";
                // cin >> width;
                // cout << "Height: ";
                // cin >> height;
                int insertionType = 0;
                cout << "1: manual positioning\t2: automatic\n";
                cin >> insertionType;
                if(insertionType == 1) {
                    cout << "Enter coordinates\n" << "x y: ";
                    cin >> position.x;
                    cin >>position.y;
                    house->AddWindow(selectedFloor, position);
                }
                if(insertionType == 2) {
                    int elevation = 0;
                    int windowsNumber = 0;
                    cout << "Number of windows: ";
                    cin >> windowsNumber;
                    cout << "Height from ground: ";
                    cin >> elevation;
                    house->AddWindows(selectedFloor, windowsNumber, elevation);
                }
            }
                break;
            default:
                isRunning = false;
        }
        image = house->Render();
        
        ClearTerminal();
        
        DrawOnConsole(image);
        
        PromptUser();
    }
}

class HouseBuilder {
public:
    void NewHouse();
}
;