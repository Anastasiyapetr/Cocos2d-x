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

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#define FIELD_WIDTH 16
#define FIELD_HEIGHT 10


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init
    if (!Scene::init())
    {
        return false;
    }
    auto bg = cocos2d::LayerColor::create(Color4B(50, 0, 130, 255));
    this->addChild(bg);
    Size size_scene = this->getContentSize();
    width_scene = size_scene.width;
    height_scene = size_scene.height;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // create a playing field
    auto background = Sprite::create("background.png");
    if (background == nullptr)
    {
        problemLoading("'background.png'");
    }
    else {
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        Size size_back = background->getContentSize();
        
        width_back = size_back.width;
        height_back = size_back.height;

        scale_back_y = height_scene / (FIELD_HEIGHT + 4) * FIELD_HEIGHT / height_back;
        size_rect = height_scene / (FIELD_HEIGHT + 4);
        scale_back_x = size_rect * FIELD_WIDTH / width_back;

        background->setScale(scale_back_x, scale_back_y);
        this->addChild(background, 0);
    }

    // create a field for information about the game
    auto bar = Sprite::create("view.png");
    bar->setPosition(Vec2(visibleSize.width/2, visibleSize.height + origin.y));
    CCLOG("%f %f", bar->getContentSize().width, bar->getContentSize().height);
    CCLOG("%f %f", bar->getPosition().x, bar->getPosition().y);
    CCLOG("visible size height is %f", visibleSize.height);
    this->addChild(bar, 0);
    float position_bar_x = bar->getPosition().x;
    float position_bar_y = bar->getPosition().y;

    // create a label - Height
    auto Label_height = Label::createWithTTF("Height", "fonts/arial.ttf", 10);
    if (Label_height == nullptr) {
        problemLoading("'fonts/arial.ttf'");
    }
    else
    {
        Label_height->setPosition(Vec2(Label_height->getContentSize().width,
             position_bar_y - Label_height->getContentSize().height/2));
        Label_height->setColor(Color3B::BLACK);
        this->addChild(Label_height, 1);
    }
    auto info_height = Sprite::create("info.png");
    if (info_height == nullptr)
    {
        problemLoading("'info.png'");
    }
    else {
        info_height->setPosition(Vec2(Label_height->getPosition().x + info_height->getContentSize().width,
            position_bar_y - info_height->getContentSize().height / 2));
        this->addChild(info_height, 1);
    }

    auto Label_count_height = Label::createWithTTF("10", "fonts/arial.ttf", 8);
    if (Label_count_height == nullptr) {
        problemLoading("'fonts/arial.ttf'");
    }
    else
    {
        Label_count_height->setPosition(Vec2(info_height->getPosition().x - Label_count_height->getContentSize().width,
            info_height->getPosition().y));
        Label_count_height->setColor(Color3B::BLACK);
        this->addChild(Label_count_height, 1);
    }

    // create a label - Width
    auto Label_width = Label::createWithTTF("Width", "fonts/arial.ttf", 10);
    if (Label_width == nullptr) {
        problemLoading("'fonts/arial.ttf'");
    }
    else
    {
        Label_width->setPosition(Vec2(info_height->getPosition().x + info_height->getContentSize().width*2,
            info_height->getPosition().y));
        Label_width->setColor(Color3B::BLACK);
        this->addChild(Label_width, 1);
    }

    auto info_width = Sprite::create("info.png");
    if (info_width == nullptr)
    {
        problemLoading("'info.png'");
    }
    else {
        info_width->setPosition(Vec2(Label_width->getPosition().x + info_width->getContentSize().width,
            position_bar_y - info_width->getContentSize().height / 2));
        this->addChild(info_width, 1);
    }

    auto Label_count_width = Label::createWithTTF("16", "fonts/arial.ttf", 8);
    if (Label_count_width == nullptr) {
        problemLoading("'fonts/arial.ttf'");
    }
    else
    {
        Label_count_width->setPosition(Vec2(info_width->getPosition().x - Label_count_width->getContentSize().width,
            info_width->getPosition().y));
        Label_count_width->setColor(Color3B::BLACK);
        this->addChild(Label_count_width, 1);
    }
    // create a label - Colors
    auto Label_color = Label::createWithTTF("Colors", "fonts/arial.ttf", 10);
    if (Label_color == nullptr) {
        problemLoading("'fonts/arial.ttf'");
    }
    else
    {
        Label_color->setPosition(Vec2(info_width->getPosition().x + info_width->getContentSize().width * 2,
            info_width->getPosition().y));
        Label_color->setColor(Color3B::BLACK);
        this->addChild(Label_color, 1);
    }
    auto info_color = Sprite::create("info.png");
    if (info_color == nullptr)
    {
        problemLoading("'info.png'");
    }
    else {
        info_color->setPosition(Vec2(Label_color->getPosition().x + info_color->getContentSize().width,
            position_bar_y - info_color->getContentSize().height / 2));
        this->addChild(info_color, 1);
    }

    auto Label_count_color = Label::createWithTTF("3", "fonts/arial.ttf", 8);
    if (Label_count_color == nullptr) {
        problemLoading("'fonts/arial.ttf'");
    }
    else
    {
        Label_count_color->setPosition(Vec2(info_color->getPosition().x - Label_count_color->getContentSize().width*2,
            info_color->getPosition().y));
        Label_count_color->setColor(Color3B::BLACK);
        this->addChild(Label_count_color, 1);
    }

    //Create a button start

    auto startButton = MenuItemImage::create(
        "normal_button.png",
        "selected_button.png",
        CC_CALLBACK_1(HelloWorld::RestartGame, this));

    if ( startButton == nullptr )
    {
        problemLoading("'normal_button.png' and 'selected_button.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - startButton->getContentSize().width / 2 - 2;
        float y = info_color->getPosition().y;
        startButton->setPosition(Vec2(x, y));
    }

    auto start = Menu::create(startButton, NULL);
    start->setPosition(Vec2::ZERO);
    this->addChild(start, 1);

    // create a field of cubes
    
    srand(time(NULL));
    float shift_ox = size_rect/2, shift_oy = size_rect/2;
    origin_x = (width_scene - width_back * scale_back_x) / 2;
    origin_y = (height_scene - height_back * scale_back_y) / 2;
    count_sprites = 0;

    for (size_t i = 0; i < FIELD_WIDTH; i++) {
        for (size_t j = 0; j < FIELD_HEIGHT; j++) {
            short color_rect = 1 + rand() % 3;
            std::string result = "rectangle_" + std::to_string(color_rect) + ".png";
            auto sprite = Sprite::create(result);
            if (sprite == nullptr)
            {
                problemLoading("'result'");
            }
            else {
                Size size_sprite = sprite->getContentSize();
                float width_sprite = size_sprite.width;
                sprite->setPosition(Vec2(origin_x + shift_oy, origin_y + shift_ox));
                float sprite_scale = size_rect / width_sprite;
                sprite->setScale(sprite_scale, sprite_scale);
                field.push_back(sprite);
                color.push_back(color_rect);
                NumberDeletions.push_back(0);
                this->addChild(field.back(), 0);
            }
            shift_ox = shift_ox + size_rect;
        }
        shift_oy = shift_oy + size_rect;
        shift_ox = size_rect/2;
    }
 
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) 
{
    float touch_position_x = touch->getLocation().x;
    float touch_position_y = touch->getLocation().y;
    if (touch_position_x >= origin_x && touch_position_x <= origin_x + size_rect * FIELD_WIDTH
        && touch_position_y >= origin_y && touch_position_y <= origin_y + size_rect * FIELD_HEIGHT) {
        int num_rec_x = (touch_position_x - origin_x) / size_rect;
        int num_rec_y = (touch_position_y - origin_y) / size_rect;
        num_rec_x *= FIELD_HEIGHT;
        int number_rectangle = num_rec_x + num_rec_y;
        short color_rect = color[number_rectangle];
        if (color_rect != 0) {
            std::vector<int> neighbours;
            TestNeighbours(number_rectangle, neighbours, color_rect);
            std::sort(neighbours.begin(), neighbours.end());
            if (neighbours.size() >= 3) {
               DropBlocks(neighbours);
            }
        }
    }
    return true;
}

