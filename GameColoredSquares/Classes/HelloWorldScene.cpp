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
#include "AudioEngine.h"
#include "ui/CocosGUI.h"
#define FIELD_WIDTH 16
#define FIELD_HEIGHT 10

USING_NS_CC;

Scene *HelloWorld::createScene() { return HelloWorld::create(); }

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(std::string filename) {
  printf("Error while loading: %s\n", filename);
  printf("Depending on how you compiled you might have to add 'Resources/' in "
         "front of filenames in HelloWorldScene.cpp\n");
  exit(-3);
}

// init scene
bool HelloWorld::init() {
  if (!Scene::init()) {
    return false;
  }
  auto bg = cocos2d::LayerColor::create(Color4B(50, 0, 130, 255));
  this->addChild(bg);
  Size size_scene = this->getContentSize();
  width_scene = size_scene.width;
  height_scene = size_scene.height;
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  score = 0;

  // create a playing field
  auto background =
      createSprite("background.png", visibleSize.width / 2 + origin.x,
                   visibleSize.height / 2 + origin.y);
  Size size_back = background->getContentSize();
  width_back = size_back.width;
  height_back = size_back.height;
  size_rect = height_scene / (FIELD_HEIGHT + 4);

  scale_back_y = height_scene / (FIELD_HEIGHT + 4) * FIELD_HEIGHT / height_back;
  scale_back_x = size_rect * FIELD_WIDTH / width_back;
  background->setScale(scale_back_x, scale_back_y);
  this->addChild(background, 0);

  // create a field for information about the game
  auto bar = createSprite("view.png", visibleSize.width / 2,
                          visibleSize.height + origin.y);
  this->addChild(bar, 0);
  float position_bar_x = bar->getPosition().x;
  float position_bar_y = bar->getPosition().y;

  // create a label - Height
  auto Label_height = createLabel("Height", 10, size_rect, position_bar_y - 5);
  auto info_height =
      createSprite("info.png", Label_height->getPosition().x * 2.5,
                   Label_height->getPosition().y);
  this->addChild(info_height, 1);
  auto Label_count_height =
      createLabel("10", 8,
                  info_height->getPosition().x -
                      info_height->getContentSize().width / 10 * 3,
                  Label_height->getPosition().y);

  // create a label - Width
  auto Label_width = createLabel("Width", 10,
                                 info_height->getPosition().x +
                                     info_height->getContentSize().width * 1.5,
                                 Label_height->getPosition().y);
  auto info_width = createSprite("info.png",
                                 Label_width->getPosition().x +
                                     info_height->getContentSize().width,
                                 Label_height->getPosition().y);
  this->addChild(info_width, 1);
  auto Label_count_width = createLabel(
      "16", 8,
      info_width->getPosition().x - info_width->getContentSize().width / 10 * 3,
      Label_height->getPosition().y);

  // create a label - Colors
  auto Label_color = createLabel("Colors", 10,
                                 info_width->getPosition().x +
                                     info_width->getContentSize().width * 1.5,
                                 Label_height->getPosition().y);
  auto info_color = createSprite("info.png",
                                 Label_color->getPosition().x +
                                     info_height->getContentSize().width,
                                 Label_height->getPosition().y);
  this->addChild(info_color, 1);
  auto Label_count_color = createLabel(
      "3", 8,
      info_color->getPosition().x - info_color->getContentSize().width / 10 * 4,
      Label_height->getPosition().y);

  // Create a label - count available moves
  auto Label_moves = createLabel("Available moves", 10,
                                 info_color->getPosition().x +
                                     info_color->getContentSize().width * 2,
                                 Label_height->getPosition().y);
  auto info_moves = createSprite("info.png",
                                 Label_moves->getPosition().x +
                                     info_height->getContentSize().width * 1.7,
                                 Label_height->getPosition().y);
  this->addChild(info_moves, 1);

  // Create a button start
  auto startButton = createMenu("normal_button.png", "selected_button.png", 1,
                                origin.x + visibleSize.width -
                                    info_height->getContentSize().width / 2 - 2,
                                Label_height->getPosition().y);

  // Create a button exit
  float x = visibleSize.width - size_rect / 2;
  float y = origin.y + size_rect / 2;
  auto closeItem = createMenu("CloseNormal.png", "CloseSelected.png", 2, x, y);

  // create a field of cubes
  srand(time(NULL));
  float shift_ox = size_rect / 2, shift_oy = size_rect / 2;
  origin_x = (width_scene - width_back * scale_back_x) / 2;
  origin_y = (height_scene - height_back * scale_back_y) / 2;

  for (size_t i = 0; i < FIELD_WIDTH; i++) {
    for (size_t j = 0; j < FIELD_HEIGHT; j++) {
      short color_rect = 1 + rand() % 3;
      std::string result = "rectangle_" + std::to_string(color_rect) + ".png";
      auto sprite =
          createSprite(result, origin_x + shift_oy, origin_y + shift_ox);
      Size size_sprite = sprite->getContentSize();
      float width_sprite = size_sprite.width;
      float sprite_scale = size_rect / width_sprite;
      sprite->setScale(sprite_scale, sprite_scale);
      field.push_back(sprite);
      color.push_back(color_rect);
      realIndexBlock.push_back(i * FIELD_HEIGHT + j);
      this->addChild(field.back(), 0);
      shift_ox = shift_ox + size_rect;
    }
    shift_oy = shift_oy + size_rect;
    shift_ox = size_rect / 2;
  }

  // Count available moves
  for (unsigned i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++) {
    is_available.push_back(1);
  }
  count_available_moves = findAvailableMoves(is_available, color);

  std::string count_moves = std::to_string(count_available_moves);
  Label_count_moves = createLabel(
      count_moves, 8,
      info_moves->getPosition().x - info_moves->getContentSize().width / 10 * 3,
      Label_height->getPosition().y);

  // Action
  auto listener = EventListenerTouchOneByOne::create();
  listener->setSwallowTouches(true);
  listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
  return true;
}

