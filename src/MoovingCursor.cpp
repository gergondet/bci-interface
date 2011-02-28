#include <bci-interface/MoovingCursor.h>

#include <SFML/Graphics.hpp>
#include <iostream>

namespace bciinterface
{

struct MoovingCursorImpl
{
private:
    sf::Shape cursor;
    bool cursorDisplay;
    float finalPosX;

public:



    MoovingCursorImpl(float x_init, float y_init, float size, float x_fin, int r, int g, int b, int a) :
        cursor(sf::Shape::Rectangle(x_init, y_init, x_init+size/5, y_init+size, sf::Color(r, g, b, a))) ,
        cursorDisplay(true)

    {
        finalPosX=x_fin;
    	cursor.EnableFill(true);
    }

    ~MoovingCursorImpl()
    {
    }

   void SetCursorDisplay(bool const displayCur)
    {
        this->cursorDisplay = displayCur;
    }

    bool CursorDisplay()
    {
        return cursorDisplay;
    }

    float GetfinalPosX()
    {
        return finalPosX;
    }

    sf::Shape * GetCursor()
    {
        return &cursor;
    }

    void SetCursorX(float posX)
     {
         this->cursor.SetX(posX);
     }


}; //class MoovingCursorImpl



MoovingCursor::MoovingCursor(float x_init, float y_init, float size, float x_fin, int r, int g, int b, int a) :
    m_mvcrsimpl(new MoovingCursorImpl(x_init, y_init, size, x_fin, r, g, b, a))
{
}

void MoovingCursor::SetCursorDisplay(bool const displayCur)
 {
     m_mvcrsimpl->SetCursorDisplay(displayCur);
 }

bool MoovingCursor::CursorDisplay()
{
    return m_mvcrsimpl->CursorDisplay();
}

float MoovingCursor::GetfinalPosX()
{
    return m_mvcrsimpl->GetfinalPosX();
}

sf::Shape * MoovingCursor::GetCursor()
{
    return m_mvcrsimpl->GetCursor();
}

void MoovingCursor::SetCursorX(float posX)
 {
	m_mvcrsimpl->SetCursorX(posX);
 }


} // namespace bciinterface