void HelloWorld::TestNeighbours(int number_rectangle, std::vector<int>& neighbours, short color_rect)
{
    bool flag = 0;
    if (number_rectangle >= 0 && number_rectangle < FIELD_HEIGHT * FIELD_WIDTH) {
        if (color[number_rectangle] == color_rect) {
           for (size_t i = 0; i < neighbours.size(); i++) {
                if (number_rectangle == neighbours[i]) {
                    flag = 1;
                }
            }
            if (!flag) {
                neighbours.push_back(number_rectangle);
                if (number_rectangle > FIELD_HEIGHT - 1) {
                    int left_rect = number_rectangle - FIELD_HEIGHT; 
                    TestNeighbours(left_rect, neighbours, color_rect);
                }
                if (number_rectangle < FIELD_WIDTH * FIELD_HEIGHT - FIELD_HEIGHT) {
                    int right_rect = number_rectangle + FIELD_HEIGHT;
                    TestNeighbours(right_rect, neighbours, color_rect);
                }
                if (number_rectangle % FIELD_HEIGHT != 0) {
                    int down_rect = number_rectangle - 1;
                    TestNeighbours(down_rect, neighbours, color_rect);
                }
                if (number_rectangle % FIELD_HEIGHT != FIELD_HEIGHT - 1) {
                    int up_rect = number_rectangle + 1;
                    TestNeighbours(up_rect, neighbours, color_rect);
                }
            }
        }
    }
}