cocos2d::Sprite *HelloWorld::createSprite(std::string path, float x, float y) {
  auto res = Sprite::create(path);
  if (res == nullptr) {
    problemLoading(path);
  } else {
    res->setPosition(Vec2(x, y));
  }
  return res;
}

cocos2d::Label *HelloWorld::createLabel(std::string letter, size_t size,
                                        float x, float y) {
  auto label = Label::createWithTTF(letter, "fonts/arial.ttf", size);
  if (label == nullptr) {
    problemLoading("'fonts/arial.ttf'");
  } else {
    label->setPosition(Vec2(x, y));
    label->setColor(Color3B::BLACK);
    if (size > 10) {
      label->setColor(Color3B::RED);
      label->setOpacity(0);
    }
    this->addChild(label, 1);
  }
  return label;
}

cocos2d::MenuItemImage *HelloWorld::createMenu(std::string normal,
                                               std::string selected,
                                               int numMenu, float x, float y) {
  MenuItemImage *menu;
  if (numMenu == 1) {
    menu = MenuItemImage::create(normal, selected,
                                 CC_CALLBACK_1(HelloWorld::RestartGame, this));
  } else if (numMenu == 2) {
    menu = MenuItemImage::create(
        normal, selected, CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    menu->setScale(2.0);
  }
  if (menu == nullptr) {
    problemLoading(normal + selected);
  } else {
    menu->setPosition(Vec2(x, y));
    auto start = Menu::create(menu, NULL);
    start->setPosition(Vec2::ZERO);
    this->addChild(start, 1);
  }
  return menu;
}

int HelloWorld::findAvailableMoves(std::vector<int> &is_available,
                                   std::vector<short> &color) {
  int res = 0;
  for (int i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++) {
    if (is_available[i] == 1 && color[i] != 0) {
      std::vector<int> neighbours;
      int colorBlock = color[i];
      TestNeighbours(i, neighbours, colorBlock);
      if (neighbours.size() >= 3) {
        for (auto k : neighbours) {
          is_available[k] = 0;
        }
        res++;
      }
    }
  }
  return res;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
  float touch_position_x = touch->getLocation().x;
  float touch_position_y = touch->getLocation().y;
  if (touch_position_x >= origin_x &&
      touch_position_x <= origin_x + size_rect * FIELD_WIDTH &&
      touch_position_y >= origin_y &&
      touch_position_y <= origin_y + size_rect * FIELD_HEIGHT) {
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
        std::string music_path = "audio/action.mp3";
        experimental::AudioEngine::play2d(music_path);
        for (unsigned i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++) {
          is_available[i] = 1;
        }
        count_available_moves = findAvailableMoves(is_available, color);
        std::string count_moves = std::to_string(count_available_moves);
        Label_count_moves->setString(count_moves);
        if (count_available_moves == 0) {
          GameOver();
        }
      }
    }
  }
  return true;
}

