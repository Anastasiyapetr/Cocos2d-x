/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d:: Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void HelloWorld::RestartGame(Ref* pSender);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void TestNeighbours(int number_rectangle, std::vector<int>& neighbours, short color_rect);
    void DropBlocks(std::vector<int>& neighbours);
    int setBlackBlock(int number, int num_column);
    void setColor(std::vector<int>& neighbours);
    int findTopColumn(int, std::vector<int>& neighbours, int numColumn);
    void setNumberDeletions(std::vector<int>& neighbours);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
    std::list<cocos2d::Sprite*> field;
    std::vector<short> color;
    std::vector<short> NumberDeletions;
    float size_rect;
    float width_back, height_back;
    float scale_back_x, scale_back_y;
    float width_scene, height_scene;
    float origin_x, origin_y;
    int count_sprites;
};

#endif // __HELLOWORLD_SCENE_H__
