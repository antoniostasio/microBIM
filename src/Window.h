#pragma once
#include "node.h"
#include "Buildable.h"

class Window: public Buildable {
public:
    Window(vec2i v): Buildable(v) {Price(300); BuildTime(2);}
    Window(int x = 0, int y = 0): Buildable(x,y) {Price(300); BuildTime(2);}
    
private:
    char _material;
}
;