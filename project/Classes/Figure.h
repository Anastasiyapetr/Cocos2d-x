#ifndef __FIGURE_H__
#define __FIGURE_H__
#include "cocos2d.h"
USING_NS_CC;

class Figure {
public:
  virtual ~Figure() {}
  virtual cocos2d::Sprite *create() = 0;
};

class Triangle : public Figure {
public:
  cocos2d::Sprite *create() override {
    cocos2d::Sprite *sprite = cocos2d::Sprite::create("Triangle.png");
    return sprite;
  }
  ~Triangle() override { CCLOG("Delete triangle"); }
};

class Square : public Figure {
  cocos2d::Sprite *create() override {
    cocos2d::Sprite *sprite = cocos2d::Sprite::create("Square.png");
    return sprite;
  }
};

class ellipse : public Figure {
  cocos2d::Sprite *create() override {
    cocos2d::Sprite *sprite = cocos2d::Sprite::create("Ellipse.png");
    return sprite;
  }
};

#endif  // __FIGURE_H__