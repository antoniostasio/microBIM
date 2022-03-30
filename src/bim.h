#pragma once
#include "lib.h"
#include <vector>
#include <memory>

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

class Window: public Buildable {
public:
    Window(vec2i v): Buildable(v) {Price(300); BuildTime(2);}
    Window(int x = 0, int y = 0): Buildable(x,y) {Price(300); BuildTime(2);}
    
private:
    char _material;
}
;

class Door: public Buildable {
public:
    Door(vec2i v): Buildable(v) {Price(500); BuildTime(1);}
    Door(int x = 0, int y = 0): Buildable(x, y) {Price(500); BuildTime(1);}
}
;

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

class HouseBuilder {
public:
    void NewHouse();
}
;


// Bim.cpp

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



bool Floor::AddDoor(int position) {
    
    constexpr char doorRaster[] = {"+---+"
                                   "|   |"
                                   "|o  |"
                                   "|___|"};
    constexpr int doorWidth = 5;
    constexpr int doorHeight = 4;
    vec2i doorPosition = {position,
                (_ctxture->size.height - doorHeight)};
    auto door = std::make_unique<Door>(doorPosition);
    std::shared_ptr<CharBuffer> doorBuffer(nullptr);
    // use existing buffer if there's already one
    for(const auto& child: _children) {
        auto existingDoor = dynamic_cast<Door*>(child.get());
        if(existingDoor != nullptr) {
            doorBuffer = existingDoor->NodeBuffer();
            break;
        }
    }
    if(!doorBuffer) {
        doorBuffer = std::make_shared<CharBuffer>();
        doorBuffer->size.height = doorHeight;
        doorBuffer->size.width =  doorWidth;
        doorBuffer->data = new char[doorWidth*doorHeight];
        memcpy(doorBuffer->data, doorRaster, doorWidth*doorHeight );
    }
    door->SetImage(doorBuffer);
    bool collided = false;
    for(const auto& child : _children) {
        collided |= child->CollidesWith(*door);
    }
    if(!collided){
        AddChild(std::move(door));
        _doors.emplace_back(_children.size()-1);
    }
    
    return !collided;
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

bool Floor::AddWindows(int numberOfWindows, int heightFromGround, bool append) {
    constexpr char windowRaster[] = {",___,"
                                     "| | |"
                                     "| | |"
                                     "-----"};
    constexpr int windowWidth = 5;
    constexpr int windowHeight = 4;
    
    // check for available space
    int availableWidth = _ctxture->size.width - 2; // 2 are the wall border chars
    if(availableWidth < 2*windowWidth*numberOfWindows) // or maybe add untill full
        return false;
    int offset = availableWidth / (2*numberOfWindows+1);
    
    _windows.reserve(_windows.size() + numberOfWindows);
    vec2i windowPosition {offset, heightFromGround};
    
    bool collided = false;
    
    // use existing buffer if there's already one or create new
    std::shared_ptr<CharBuffer> windowBuffer(nullptr);
    for(const auto& child: _children) {
        auto existingWindow = dynamic_cast<Window*>(child.get());
        if(existingWindow != nullptr) {
            windowBuffer = existingWindow->NodeBuffer();
            break;
        }
    }
    if(!windowBuffer) {
        windowBuffer = std::make_shared<CharBuffer>();
        windowBuffer->size.height = windowHeight;
        windowBuffer->size.width =  windowWidth;
        windowBuffer->data = new char[windowWidth*windowHeight];
        memcpy(windowBuffer->data, windowRaster, windowWidth*windowHeight);
    }
    
    for(int n=0; n<numberOfWindows; ++n) {
        std::unique_ptr<Node> window = std::make_unique<Window>(windowPosition);
        window->SetImage(windowBuffer);
        for(const auto& child : _children) {
            collided |= child->CollidesWith(*window);
        }
        if(!collided){
            AddChild(std::move(window));
            _windows.emplace_back(_children.size()-1);
        }
        windowPosition.x += (windowWidth + offset);
        collided = false;
    }
    
    return !collided;
}

bool Floor::AddWindow(const vec2i& atPosition) {
    bool collided = false;
    
    constexpr char windowRaster[] = {",___,"
                                     "| | |"
                                     "| | |"
                                     "-----"};
    constexpr int windowWidth = 5;
    constexpr int windowHeight = 4;
    
    // use existing buffer if there's already one or create new
    std::shared_ptr<CharBuffer> windowBuffer(nullptr);
    for(const auto& child: _children) {
        auto existingWindow = dynamic_cast<Window*>(child.get());
        if(existingWindow != nullptr) {
            windowBuffer = existingWindow->NodeBuffer();
            break;
        }
    }
    if(!windowBuffer) {
        windowBuffer = std::make_shared<CharBuffer>();
        windowBuffer->size.height = windowHeight;
        windowBuffer->size.width =  windowWidth;
        windowBuffer->data = new char[windowWidth*windowHeight];
        memcpy(windowBuffer->data, windowRaster, windowWidth*windowHeight);
    }
    
    std::unique_ptr<Node> window = std::make_unique<Window>(atPosition);
    window->SetImage(windowBuffer);
    for(const auto& child : _children) {
        collided |= child->CollidesWith(*window);
    }
    if(!collided){
        AddChild(std::move(window));
        _windows.emplace_back(_children.size()-1);
    }
    
    return !collided;
}

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


/*
void Floor::DrawIn(const CharBuffer* buffer, int atX, int atY) const {
    constexpr char wall[] = {"+------------------------------+"
                             "|                              |"
                             "|                              |"
                             "|                              |"
                             "|                              |"
                             "|                              |"
                             "|                              |"
                             "|______________________________|"};
    constexpr int wallWidth = 32;
    constexpr int wallHeight = 8;
    int srcHeight = buffer->size.height;
	int srcWidth = buffer->size.width;
	atX += _position.x;
	atY += _position.y;
	for(int y = 0; y < wallHeight; ++y) {
        for(int x = 0; x < wallWidth; ++x) {
            buffer->data[atX+x
					   +(atY+y)*srcWidth]
				= wall[x+y*wallWidth];
        }
    }
    for(const auto& child : _children) {
		child->DrawIn(buffer, atX, atY);
	}
}

void Door::DrawIn(const CharBuffer* buffer, int atX, int atY) const {
    constexpr char door[] = {"+---+"
                             "|   |"
                             "|o  |"
                             "|___|"};
    constexpr int doorWidth = 5;
    constexpr int doorHeight = 4;
    int srcHeight = buffer->size.height;
	int srcWidth = buffer->size.width;
	atX += _position.x;
	atY += _position.y;
	for(int y = 0; y < doorHeight; ++y) {
        for(int x = 0; x < doorWidth; ++x) {
            buffer->data[atX+x
					   +(atY+y)*srcWidth]
				= door[x+y*doorWidth];
        }
    }
    for(const auto& child : _children) {
		child->DrawIn(buffer, atX, atY);
	}
}

void Window::DrawIn(const CharBuffer* buffer, int atX, int atY) const {
    constexpr char window[] = {",___,"
                               "| | |"
                               "| | |"
                               "-----"};
    constexpr int windowWidth = 5;
    constexpr int windowHeight = 4;
    int srcHeight = buffer->size.height;
	int srcWidth = buffer->size.width;
	atX += _position.x;
	atY += _position.y;
	for(int y = 0; y < windowHeight; ++y) {
        for(int x = 0; x < windowWidth; ++x) {
            buffer->data[atX+x
					   +(atY+y)*srcWidth]
				= window[x+y*windowWidth];
        }
    }
    for(const auto& child : _children) {
		child->DrawIn(buffer, atX, atY);
	}
}
*/
