#pragma once
#include "node.h"
#include "Buildable.h"
#include <vector>
#include <memory>

class Floor: public Buildable {
public:
    Floor(vec2i v): Buildable(v) {Price(5000); BuildTime(30);}
    Floor(int x = 0, int y = 0): Buildable(x,y) {Price(5000); BuildTime(30);}
    
    bool AddWindows(int numberOfWindows, int heightFromGround, bool append = true);
    bool AddWindow(const vec2i& atPosition);
    bool AddDoor(int position);
private:
    std::vector<int> _windows;
    std::vector<int> _doors;
}
;