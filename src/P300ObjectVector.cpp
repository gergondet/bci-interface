#include <bci-interface/P300ObjectVector.h>

#include <iostream>

namespace bciinterface
{

P300ObjectVector::P300ObjectVector() 
{
    m_objects.resize(0);
}

P300ObjectVector::~P300ObjectVector() 
{
    for(size_t i = 0; i < m_objects.size(); ++i)
    {
        delete m_objects[i];
    }
    m_objects.resize(0);
}

void P300ObjectVector::AddObject(P300Object * object)
{
    if(object)
    {
        for(size_t i = 0; i < m_objects.size(); ++i)
        {
            if( m_objects[i] == object )
            {
                std::cerr << "Could not add object " << object->GetName() << ", an object with the same name already exists!" << std::endl;
                return;
            }
        }
        m_objects.push_back(object);
    }
}

void P300ObjectVector::RemoveObject(const std::string & name)
{
    for(std::vector<P300Object *>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
    {
        if((*it)->GetName() == name)
        {
            m_objects.erase(it);
            delete *it;
        }
    }
}

void P300ObjectVector::ClearObjects()
{
    for(size_t i = 0; i < m_objects.size(); ++i)
    {
        delete m_objects[i];
    }
    m_objects.resize(0);
}

void P300ObjectVector::DrawObjects(sf::RenderWindow * app, int activeObject)
{
    for(size_t i = 0; i < m_objects.size(); ++i)
    {
        if( i == activeObject )
        {
            m_objects[i]->DrawActive(app);
        }
        else
        {
            m_objects[i]->DrawInactive(app);
        }
    }
}

} // namespace

