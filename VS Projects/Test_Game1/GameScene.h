#ifndef _GameScene_H_
#define _GameScene_H_

#include "../../../QwerkE_Framework/Source/Core/Scenes/Scene.h"

class GameScene : public Scene
{
public:
    GameScene();
    ~GameScene();

    void Initialize();
    void Draw();

private:
    void p_Update(double TimePassed);
};

#endif // _GameScene_H_
