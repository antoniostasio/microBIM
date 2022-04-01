#include "Node.h"

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

// TODO check that child does not goes out of parent's border
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
