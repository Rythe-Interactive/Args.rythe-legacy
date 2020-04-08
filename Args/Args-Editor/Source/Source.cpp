#include <Args-Core.h>
#include <Args-Physics.h>
#include <Args-Rendering.h>
#include <Args-Math.h>
#include <Args-Window.h>
#include <Args-Input.h>
#include <Args-Audio.h>

#include <Module/TestModule.h>
#include <Hierarchy/System/SceneSystem.h>
#include <Hierarchy/Module/SceneModule.h>
#include "Module/PhysicsModule.h"

int main(int argc, char* argv[])
{
	Args::Engine engine(argc, argv);

	engine.AttachModule<Args::WindowModule>();
	engine.AttachModule<TestModule>();
	engine.AttachModule<Args::SceneModule>();
	engine.AttachModule<Args::RenderingModule>();
	engine.AttachModule<Args::InputModule>();
	engine.AttachModule<Args::AudioModule>();
	engine.AttachModule<Args::PhysicsModule>();

	engine.Initialise();

	engine.Run();

	system("pause");
	return engine.ExitCode();
}

//#include <iostream>
//#include <windows.h>   // WinApi header
//using namespace std;    // std::cout, std::cin
//int main()
//{
//    HANDLE  hConsole;
//    int k;
//
//    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//    // you can loop k higher to see more color choices
//    for (k = 1; k < 255; k++)
//    {
//        // pick the colorattribute k you want
//        SetConsoleTextAttribute(hConsole, k);
//        cout << k << " I want to be nice today!" << endl;
//    }
//
//    cin.get(); // wait
//    return 0;
//}