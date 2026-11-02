#include <vector>
#include "GameObject.hpp"
// The object register is used for keeping track of all participating game objects. Only registered game objects will be considered, e.g., in rendering or movement with collision.
using namespace std;

class ObjectRegister
{
private:
    // List of all registered objects.
    vector<GameObject *> objectRegister;

public:
    ObjectRegister();
    // Registers the passed object if it is not already in the register.
    void insertIntoRegister(GameObject *object);
    // Deletes the object from the register if it is included.
    void deleteFromRegister(GameObject *object);
    // Returns the count of objects in the register.
    int getCountOfObjectsInRegister();
    // Returns the object at the passed index.
    GameObject *getObjectAt(int index);
    bool isInVector(GameObject *object);
    vector<GameObject *> getAllObjects();
};
