
class Flyer
{
public:
    Flyer();
    ~Flyer();

    bool finishThrow = false;

    bool isOK();

    void moveLeft();
    void moveRight();
    void moveForward();
    void moveBack();

    void stopLeft();
    void stopRight();
    void stopForward();
    void stopBack();

    void arriving();

};
