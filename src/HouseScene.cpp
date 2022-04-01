#include "HouseScene.h"
#include "Floor.h"
#include "Door.h"
#include "Window.h"

bool HouseScene::AddNewFloor() {
    bool added = false;
    
    constexpr char floorRaster[] = {"+------------------------------+"
                                    "|                              |"
                                    "|                              |"
                                    "|                              |"
                                    "|                              |"
                                    "|                              |"
                                    "|                              |"
                                    "|                              |"};
    constexpr int wallWidth = 32;
    constexpr int wallHeight = 8;
    int houseHeight = _ctxture->size.height;
    if(_roofStartingPoint.y < wallHeight)
        return false;
    auto floor = std::make_unique<Floor>(_roofStartingPoint -= vec2i{0,wallHeight});
    std::shared_ptr<CharBuffer> floorBuffer;
    // use existing buffer if there's already one
    if(_children.size() > 0) {
        auto firstFloor = dynamic_cast<Floor*>(_children.front().get());
        if(firstFloor != nullptr)
            floorBuffer = firstFloor->NodeBuffer();
    }
    else {
        floorBuffer = std::make_shared<CharBuffer>();
        floorBuffer->size.height = wallHeight;
        floorBuffer->size.width =  wallWidth;
        floorBuffer->data = new char[wallWidth*wallHeight];
        memcpy(floorBuffer->data, floorRaster, wallWidth*wallHeight );
    }
    floor->SetImage(floorBuffer);
    AddChild(std::move(floor));
    
    _roofStartingPoint += vec2i{0,1};
    ++_floorsCount;
    
    return added;
}

bool HouseScene::AddDoor(int floorNumber, int position) {
    bool added = false;
    
    int floor = 0;
    for(auto& floorPtr : _children) {
        if(floor == floorNumber){
            Floor* currentFloor = dynamic_cast<Floor*>(floorPtr.get());
            if(currentFloor != nullptr) {
                currentFloor->AddDoor(position);
                added = true;
            }
            break;
        }
        ++floor;
    }
    
    return added;
}

bool HouseScene::AddWindow(int floorNumber, vec2i position) {
    bool added = false;
    
    int floor = 0;
    for(auto& floorPtr : _children) {
        if(floor == floorNumber){
            Floor* currentFloor = dynamic_cast<Floor*>(floorPtr.get());
            if(currentFloor != nullptr) {
                currentFloor->AddWindow(position);
                added = true;
            }
            break;
        }
        ++floor;
    }
    
    return added;
}

bool HouseScene::AddWindows(int floorNumber, int numberOfWindows, int heightFromGround, bool append) {
    bool added = false;
    
    int floor = 0;
    for(auto& floorPtr : _children) {
        if(floor == floorNumber){
            Floor* currentFloor = dynamic_cast<Floor*>(floorPtr.get());
            if(currentFloor != nullptr) {
                currentFloor->AddWindows(numberOfWindows, heightFromGround);
                added = true;
            }
            break;
        }
        ++floor;
    }
    
    return added;
}


const std::shared_ptr<CharBuffer> HouseScene::Render() {
    for(const auto& child : _children) {
        child->DrawIn(_ctxture.get(), 0,  0);
    }
    return _ctxture;
}


// bool Floor::AddWindows(int numberOfWindows, int heightFromGround, bool append) {
//     constexpr char windowRaster[] = {",___,"
//                                      "| | |"
//                                      "| | |"
//                                      "-----"};
//     constexpr int windowWidth = 5;
//     constexpr int windowHeight = 4;
    
//     // check for available space
//     int availableWidth = _ctxture->size.width - 2; // 2 are the wall border chars
//     if(availableWidth < 2*windowWidth*numberOfWindows) // or maybe add untill full
//         return false;
//     int offset = availableWidth / (2*numberOfWindows+1);
    
//     _windows.reserve(_windows.size() + numberOfWindows);
//     vec2i windowPosition {offset, heightFromGround};
    
//     bool collided = false;
    
//     // use existing buffer if there's already one or create new
//     std::shared_ptr<CharBuffer> windowBuffer(nullptr);
//     for(const auto& child: _children) {
//         auto existingWindow = dynamic_cast<Window*>(child.get());
//         if(existingWindow != nullptr) {
//             windowBuffer = existingWindow->NodeBuffer();
//             break;
//         }
//     }
//     if(!windowBuffer) {
//         windowBuffer = std::make_shared<CharBuffer>();
//         windowBuffer->size.height = windowHeight;
//         windowBuffer->size.width =  windowWidth;
//         windowBuffer->data = new char[windowWidth*windowHeight];
//         memcpy(windowBuffer->data, windowRaster, windowWidth*windowHeight);
//     }
    
//     for(int n=0; n<numberOfWindows; ++n) {
//         std::unique_ptr<Node> window = std::make_unique<Window>(windowPosition);
//         window->SetImage(windowBuffer);
//         for(const auto& child : _children) {
//             collided |= child->CollidesWith(*window);
//         }
//         if(!collided){
//             AddChild(std::move(window));
//             _windows.emplace_back(_children.size()-1);
//         }
//         windowPosition.x += (windowWidth + offset);
//         collided = false;
//     }
    
//     return !collided;
// }