#include "ObjectRegister.hpp"

ObjectRegister::ObjectRegister() {}
void ObjectRegister::insertIntoRegister(GameObject *object)
{
    if (isInVector(object))
    {
        return;
    }
    this->objectRegister.push_back(object);
}
void ObjectRegister::deleteFromRegister(GameObject *object)
{
    if (!isInVector(object))
    {
        return;
    }

    vector<GameObject *>::iterator it;

    for (it = objectRegister.begin(); it != objectRegister.end(); ++it)
    {
        if (*it == object)
        {
            break;
        }
    }

    objectRegister.erase(it);
}
int ObjectRegister::getCountOfObjectsInRegister()
{
    return this->objectRegister.size();
}
GameObject *ObjectRegister::getObjectAt(int index)
{
    if (index < 0 || index >= this->objectRegister.size())
    {
        return nullptr;
    }
    return this->objectRegister.at(index);
}
bool ObjectRegister::isInVector(GameObject *object)
{
    for (GameObject *o : this->objectRegister)
    {
        if (o == object)
        {
            return true;
        }
    }

    return false;
}
// return a copy
vector<GameObject *> ObjectRegister::getAllObjects()
{
    return this->objectRegister;
}