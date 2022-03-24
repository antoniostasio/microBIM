#include "lib.h"
#include <algorithm>
// #include <ncurses.h>

// main.cpp
int main(int argc, char *argv[])
{	
	 
	constexpr int termWidth = 25;
	constexpr int termHeight = 20;
    
    // initscr();			/* Start curses mode 		  */
	// printw("Hello World !!!");	/* Print Hello World		  */
	// refresh();			/* Print it on to the real screen */
	// getch();			/* Wait for user input */
	// endwin();			/* End curses mode		  */
	// resizeterm(termHeight, termWidth);
    
    int width = 15;
    int heigth = 10;
    std::shared_ptr<CharBuffer> wallBuffer = std::make_shared<CharBuffer>();
    wallBuffer->data = new char[heigth * width];
    wallBuffer->size.height = heigth;
    wallBuffer->size.width = width;
    std::fill_n(wallBuffer->data, width*heigth, '@');

    int posx = 5;
    int posy = 5;
    Node wall(posx,posy);
    wall.SetImage(wallBuffer);
    
    
    width = 5;
    heigth = 4;
    std::shared_ptr<CharBuffer> windowBuffer = std::make_shared<CharBuffer>();
    windowBuffer->data = new char[heigth * width];
    windowBuffer->size.height = heigth;
    windowBuffer->size.width = width;
    std::fill_n(windowBuffer->data, width*heigth, '-');
    
    posx = 1;
    posy = 3;
    Node window(posx, posy);
    window.SetImage(windowBuffer);
    
    posx = 7;
    Node window2(posx, posy);
    window2.SetImage(windowBuffer);
    
    // wall.AddChild(window);
    // if(!window.CheckCollisionWith(window2))
        // wall.AddChild(window2);

    wall.AddChild(window2);
    if(!window2.CheckCollisionWith(window))
        wall.AddChild(window);
    
    
    char fill = ' ';
    if(argc>1) {
        fill = argv[1][0];
    }
    
    Writer PabloNeruda(termWidth, termHeight, fill);
    
    // print result
    PabloNeruda.AddRoot(&wall);
    PabloNeruda.RenderScene();
    PabloNeruda.WriteOnConsole();
    

	return 0;
}