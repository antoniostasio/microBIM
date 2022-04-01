#pragma once
#include <iostream>
#include <list>
#include <chrono>
#include <memory>

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

vec2i operator-(const vec2i& left, const vec2i& right);
vec2i operator+(const vec2i& left, const vec2i& right);
vec2i& operator-=(vec2i& left, const vec2i& right);
vec2i& operator+=(vec2i& left, const vec2i& right);

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
	Node& operator=(const Node&) = delete;
	
	void AddChild(std::unique_ptr<Node> elem); // TODO move protected?
	void AddParent(Node* parent);			   // TODO move protected?
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
};
