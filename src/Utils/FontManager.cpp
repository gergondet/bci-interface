#include "bci-interface/Utils/FontManager.h"

#include <SFML/Graphics/Font.hpp>

#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <sstream>

namespace bciinterface
{

struct FontManagerImpl
{
    FontManagerImpl()
    : loaded_fonts(), font_directories(), default_font("arial.ttf")
    {
        font_directories.insert("/usr/share/fonts");
    }

    ~FontManagerImpl()
    {
        for(auto it = loaded_fonts.begin(); it != loaded_fonts.end(); ++it)
        {
            delete it->second;
            it->second = 0;
        }
    }

    inline sf::Font & GetDefaultFont()
    {
        return GetFont(default_font);
    }

    inline sf::Font & GetFont(const std::string & font)
    {
        if( loaded_fonts.count(font) )
        {
            return *(loaded_fonts[font]);
        }
        if( LoadFont(font) )
        {
            return *(loaded_fonts[font]);
        }
        std::stringstream ss; ss << "Font " << font << " could not be loaded";
        throw(ss.str().c_str());
    }

    bool LoadFont(const std::string & font)
    {
        for(auto it = font_directories.begin(); it != font_directories.end(); ++it)
        {
            if( LoadFont(bfs::path(*it), font) ) return true;
        }
        return false;
    }

    bool LoadFont(const bfs::path && path, const std::string & font)
    {
        if(bfs::exists(path))
        {
            if( is_regular_file(path) )
            {
                if(path.filename() == font)
                {
                    sf::Font * f = new sf::Font();
                    f->loadFromFile(path.string());
                    loaded_fonts[const_cast<std::string&>(font)] = f;
                    return true;
                }
            }
            if( is_directory(path) )
            {
                std::vector<bfs::path> ls_path;
                std::copy(bfs::directory_iterator(path), bfs::directory_iterator(), std::back_inserter(ls_path));
                for(auto it = ls_path.begin(); it != ls_path.end(); ++it)
                {
                    if( LoadFont(*it, font) ) return true;
                }
            }
        }
        return false;
    }

    void SetDefaultFont(const std::string & font)
    {
        default_font = font;
    }

    void AddFontDirectory(const std::string & dir)
    {
        font_directories.insert(dir);
    }

    std::map<std::string, sf::Font*> loaded_fonts;
    std::set<std::string> font_directories;
    std::string default_font;
};

FontManager::FontManager() : impl(new FontManagerImpl())
{}

FontManager::~FontManager()
{}

sf::Font &  FontManager::GetDefaultFont()
{
    return impl->GetDefaultFont();
}

sf::Font &  FontManager::GetFont(const std::string & font)
{
    return impl->GetFont(font);
}

sf::Font &  FontManager::LoadFont(const std::string & font)
{
    return impl->GetFont(font);
}

void FontManager::SetDefaultFont(const std::string & font)
{
    impl->SetDefaultFont(font);
}

void FontManager::AddFontDirectory(const std::string & dir)
{
    impl->AddFontDirectory(dir);
}

}
