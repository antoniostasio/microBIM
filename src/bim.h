#pragma once
#include "lib.h"
#include <vector>
#include <memory>

class Floor: public Node {
public:
    Floor(vec2i v): Node(v) {}
    Floor(int x = 0, int y = 0): Node(x,y) {}
    
	bool AddWindows(int numberOfWindows, int heightFromGround, bool append = true);
    bool AddWindow(const vec2i& atPosition);
	bool AddDoor(int position);
    
	void DrawIn(const CharBuffer* buffer, int atX, int atY) const;
private:
    std::vector<int> _windows;
	std::vector<int> _doors;
}
;

class Window: public Node {
public:
    Window(vec2i v): Node(v) {}
    Window(int x = 0, int y = 0): Node(x,y) {}
    
    void DrawIn(const CharBuffer* buffer, int atX, int atY) const override;
private:
    char _material;
}
;

class Door: public Node {
public:
    Door(vec2i v): Node(v) {}
    Door(int x = 0, int y = 0): Node(x, y) {}
    
    void DrawIn(const CharBuffer* buffer, int atX, int atY) const;
}
;

class House: public Node {
public:
    House(): Node() {
        constexpr int canvasWidth = 40;
        constexpr int canvasHeight = 20;
        _ctxture = std::make_shared<CharBuffer>();
        _ctxture->size.height = canvasHeight;
        _ctxture->size.width = canvasWidth;
        _ctxture->data = new char[canvasWidth*canvasHeight];
        std::fill_n(_ctxture->data, canvasWidth*canvasHeight, '@');
        _roofStartingPoint.x = 4;
        _roofStartingPoint.y = 10;
    }
    
    int Price();
    int ConstructionTime();
	bool AddNewFloor(); // add on top of existing floor
    bool AddDoor(int floor, int position);
    bool AddWindow(int floor, vec2i position);
    
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

bool House::AddNewFloor() {
    bool added = false;
    
    int floorHeight = 8;
    
    int houseHeight = _ctxture->size.height;
    if(houseHeight - _roofStartingPoint.y < floorHeight)
        return false;
    auto floor = std::make_unique<Floor>(_roofStartingPoint);  //(new Floor(_roofStartingPoint));
    auto floorBuffer = std::make_shared<CharBuffer>();
    floorBuffer->size.height = 8;
    floorBuffer->size.width =  32;
    floor->SetImage(floorBuffer);
    AddChild(std::move(floor));
    
    _roofStartingPoint.y -= floorHeight;
    
    return added;
}

bool House::AddDoor(int floorNumber, int position) {
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

bool House::AddWindow(int floorNumber, vec2i position) {
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

const std::shared_ptr<CharBuffer> House::Render() {
    for(const auto& child : _children) {
        child->DrawIn(_ctxture.get(), 0,  0);
    }
    return _ctxture;
}

bool Floor::AddDoor(int position) {
    
    int doorHeight = 4;
    vec2i doorPosition = {position,
                (_ctxture->size.height - doorHeight)};
    auto door = std::make_unique<Door>(doorPosition);  //(new Door(doorPosition));
    auto doorBuffer = std::make_shared<CharBuffer>();
    doorBuffer->size.height = 4;
    doorBuffer->size.width =  5;
    door->SetImage(doorBuffer);
    bool collided = false;
    for(const auto& child : _children) {
        collided |= child->CollidesWith(*door);
    }
    if(!collided){
        AddChild(std::move(door));
        _doors.emplace_back(_children.size());        
    }
    
    return !collided;
}

bool Floor::AddWindows(int numberOfWindows, int heightFromGround, bool append) {
    
    int availableWidth = _ctxture->size.width - 2; // 2 are the wall border chars
    int  windowWidth = 4;
    if(availableWidth < 2*windowWidth*numberOfWindows) // or maybe add untill full
        return false;
    int offset = availableWidth / (2*numberOfWindows);
    
    _windows.reserve(_windows.size() + numberOfWindows);
    vec2i windowPosition {offset, heightFromGround};
    
    bool collided = false;    
    for(int n=0; n<numberOfWindows; ++n) {
        std::unique_ptr<Node> window = std::make_unique<Window>(windowPosition); //(new Window(windowPosition));
        auto windowBuffer = std::make_shared<CharBuffer>();
        windowBuffer->size.height = 4;
        windowBuffer->size.width =  5;
        window->SetImage(windowBuffer);
        for(const auto& child : _children) {
            collided |= child->CollidesWith(*window);
        }
        if(!collided){
            AddChild(std::move(window));
            _windows.emplace_back(_children.size());
        }
        windowPosition.x += offset;
    }
    
    return !collided;
}

bool Floor::AddWindow(const vec2i& atPosition) {
        
    int availableWidth = _ctxture->size.width - 2;
    int windowWidth = 4;
    if(availableWidth < windowWidth)
        return false;
        
    bool collided = false;
    std::unique_ptr<Node> window = std::make_unique<Window>(atPosition);  //(new Window(atPosition));
    auto windowBuffer = std::make_shared<CharBuffer>();
    windowBuffer->size.height = 4;
    windowBuffer->size.width =  5;
    window->SetImage(windowBuffer);
    for(const auto& child : _children) {
        collided |= child->CollidesWith(*window);
    }
    if(!collided){
        AddChild(std::move(window));
        _windows.emplace_back(_children.size());
    }
    
    return !collided;
}

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

