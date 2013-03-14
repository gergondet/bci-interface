#include <bci-interface/BCIInterface.h>
#include <bci-interface/Background/VisionServerBG.h>
#include <bci-interface/DisplayObject/SSVEPStimulus.h>
#include <bci-interface/CommandReceiver/UDPReceiver.h>
#include <bci-interface/CommandInterpreter/SimpleInterpreter.h>
#include <bci-interface/CommandOverrider.h>

#include <bci-interface/Background/BufferBG.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <iostream>
#include <sstream>
#include <fstream>

#ifdef WIN32
#include <Windows.h>
void sleep(DWORD t)
{
	Sleep(1000*t);
}
#include <stdint.h>

std::string dirname(char * path)
{
    return ".";
}

#else
#include <inttypes.h>
#include <libgen.h>
#endif

using namespace bciinterface;

class Cube3D : public bciinterface::DisplayObject
{
public:
    virtual void Display(sf::RenderWindow * app, unsigned int frameCount, sf::Clock & clock)
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(1.0f,1.0f,-4.0f);              // Move Right And Into The Screen
        glRotatef(rotQuad,1.0f,1.0f,0.0f);            // Rotate The Cube
        glBegin(GL_QUADS);
            /* top of cube */
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glVertex3f(-0.5f, 0.5f, -0.5f);
            glVertex3f(-0.5f, 0.5f, 0.5f);
            glVertex3f(0.5f, 0.5f, 0.5f);
            /* bottom of cube */
            glColor3f(1.0f, 1.0f, 0.0f);
            glVertex3f(0.5f, -0.5f, 0.5f);
            glVertex3f(-0.5f, -0.5f, 0.5f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(0.5f, -0.5f, -0.5f);
            /* front of cube */
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(0.5f, 0.5f, 0.5f);
            glVertex3f(-0.5f, 0.5f, 0.5f);
            glVertex3f(-0.5f, -0.5f, 0.5f);
            glVertex3f(0.5f, -0.5f, 0.5f);
            /* back of cube */
            glColor3f(1.0f, 1.0f, 1.0f);
            glVertex3f(-0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            /* right side of cube */
            glColor3f(1.0f, 0.0f, 1.0f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, 0.5f);
            glVertex3f(0.5f, -0.5f, 0.5f);
            glVertex3f(0.5f, -0.5f, -0.5f);
            /* left side of cube */
            glColor3f(0.0f, 1.0f, 1.0f);
            glVertex3f(-0.5f, 0.5f, 0.5f);
            glVertex3f(-0.5f, 0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f, 0.5f);
        glEnd();
        rotQuad -= 0.15f;
    }

    Cube3D() : rotQuad(0) {}

    virtual bool DrawWithGL() { return true; }

    float rotQuad;
};

class GLBG : public bciinterface::Background
{
public:
    GLBG() : rot(0) {}

    virtual bool DrawWithGL() { return true; }

    void UpdateLoop() {}

    void Close() {}

    void Draw(sf::RenderWindow * app)
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();                   // Reset The View
        glTranslatef(0.0f,0.0f,-1.5f);             // Move Left And Into The Screen
        glRotatef(rot,0.0f,1.0f,0.0f);            // Rotate The Cube On X, Y & Z
        glBegin(GL_TRIANGLES);
            glColor3f(0.5f,0.0f,0.0f);
            glVertex3f( 0.0f, 0.5f, 0.0f);
            glColor3f(0.0f,0.5f,0.0f);
            glVertex3f(-0.5f,-0.5f, 0.5f);
            glColor3f(0.0f,0.0f,0.5f);
            glVertex3f( 0.5f,-0.5f, 0.5f);
            glColor3f(0.5f,0.0f,0.0f);
            glVertex3f( 0.0f, 0.5f, 0.0f);
            glColor3f(0.0f,0.0f,0.5f);
            glVertex3f( 0.5f,-0.5f, 0.5f);
            glColor3f(0.0f,0.5f,0.0f);
            glVertex3f( 0.5f,-0.5f, -0.5f);
            glColor3f(0.5f,0.0f,0.0f);
            glVertex3f( 0.0f, 0.5f, 0.0f);
            glColor3f(0.0f,0.5f,0.0f);
            glVertex3f( 0.5f,-0.5f, -0.5f);
            glColor3f(0.0f,0.0f,0.5f);
            glVertex3f(-0.5f,-0.5f, -0.5f);
            glColor3f(0.5f,0.0f,0.0f);
            glVertex3f( 0.0f, 0.5f, 0.0f);
            glColor3f(0.0f,0.0f,0.5f);
            glVertex3f(-0.5f,-0.5f,-0.5f);
            glColor3f(0.0f,0.5f,0.0f);
            glVertex3f(-0.5f,-0.5f, 0.5f);
        glEnd();
        rot += 0.2;
    }

