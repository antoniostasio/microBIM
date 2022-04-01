#include "Writer.h"

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