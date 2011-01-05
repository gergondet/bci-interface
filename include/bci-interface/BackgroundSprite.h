/*! \brief BackgroundSprite class gets the video feed from the robot and put it in a sprite
 * \author Pierre Gergondet
 * Copyright: JRL, CNRS-AIST
 * Date: 24/11/2010
 */

#ifndef _H_BACKGROUNDSPRITE_H_
#define _H_BACKGROUNDSPRITE_H_

#include <boost/shared_ptr.hpp>
#include <SFML/Graphics.hpp>

#ifdef WIN32
#define SSVEPINTERFACE_API __declspec(dllexport)
#else
#define SSVEPINTERFACE_API 
#endif

namespace bciinterface
{

struct BackgroundSpriteImpl;

class BackgroundSprite 
{
public:
    /*! \brief Constructor */ 
    SSVEPINTERFACE_API BackgroundSprite(const std::string & visionName, unsigned short visionPort);

    /*! \brief Initialize */
    SSVEPINTERFACE_API void Initialize();

    /*! \brief Update loop */
    SSVEPINTERFACE_API void UpdateLoop();

    /*! \brief Close */
    SSVEPINTERFACE_API void Close();

    /*! \brief Sprite accessor */
    SSVEPINTERFACE_API sf::Sprite * GetSprite();

private:
    boost::shared_ptr<BackgroundSpriteImpl> m_bsimpl;
};

}

#endif
