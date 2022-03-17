#include <iostream>
#include <list>
#include <memory>

using std::cout;
using std::list;

struct vec2i {
	int32_t x;
	int32_t y;
}
;

struct CharBuffer {
	~CharBuffer() {delete data;}
	uint32_t width;
	uint32_t height;
	char* data;
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
	
	bool AddChild(Node& elem);
	void AddParent(Node* parent);
	void SetImage(const std::shared_ptr<CharBuffer>& txture) {_ctxture = txture;}
	void DrawIn(const CharBuffer* buffer, int atX, int atY) const;
	
	void X(int val) {_position.x = val;}
	void Y(int val) {_position.y = val;}

	int X() const {return _position.x;}
	int Y() const {return _position.y;}	
private:
	vec2i _position;
	list<std::unique_ptr<Node>> _children;
	// std::weak_ptr<Node> _parent;
	Node* _parent;
	std::shared_ptr<CharBuffer> _ctxture;
	
	bool _doesCollide(const Node& elem);
}
;

// Node.cpp
bool Node::_doesCollide(const Node& elem) {
	return(false);
}

void Node::AddParent(Node* parent) {
	_parent = parent;
}

bool Node::AddChild(Node& elem) {
	if(!_doesCollide(elem)) {
		std::unique_ptr<Node> childPtr(&elem);
		_children.push_back(std::move(childPtr));
		elem._parent = this;
		// elem.setParent(*this);
		return true;
	}
	return false;
}

void Node::DrawIn(const CharBuffer* buffer, int atX, int atY) const {
	int srcHeight = buffer->height;
	int srcWidth = buffer->width;
	atX += _position.x;
	atY += _position.y;
	for(int y = 0; y < _ctxture->height; ++y) {
        for(int x = 0; x < _ctxture->width; ++x) {
            buffer->data[atX+x
					   +(atY+y)*srcWidth]
				= _ctxture->data[_position.x+
						  		 _position.y*_ctxture->width];
        }
    }
    for(auto& child : _children) {
		child->DrawIn(buffer, atX, atY);
	}
}

// Writer.h
class Writer {
public:
	Writer(): _paperSheet{} {}
	Writer(int rows, int columns, char fill = ' ') {
		_paperSheet = new CharBuffer();
		_paperSheet->data = new char[rows*columns];
		_paperSheet->width = columns;
		_paperSheet->height = rows;
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
	for(int i=0; i < _paperSheet->height; i++) {
        for(int j=0; j<_paperSheet->width; j++) {
            cout << _paperSheet->data[j+i*_paperSheet->width];
        }
        cout << '\n';
    }
}