void HelloWorld::DropBlocks(std::vector<int>& neighbours)
{
    for (auto i : neighbours) {
        int num_column = i / FIELD_HEIGHT + 1;
        int countBlackBlock = setBlackBlock(i, num_column);
        
        if ((i % (FIELD_HEIGHT - 1)) != 9) {    
            for (int j = i; j != (num_column * FIELD_HEIGHT - 1); j++) {
                for (int k = j + 1; k != num_column * FIELD_HEIGHT; k++) {
                    if (color[k] != 0) {
                        auto moveDown = MoveBy::create(1, Vec2(0, -size_rect));
                        auto iter = field.begin();
                        int count = 0;
                        for (; count != k + countBlackBlock; count++) {
                            if ((count < num_column * FIELD_HEIGHT - 1) && (color[k] != 0)) {
                                iter++;
                            }
                            else {
                                break;
                            }
                        }

                        (*iter)->runAction(moveDown->clone());
                        break;
                    }
                }
            }
        }
    }
    setColor(neighbours);
    setNumberDeletions(neighbours);
}
void HelloWorld::setNumberDeletions(std::vector<int>& neighbours) {
    for (auto i : neighbours) {
        NumberDeletions[i]++;
    }
}

int HelloWorld::setBlackBlock(int number, int num_column) {
    auto iter = field.begin();
    int tmp = 0;
    int countBkackBlock = 0;
    for (int i = (num_column - 1) * FIELD_HEIGHT; i <= number; i++) {
        countBkackBlock += NumberDeletions[i];
    }
    while (tmp != number + countBkackBlock) {
        tmp++;
        iter++;
    }
    (*iter)->setColor(Color3B::BLACK);
    
    return countBkackBlock;
}

void HelloWorld::setColor(std::vector<int>& neighbours) {
    std::vector<int> copy_neighbours;
    copy_neighbours.insert(copy_neighbours.begin(), neighbours.begin(), neighbours.end());

    for (size_t i = 0; i < neighbours.size(); i++) {
        int numColumn = neighbours[i] / FIELD_HEIGHT + 1;
        if (copy_neighbours[i] != -1) {
            int HeightColumn = findTopColumn(i, copy_neighbours, numColumn);
            
            int start = copy_neighbours[i];
            int stop = start + HeightColumn+1;
            while ( stop < numColumn * FIELD_HEIGHT) {
                color[start] = color[stop];
                start++;
                stop++;
            }
            start = stop - 1;
            while (HeightColumn + 1 != 0) {
                color[start] = 0;
                HeightColumn--;
                start--;
            }
        }
    }
}

int HelloWorld::findTopColumn(int number_block, std::vector<int>& neighbours, int numColumn) {
    int res = 0;
    int tmp = number_block;
    for (size_t j = number_block + 1; j < neighbours.size(); ) {
        if (neighbours[j] - neighbours[tmp] == 1) {
            res++;
            tmp++, j++;
        }
        else {
            break;
        }
    }
    if (res != 0) {
        int count = 0;
        tmp = number_block + 1;
        while (count != res) {
            neighbours[tmp] = -1;
            count++;
            tmp++;
        }
    }
    return res;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}

void HelloWorld::RestartGame(Ref* pSender) 
{
    cocos2d::Scene* scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(scene);
}