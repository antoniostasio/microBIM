#include "Buildable.h"

int Buildable::Price() {
    int totalPrice = _price;
    
    for(auto& child: _children) {
        Buildable* buildable = dynamic_cast<Buildable*>(child.get());
        if(buildable){
            totalPrice += buildable->Price();
        }
    }
    
    return totalPrice;
}

int Buildable::BuildTime() {
    int totalTime = _buildTime;
    
    for(auto& child: _children) {
        Buildable* buildable = dynamic_cast<Buildable*>(child.get());
        if(buildable){
            totalTime += buildable->BuildTime();
        }
    }
    
    return totalTime;
}