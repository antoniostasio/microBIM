#pragma once
#include "Node.h"

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