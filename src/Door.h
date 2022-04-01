#pragma once
#include "node.h"
#include "Buildable.h"

class Door: public Buildable {
public:
    Door(vec2i v): Buildable(v) {Price(500); BuildTime(1);}
    Door(int x = 0, int y = 0): Buildable(x, y) {Price(500); BuildTime(1);}
}
;