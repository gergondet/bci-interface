#ifndef _H_P300OBJECT_VECTOR_H_
#define _H_P300OBJECT_VECTOR_H_

#include <bci-interface/P300Object.h>
#include <vector>

namespace bciinterface
{

class P300ObjectVector
{
    public:
        P300ObjectVector();

        ~P300ObjectVector();

        void AddObject(P300Object * object);

        void RemoveObject(const std::string & name);

        void ClearObjects();

        void DrawObjects(sf::RenderWindow * app, int activeObject);

    private:
        std::vector<P300Object *> m_objects;

};

}

#endif
