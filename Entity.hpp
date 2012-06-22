/* Entity interface for game objects
 *
 */


class Entity
{
public:
  virtual void Think(float dt) = 0;   
  virtual void AddProcess(const Process & proc) = 0; 
};

class PhysicsEntity : public Entity, PhysicsObject
{ 
public: 
  virtual void Think(float dt) = 0; 
  virtual void AddProcess(const Process &proc) = 0; 

    
}; 
