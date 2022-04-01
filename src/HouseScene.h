#pragma once
#include "node.h"
#include "Buildable.h"

class HouseScene: public Buildable {
public:
    HouseScene(int width = 40, int height = 20): Buildable() {
        _ctxture = std::make_shared<CharBuffer>();
        _ctxture->size.height = height;
        _ctxture->size.width = width;
        _ctxture->data = new char[width*height];
        std::fill_n(_ctxture->data, width*height, '\'');
        _roofStartingPoint.x = 4;
        _roofStartingPoint.y = height;
    }
    
	bool AddNewFloor(); // add on top of existing floor
    bool AddDoor(int floor, int position);
    bool AddWindow(int floor, vec2i position);
	bool AddWindows(int floorNumber, int numberOfWindows, int heightFromGround, bool append = true);
    
    const std::shared_ptr<CharBuffer> Render();
private:
    vec2i _roofStartingPoint;
	int _floorsCount;
}
;