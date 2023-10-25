#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "Action.h"
#include "Figure.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include <CCEventKeyboard.h>
#include <CCEventListenerKeyboard.h>
#include <unordered_map>
#include <vector>
#define COUNT_BUTTON 4
USING_NS_CC;

class HelloWorld : public cocos2d::Scene {
public:
  static cocos2d::Scene *createScene();
  virtual bool init();
  void defaultSettings();
  //////Create Widgets//////////
  Label *createLabel(const std::string &, float size, float, float,
                     const std::string &, Color3B &);
  ui::Button *createButton(const std::string &normal,
                           const std::string &selected,
                           const std::string &disabled, const std::string &,
                           const Color3B &, const std::string &, float);
  Sprite *createSprite(std::string);
  void createActionButton(Sprite *);

  //////creating figures and actions using factories//////

  void createFigure(Sprite *panel);
  void createAction();

  ////control the visibility of figures////
  void getVisibleFigure(int);

  ///// check the start of the touch action, used when interacting with
  ///buttons/////
  bool isBeginTouch(ui::Widget::TouchEventType type);

  //////change screen resolution/////
  void changeDesignResolutionSize();

  ~HelloWorld() {
    delete figure;
    delete act;
  }

  // a selector callback
  void ManagePanel(Ref *pSender, ui::Widget::TouchEventType type, Sprite *);
  void makeAction(Ref *pSender, ui::Widget::TouchEventType type, std::string,
                  int &);
  void onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event);

  // implement the "static create()" method manually
  CREATE_FUNC(HelloWorld);

private:
  bool isPanelActivated;
  int numActiveFigure;
  bool isInvisible;
  std::unordered_map<ui::Button *, bool> buttons;
  std::unordered_map<Sprite *, std::string> figures;
  std::unordered_map<std::string, bool> activeAction;
  std::vector<std::string> nameFigure;
  Figure *figure;
  Act *act;
  float startPosition_x, startPosition_y;
  int tagPrevAct;
  int tagCount;
};

#endif // __HELLOWORLD_SCENE_H__