    float rot;
};

int main(int argc, char * argv[])
{
    bool fullscreen = true;
    if(argc > 1)
    {
        std::stringstream ss;
        ss << argv[1];
        ss >> fullscreen;
    }
    unsigned int width = 1680;
    unsigned int height = 1050; 
    unsigned int iwidth = 1024;
    unsigned int iheight = 768;
    if(!fullscreen)
    {
        width = 1024;
        height = 768;
        iwidth = 640;
        iheight = 480;
    }

    BCIInterface * bciinterface = new BCIInterface(width, height);
    UDPReceiver * receiver = new UDPReceiver(1111);
    SimpleInterpreter * interpreter = new SimpleInterpreter();
    bciinterface->SetCommandReceiver(receiver);
    bciinterface->SetCommandInterpreter(interpreter);

    CommandOverrider overrider;
    overrider.AddOverrideCommand(sf::Keyboard::Up, 1);
    overrider.AddOverrideCommand(sf::Keyboard::Right, 2);
    overrider.AddOverrideCommand(sf::Keyboard::Down, 3);
    overrider.AddOverrideCommand(sf::Keyboard::Left, 4);
    bciinterface->SetCommandOverrider(&overrider);

    bool data_compressed = true;
    //bciinterface->SetBackground(new VisionServerBG("hrp2010v", 4242, 640, 480, data_compressed, width, height, 800, 600));
    BufferBG * bufferBG = new BufferBG(640, 480, width, height, iwidth, iheight);
    uint32_t * buffer = new uint32_t[640*480];
    for(unsigned int x = 0; x < 640; ++x)
    {
        for(unsigned int y = 0; y < 480; ++y)
        {
            buffer[640*y + x] = 0xFF0000FF;
            if(x >= 320 && y < 240)
            {
                buffer[640*y + x] = 0xFF00FF00;
            }
            if(x < 320 && y < 240)
            {
                buffer[640*y + x] = 0xFFFF0000;
            }
            if(x < 320 && y >= 240)
            {
                buffer[640*y + x] = 0xFF00FFFF;
            }
        }
    }
    bufferBG->SetSubRect(0, 0, 320, 240);
    bufferBG->SetSubRect(0, 0, 640, 480);
    bufferBG->UpdateFromBuffer_RGB((unsigned char*)buffer);
//    bciinterface->SetBackground(bufferBG);

    std::string dir = dirname(argv[0]);
    dir += "/";
    bciinterface->AddObject(new SSVEPStimulus(6, 60, width/2, 100, 200,200, dir + "UP.png", dir + "UP_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(8, 60, width-100, height/2, 200, 200, dir + "RIGHT.png", dir + "RIGHT_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(10, 60, width/2, height-100, 200, 200, dir + "DOWN.png", dir + "DOWN_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(9, 60, 100, height/2,200, 200, dir + "LEFT.png", dir + "LEFT_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(14, 60, 100, height/2,200, 200, "LEFT.png", "LEFT_HL.png"));
    bciinterface->AddObject(new SSVEPStimulus(9, 60, width/2, height/2, 200, 200, "STOP.png", "STOP_HL.png"));

    bciinterface->DisplayLoop(fullscreen);

    delete bciinterface;
    delete interpreter;
    delete receiver;

    return 0;
}