void HelloWorld::GameOver() {
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  // game over music
  std::string music_path = "audio/game_over.mp3";
  experimental::AudioEngine::play2d(music_path);

  auto LabelGameOver = createLabel("GAME OVER", 20, visibleSize.width / 2,
                                   visibleSize.height / 2 + origin.y * 2.2);
  auto fadeIn = FadeIn::create(3.0f);
  LabelGameOver->runAction(fadeIn);

  for (auto i : color) {
    if (i == 0) {
      score++;
    }
  }
  std::string commonScore = std::to_string(score);
  commonScore = "your score - " + commonScore;
  auto LabelScore = createLabel(commonScore, 15, visibleSize.width / 2,
                                visibleSize.height / 2 + origin.y * 1.5);
  LabelScore->runAction(fadeIn->clone());
}

void HelloWorld::TestNeighbours(int number_rectangle,
                                std::vector<int> &neighbours,
                                short color_rect) {
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

void HelloWorld::DropBlocks(std::vector<int> &neighbours) {
  for (auto i : neighbours) {
    int num_column = i / FIELD_HEIGHT + 1;
    setBlackBlock(i, num_column);

    if ((i % (FIELD_HEIGHT - 1)) != 9) {
      for (int j = i; j != (num_column * FIELD_HEIGHT - 1); j++) {
        for (int k = j + 1; k != num_column * FIELD_HEIGHT; k++) {
          if (color[k] != 0) {
            auto moveDown = MoveBy::create(0.9, Vec2(0, -size_rect));
            auto iter = field.begin();
            int count = 0;
            for (; count != realIndexBlock[k]; count++) {
              if ((count < num_column * FIELD_HEIGHT - 1)) {
                iter++;
              } else {
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
}

void HelloWorld::setBlackBlock(int number, int num_column) {
  auto iter = field.begin();
  int tmp = 0;
  int countBkackBlock = 0;
  while (tmp != realIndexBlock[number]) {
    tmp++;
    iter++;
  }
  (*iter)->setColor(Color3B::BLACK);
}

void HelloWorld::setColor(std::vector<int> &neighbours) {
  std::vector<int> indexColumns;
  std::vector<int> countNegative;
  std::vector<int> countZeroColor;
  for (auto i : neighbours) {
    int numColumn = i / FIELD_HEIGHT;
    realIndexBlock[i] = -1;
    color[i] = 0;
    indexColumns.push_back(numColumn);
  }
  std::sort(indexColumns.begin(), indexColumns.end());
  indexColumns.erase(std::unique(indexColumns.begin(), indexColumns.end()),
                     indexColumns.end());

  for (auto i : indexColumns) {
    int count = 0;
    for (int start = i * FIELD_HEIGHT; start <= ((i + 1) * FIELD_HEIGHT - 1);
         start++) {
      if (realIndexBlock[start] == -1) {
        count++;
      }
    }
    countNegative.push_back(count);
    countZeroColor.push_back(count);
  }

  for (auto i : indexColumns) {
    int start = 0;
    for (int begin = i * FIELD_HEIGHT; begin <= ((i + 1) * FIELD_HEIGHT - 1);
         begin++) {
      if (realIndexBlock[begin] == -1) {
        start = begin;
        break;
      }
    }
    int stop = start + 1;
    while (stop <= ((i + 1) * FIELD_HEIGHT - 1)) {
      if (realIndexBlock[stop] != -1) {
        realIndexBlock[start] = realIndexBlock[stop];
        color[start] = color[stop];
        start++;
      }
      stop++;
    }
  }

  for (size_t i = 0; i < indexColumns.size(); i++) {
    int start = (indexColumns[i] + 1) * FIELD_HEIGHT - 1;
    while (countNegative[i] != 0) {
      realIndexBlock[start] = -1;
      color[start] = 0;
      start--;
      countNegative[i]--;
    }
  }
}

void HelloWorld::menuCloseCallback(Ref *pSender) {
  Director::getInstance()->end();
}

void HelloWorld::RestartGame(Ref *pSender) {
  cocos2d::Scene *scene = HelloWorld::createScene();
  Director::getInstance()->replaceScene(scene);
}
