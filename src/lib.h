#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <chrono>

using std::cout;
using std::list;

typedef struct {
	int32_t x;
	int32_t y;
} vec2i
;

typedef struct {
	uint32_t width;
	uint32_t height;
} Size
;

struct CharBuffer {
	~CharBuffer() {delete data;}
	Size size;
	char* data;
	uint8_t z;
}
;

vec2i operator-(const vec2i& left, const vec2i& right) {
	return vec2i{left.x-right.x, left.y-right.y};
}

vec2i operator+(const vec2i& left, const vec2i& right) {
	return vec2i{left.x+right.x, left.y+right.y};
}

vec2i& operator-=(vec2i& left, const vec2i& right) {
	left.x -= right.x;
	left.y -= right.y;
	return left;
}

vec2i& operator+=(vec2i& left, const vec2i& right) {
	left.x += right.x;
	left.y += right.y;
	return left;
}

// Drawable.h
class Drawable {
public:
	virtual void DrawIn(const CharBuffer* buffer, int atX, int atY) const = 0;
	void SetImage(const std::shared_ptr<CharBuffer>& txture) {_ctxture = txture;}
protected:
	std::shared_ptr<CharBuffer> _ctxture;
}
;

// Node.h
class Node {
public:
	Node(int x = 0, int y = 0): _ctxture(nullptr), _parent(nullptr), _position({x, y}) {}
	Node(const vec2i& v): _ctxture(nullptr), _parent(nullptr), _position(v) {}
	Node(const Node&) = delete;
	// Node(const Node&& nd) noexcept : 
		// _position(std::move(nd._position)),
		// _children(std::move(nd._children)),
		// _parent(std::move(nd._parent)),
		// _ctxture(std::move(nd._ctxture))
	// {};
	Node& operator=(const Node&) = delete;
	
	void AddChild(std::unique_ptr<Node> elem); // TODO move protected
	void AddParent(Node* parent);			   // TODO move protected
	void SetImage(std::shared_ptr<CharBuffer>& txture) {_ctxture = txture;}
	virtual void DrawIn(const CharBuffer* buffer, int atX, int atY) const;
	bool CollidesWith(const Node& elem);
	
	const char* const getPixel(int atX, int atY) const;
	virtual void DrawByPixel() const;
	
	void X(int val) {_position.x = val;}
	void Y(int val) {_position.y = val;}

	int X() const {return _position.x;}
	int Y() const {return _position.y;}
	
	Size getSize() const;
    std::shared_ptr<CharBuffer> NodeBuffer();
protected:
	vec2i _position;
	list<std::unique_ptr<Node>> _children;
	// std::weak_ptr<Node> _parent;
	Node* _parent;
	std::shared_ptr<CharBuffer> _ctxture;
}
;

// Node.cpp
Size Node::getSize() const {
	return _ctxture->size;
}

std::shared_ptr<CharBuffer> Node::NodeBuffer() {
	return _ctxture;
}


void Node::AddParent(Node* parent) {
	_parent = parent;
}

// TODO check that child does not exceed parent size
void Node::AddChild(std::unique_ptr<Node> elem) {
	elem->_parent = this;
	_children.push_back(std::move(elem));
}

// TODO use z in node buffer by drawing from leaves to root skipping already drawn points
// TODO skip char = 0
void Node::DrawIn(const CharBuffer* buffer, int atX, int atY) const {
	int srcHeight = buffer->size.height;
	int srcWidth = buffer->size.width;
	atX += _position.x;
	atY += _position.y;
	for(int y = 0; y < _ctxture->size.height; ++y) {
        for(int x = 0; x < _ctxture->size.width; ++x) {
            buffer->data[atX+x
					   +(atY+y)*srcWidth]
				= _ctxture->data[x+
						  		 y*_ctxture->size.width];
        }
    }
    for(const auto& child : _children) {
		child->DrawIn(buffer, atX, atY);
	}
}

bool Node::CollidesWith(const Node& other) {
	const Size otherSize = other.getSize();
	
	uint32_t otherStartX = other.X();
	uint32_t otherStartY = other.Y();
	uint32_t otherEndX = otherStartX + otherSize.width;
	uint32_t otherEndY = otherStartY + otherSize.height;
	
	uint32_t overlappingXs = otherEndX - _position.x;
	//overlappingXes = _ctxture->size.width - other.Y();
	bool xOverlap = overlappingXs < _ctxture->size.width + otherSize.width;
	xOverlap = xOverlap && (overlappingXs > 0);
	
	uint32_t overlappingYs = otherEndY - _position.y;
	// if(yRange < 0)
		// yRange = _ctxture->size.height - other.Y();
	bool yOverlap = overlappingYs < _ctxture->size.height + otherSize.height;
	yOverlap = yOverlap && (overlappingYs > 0);
	
	bool collision = xOverlap && yOverlap;
	
	return(collision);
}

const char* const Node::getPixel(int atX, int atY) const {
	int relativeX = atX-_position.x;
	int relativeY = atY-_position.y;
	if((relativeX >= 0) && (relativeX < _ctxture->size.width)
	&& (relativeY >= 0) && (relativeY < _ctxture->size.height)) {
		// check if childern need to draw
		for(const auto& child : _children) {
			const char* const c = child->getPixel(relativeX, relativeY);
			if(c != nullptr)
				return c;
		}
		
		return &_ctxture->data[relativeX + relativeY*_ctxture->size.width];
	}
	return nullptr;
}



void Node::DrawByPixel() const {
	
}


// Writer.h
class Writer {
public:
	Writer(): _paperSheet{} {}
	Writer(int columns, int rows, char fill = ' ') {
		_paperSheet = new CharBuffer();
		_paperSheet->data = new char[rows*columns];
		_paperSheet->size.width = columns;
		_paperSheet->size.height = rows;
		std::fill_n(_paperSheet->data, rows*columns, fill);
	}
	void AddRoot(std::unique_ptr<Node> root);
	void RenderScene(int startingX=0, int startingY=0);
	void RenderSceneByPixel(int startingX=0, int startingY=0);
	void WriteOnConsole();
private:
	CharBuffer *_paperSheet;
	std::unique_ptr<Node> _drafts;
}
;

// Writer.cpp
void Writer::AddRoot(std::unique_ptr<Node> root) {
	_drafts = std::move(root);
}

void Writer::RenderScene(int startingX, int startingY) {
		_drafts->DrawIn(_paperSheet, startingX, startingY);
	}

void Writer::RenderSceneByPixel(int startingX, int startingY) {
	uint32_t sceneWidth = _paperSheet->size.width;
	uint32_t sceneHeight = _paperSheet->size.height;
	for(int y=startingY; y < sceneHeight; ++y) {
		for(int x=startingX; x < sceneWidth; x++) {
			const char* const c = _drafts->getPixel(x, y);
			if(c != nullptr)
				_paperSheet->data[x + y*sceneWidth] = *c;
			// cout << _paperSheet->data[x + y*sceneWidth];
		}
		// cout << std::endl;
	}
}

void Writer::WriteOnConsole() {
	for(int i=0; i < _paperSheet->size.height; i++) {
        for(int j=0; j<_paperSheet->size.width; j++) {
            cout << _paperSheet->data[j+i*_paperSheet->size.width];
        }
        cout << '\n';
    }
}