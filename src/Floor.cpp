#include "Floor.h"
#include "Window.h"
#include "Door.h"

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

// TODO fix: height is relative to roof instead of ground
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
    vec2i windowPosition {offset,
                          heightFromGround};
    
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
