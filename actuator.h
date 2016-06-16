
class Actuator
{
public:
    Actuator();
    ~Actuator();

    float getAngle();

    void moveUp();
    void moveDown();

    void stopUp();
    void stopDown();

private:
    float angle;

};
