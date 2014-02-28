#ifndef _H_FONTMANAGER_H_
#define _H_FONTMANAGER_H_

/* This class manages available font for the interface, by default, fonts will
 * be searched in a default directory (/usr/share/fonts), additionnal
 * directories to be searched can be added, default font is arial but can be
 * changed */

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

#include <bci-interface/BCIAPI.h>

namespace sf
{
    class Font;
}

namespace bciinterface
{

struct FontManagerImpl;

class FontManager : public boost::noncopyable
{
public:
    BCIINTERFACE_API FontManager();

    BCIINTERFACE_API ~FontManager();

    /* Returns the default font (arial.ttf if not changed by the user), can throw if font cannot be loaded */
    BCIINTERFACE_API sf::Font & GetDefaultFont();

    /* Returns a font by name, can throw if font cannot be loaded */
    BCIINTERFACE_API sf::Font & GetFont(const std::string & font);

    /* Load a font, search for it in all font directories configured by the user, can throw if font cannot be loaded */
    BCIINTERFACE_API sf::Font & LoadFont(const std::string & font);

    /* Change the font returned by GetDefaultFont */
    BCIINTERFACE_API void SetDefaultFont(const std::string & font);

    /* Add a font directory that will be searched when loading a new font */
    BCIINTERFACE_API void AddFontDirectory(const std::string & dir);
private:
    boost::shared_ptr<FontManagerImpl> impl;
};

}

#endif
