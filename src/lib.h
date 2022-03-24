#include <iostream>
#include <list>
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
	// Node(): _ctexture(nullptr), _parent(nullptr) {}
	Node(int x = 0, int y = 0): _ctxture(nullptr), _parent(nullptr) {
		X(x); Y(y);
	}
	Node(const Node&) = delete;
	// Node(const Node&& nd) noexcept : 
		// _position(std::move(nd._position)),
		// _children(std::move(nd._children)),
		// _parent(std::move(nd._parent)),
		// _ctxture(std::move(nd._ctxture))
	// {};
	Node& operator=(const Node&) = delete;
	
	void AddChild(Node& elem);
	void AddParent(Node* parent);
	void SetImage(std::shared_ptr<CharBuffer>& txture) {_ctxture = txture;}
	void DrawIn(const CharBuffer* buffer, int atX, int atY) const;
	bool CheckCollisionWith(const Node& elem);
	
	void X(int val) {_position.x = val;}
	void Y(int val) {_position.y = val;}

	int X() const {return _position.x;}
	int Y() const {return _position.y;}
	
	Size getSize() const;
private:
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

void Node::AddParent(Node* parent) {
	_parent = parent;
}

// TODO move collision check to client classes (wall, windows, ...)
void Node::AddChild(Node& elem) {
	std::unique_ptr<Node> childPtr(&elem);
	_children.push_back(std::move(childPtr));
	elem._parent = this;
}

void Node::DrawIn(const CharBuffer* buffer, int atX, int atY) const {
	int srcHeight = buffer->size.height;
	int srcWidth = buffer->size.width;
	atX += _position.x;
	atY += _position.y;
	for(int y = 0; y < _ctxture->size.height; ++y) {
        for(int x = 0; x < _ctxture->size.width; ++x) {
			// if((atX+x==7) || (atY+y==8)) {
				// buffer->data[atX+x
						//   + (atY+y)*srcWidth] = '/';
			// } else {
            buffer->data[atX+x
					   +(atY+y)*srcWidth]
				= _ctxture->data[x+
						  		 y*_ctxture->size.width];
			// }
        }
    }
    for(const auto& child : _children) {
		child->DrawIn(buffer, atX, atY);
	}
}

bool Node::CheckCollisionWith(const Node& other) {
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
	void AddRoot(Node* root);
	void RenderScene(int startingX, int startingY);
	void WriteOnConsole();
private:
	CharBuffer *_paperSheet;
	Node* _drafts;
}
;

// Writer.cpp
void Writer::AddRoot(Node* root) {
	_drafts = root;
}

void Writer::RenderScene(int startingX=0, int startingY=0) {
		_drafts->DrawIn(_paperSheet, startingX, startingY);
	}

void Writer::WriteOnConsole() {
	for(int i=0; i < _paperSheet->size.height; i++) {
        for(int j=0; j<_paperSheet->size.width; j++) {
            cout << _paperSheet->data[j+i*_paperSheet->size.width];
        }
        cout << '\n';
    }
}