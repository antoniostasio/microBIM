#pragma once
#include "Node.h"

class Buildable: public Node {
public:
    Buildable(vec2i v): Node(v), _price(0), _buildTime(0) {}
    Buildable(int x=0, int y=0): Node(x,y), _price(0), _buildTime(0) {}
    
    void Price(int p) {_price = p;}
    void BuildTime(int t) {_buildTime = t;}
    int Price();
    int BuildTime();
private:
    int _price;
    int _buildTime;  // days
}
;