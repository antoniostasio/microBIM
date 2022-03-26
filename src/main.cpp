#include "lib.h"
#include <algorithm>
#include <chrono>
#include <vector>

// #include <ncurses.h>

template<typename TimeT = std::chrono::milliseconds>
struct measure
{
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F func, Args&&... args)
    {
        auto start = std::chrono::high_resolution_clock::now();
        func(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast< TimeT>
                (std::chrono::high_resolution_clock::now() - start);
        return duration.count();
    }
};


// main.cpp
int main(int argc, char *argv[])
{	
	 
	constexpr int termWidth = 20;
	constexpr int termHeight = 15;
    
    int width = 15;
    int heigth = 10;
    std::shared_ptr<CharBuffer> wallBuffer = std::make_shared<CharBuffer>();
    wallBuffer->data = new char[heigth * width];
    wallBuffer->size.height = heigth;
    wallBuffer->size.width = width;
    std::fill_n(wallBuffer->data, width*heigth, '@');

    int posx = 2;
    int posy = 0;
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
    
    wall.AddChild(window);
    if(!window.CheckCollisionWith(window2))
        wall.AddChild(window2);

    wall.AddChild(window2);
    if(!window2.CheckCollisionWith(window))
        wall.AddChild(window);
    
    
    char fill = 'x';
    if(argc>1) {
        fill = argv[1][0];
    }
    
    
    // Writer PabloNeruda(termWidth, termHeight, fill);
    // 
    // // print result
    // PabloNeruda.AddRoot(&wall);
    // PabloNeruda.RenderSceneByPixel();
    // PabloNeruda.WriteOnConsole();
    
    // performance test
    using namespace std::chrono;
    
    int averageTime = 0;
    int iterations = 10;
    for (int i = 0; i < iterations; ++i)
    {
        Writer PabloNeruda(termWidth, termHeight, fill);
        PabloNeruda.AddRoot(&wall);
        
        auto t = measure<std::chrono::nanoseconds>::execution([&PabloNeruda](){
            PabloNeruda.RenderSceneByPixel();
        });
        averageTime += t;
    }   
    averageTime /= iterations;
    std::cout << "RenderSceneByPixel():\n\tExecution time: " << averageTime << " ns." << std::endl;
    
    averageTime = 0;
    for (int i = 0; i < iterations; ++i)
    {
        Writer PabloNeruda(termWidth, termHeight, fill);
        PabloNeruda.AddRoot(&wall);
        
        auto t = measure<std::chrono::nanoseconds>::execution([&PabloNeruda](){
            PabloNeruda.RenderScene();
        });
        averageTime += t;
    }   
    averageTime /= iterations;
    std::cout << "RenderScene():\n\tExecution time: " << averageTime << " ns." << std::endl;
    
	return 0;
}


/*
int main(int argc, char *argv[]) {
    constexpr int termWidth = 20;
	constexpr int termHeight = 15;
    
    int width = 15;
    int heigth = 10;
    std::shared_ptr<CharBuffer> wallBuffer = std::make_shared<CharBuffer>();
    wallBuffer->data = new char[heigth * width];
    wallBuffer->size.height = heigth;
    wallBuffer->size.width = width;
    std::fill_n(wallBuffer->data, width*heigth, '@');
    
    int posx = 0;
    int posy = 0;
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
    
    std::vector<Node*> nodes;
    nodes.emplace_back(&window);
    nodes.emplace_back(&window2);
    nodes.emplace_back(&wall);
    
    
    char* buffer = new char[termHeight*termWidth];
    
    // performance test
    using namespace std::chrono;
    
    int averageTime = 0;
    int iterations = 10;
    for (int i = 0; i < iterations; ++i)
    {
        auto t = measure<std::chrono::nanoseconds>::execution([&nodes, termHeight, termWidth, &buffer](){
            for(int y=0; y< termHeight; ++y) {
                for(int x=0; x< termWidth; ++x) {
                    char c = '.';
                    for(auto& child: nodes) {
                        const char* const cPtr = child->getPixel(x, y);
                        if(cPtr) {
                            c = *cPtr;
                            break;
                        }
                    }
                    buffer[x+y*termWidth] = c;
                }
            }
        });
        averageTime += t;
    }   
    averageTime /= iterations;
    std::cout << "Execution time: " << averageTime << " ns." << std::endl;
    
    return 0;
}
*/