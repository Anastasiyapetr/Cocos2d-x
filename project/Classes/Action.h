#ifndef __ACTION_H__
#define __ACTION_H__
#include "Figure.h"
#include "cocos2d.h"

class Act {
private:
  float time;

public:
  Act() { this->time = 0; }
  Act(float t) { this->time = t; }
  virtual ~Act(){};
  virtual cocos2d::Action *create() = 0;
  float getTime() { return time; }
};

class Rotate : public Act {
public:
  Rotate(float time, float degr) : Act(time) { this->degrees = degr; };
  cocos2d::Action *create() override {
    cocos2d::Action *rotate = cocos2d::RotateTo::create(getTime(), degrees);
    return rotate;
  }

private:
  float degrees;
};

class Scale : public Act {
public:
  Scale(float time, double sc) : Act(time) { this->sc = sc; }
  cocos2d::Action *create() override {
    cocos2d::Action *scale = cocos2d::ScaleTo::create(getTime(), sc);
    return scale;
  }

private:
  float sc;
};

class Translate : public Act {
public:
  Translate(float time, float x, float y) : Act(time) {
    this->position_x = x;
    this->position_y = y;
  }
  cocos2d::Action *create() override {
    cocos2d::Action *translate = cocos2d::MoveTo::create(
        getTime(), cocos2d::Vec2(position_x, position_y));
    return translate;
  }

private:
  float position_x;
  float position_y;
};

class Hid : public Act {
public:
  Hid(float time, float fd) : Act(time) { this->fade = fd; };
  cocos2d::Action *create() override {
    cocos2d::Action *hide = cocos2d::FadeTo::create(getTime(), fade);
    return hide;
  }

private:
  float fade;
};

#endif // __ACTION_H__