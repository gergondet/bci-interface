#include <bci-interface/BCIInterface.h>
#include <bci-interface/Background/BufferBG.h>
#include <bci-interface/DisplayObject/SSVEPStimulus.h>
#include <bci-interface/DisplayObject/SpriteObject.h>
#include <bci-interface/DisplayObject/TextObject.h>
#include <bci-interface/CommandReceiver/UDPReceiver.h>

#include "WhackInterpreter.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <boost/thread.hpp>

#include <iostream>
#include <sstream>
#include <cstdlib>
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
    if(!fullscreen)
    {
        width = 1024;
        height = 768;
    }

    srand(time(0));

    BCIInterface * bciinterface = new BCIInterface(width, height);

    UDPReceiver * receiver = new UDPReceiver(1111);
    bciinterface->SetCommandReceiver(receiver);

    WhackInterpreter * interpreter = new WhackInterpreter();
    bciinterface->SetCommandInterpreter(interpreter);

    unsigned char * bgcolor = new unsigned char[640*480*4];
    std::memset(bgcolor, 0x77, 640*480*4); 
    BufferBG * bg = new BufferBG(640,480, width, height);
    bg->UpdateFromBuffer_RGB(bgcolor);
    bciinterface->SetBackground(bg);

    bciinterface->AddObject(new SSVEPStimulus(6, 60, width/2, 150, 300, 300, "data/mole_1.png", "data/mole_1_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(8, 60, width-150, height/2, 300, 300, "data/mole_2.png", "data/mole_2_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(11, 60, width/2, height-150, 300, 300, "data/mole_3.png", "data/mole_3_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(13, 60, 150, height/2,300, 300, "data/mole_4.png", "data/mole_4_HL.png"));

    SpriteObject * target = new SpriteObject("data/wanted.png");
    target->SetPosition(width/2-150, height/2-200);
    unsigned int targetid = rand() % 4;
    interpreter->SetCurrentTarget(targetid);
    unsigned int line = targetid / 2;
    unsigned int clmn = targetid % 2;
    target->SetSubRect(clmn*300,line*400,300,400);
    bciinterface->AddObject(target);

    TextObject * txtobj = new TextObject("Score: 0");
    txtobj->SetPosition(width-150, height-50);
    bciinterface->AddObject(txtobj);

    int * out_cmd = new int(-1);
    float timeout = 2;
    sf::RenderWindow * app = 0;
    boost::thread th(boost::bind(&bciinterface::BCIInterface::DisplayLoop, bciinterface, app, fullscreen, out_cmd, timeout));

    unsigned int score = 0;
    sf::SoundBuffer buffer;
    buffer.loadFromFile("data/score.wav");

    sf::Sound scoreSound;
    scoreSound.setBuffer(buffer);

    while(*out_cmd != 0)
    {
        targetid = rand() % 4;
        interpreter->SetCurrentTarget(targetid+1);
        line = targetid / 2;
        clmn = targetid % 2;
        std::cout << "targetid " << targetid << " line " << line << " clmn " << clmn << std::endl;
        std::cout << clmn*300<< " " <<line*400<< " " <<clmn*300+300<< " " <<line*400+400 << std::endl;
        target->SetSubRect(clmn*300,line*400,300,400);

        bciinterface->StartParadigm();
        bciinterface->SetCommandInterpreter(interpreter);

        while(bciinterface->ParadigmStatus())
        {
#ifndef WIN32
            usleep(100000);
#else
			Sleep(100);
#endif
        }
        bciinterface->SetCommandInterpreter(0);
        score++;
        scoreSound.play();
        std::stringstream ss;
        ss << "Score: " << score;
        txtobj->SetText(ss.str());
        target->SetSubRect(0, 800, 300, 400);
        sleep(5);
    }

    if(app)
    {
        app->close();
    }

    delete bciinterface;
    delete interpreter;
    delete receiver;

    return 0;
}
