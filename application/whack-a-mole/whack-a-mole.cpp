#include <bci-interface/BCIInterface.h>
#include <bci-interface/Background/BufferBG.h>
#include <bci-interface/DisplayObject/SSVEPStimulus.h>
#include <bci-interface/DisplayObject/SpriteObject.h>
#include <bci-interface/CommandReceiver/UDPReceiver.h>
#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>
#include <cstring>

#ifdef WIN32
#include <Windows.h>
void sleep(DWORD t)
{
	Sleep(1000*t);
}
#endif

using namespace bciinterface;

int main(int argc, char * argv[])
{
    bool fullscreen = true;
    unsigned int width = 1280;
    unsigned int height = 1024;

    BCIInterface * bciinterface = new BCIInterface(width, height);

    UDPReceiver * receiver = new UDPReceiver(1111);
    bciinterface->SetCommandReceiver(receiver);

    SimpleInterpreter * interpreter = new SimpleInterpreter();
    bciinterface->SetCommandInterpreter(interpreter);

    unsigned char * bgcolor = new unsigned char[640*480*4];
    std::memset(bgcolor, 0x77, 640*480*4); 
    BufferBG * bg = new BufferBG(640,480);
    bg->UpdateFromBuffer_RGB(bgcolor);
    bciinterface->SetBackground(bg);

    bciinterface->AddObject(new SSVEPStimulus(6, 60, width/2, 150, 300, 300, "data/mole_1.png", "data/mole_1_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(8, 60, width-150, height/2, 300, 300, "data/mole_2.png", "data/mole_2_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(11, 60, width/2, height-150, 300, 300, "data/mole_3.png", "data/mole_3_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(13, 60, 150, height/2,300, 300, "data/mole_4.png", "data/mole_4_HL.png"));

    SpriteObject * target = new SpriteObject("data/wanted.png");
    target->SetSubRect(0,0,300,400);
    target->SetPosition(width/2-150, height/2-200);
    bciinterface->AddObject(target);

    bciinterface->StartParadigm();
    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;
    delete interpreter;
    delete receiver;

    return 0;
}
