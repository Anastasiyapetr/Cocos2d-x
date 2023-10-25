#include "HelloWorldScene.h"
USING_NS_CC;

Scene *HelloWorld::createScene() { return HelloWorld::create(); }

static void problemLoading(const std::string filename) {
  CCLOG("Error while loading: %s\n", filename);
  CCLOG("Depending on how you compiled you might have to add 'Resources/' in "
        "front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init() {
  if (!Scene::init()) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  defaultSettings();

  auto labelHello =
      createLabel("Hello world", 42, origin.x + visibleSize.width / 2,
                  visibleSize.height - 42, "fonts/TimesNewRoman.ttf",
                  Color3B(173, 216, 230));

  auto panel = createSprite("Panel.png");
  panel->setPosition(
      Vec2(origin.x + visibleSize.width / 2,
           visibleSize.height / 2 - panel->getContentSize().height / 2));
  panel->setOpacity(0);

  ui::Widget::ccWidgetTouchCallback callback =
      CC_CALLBACK_2(HelloWorld::ManagePanel, this, panel);
  auto buttonPanel = createButton(
      "ButtonNormal.png", "ButtonPressed.png", "ButtonPressed.png", "Panel",
      Color3B(173, 216, 230), "fonts/TimesNewRoman.ttf", 40);
  buttonPanel->addTouchEventListener(callback);
  float positionMenu_x =
      origin.x + visibleSize.width - buttonPanel->getContentSize().width / 2;
  float positionMenu_y = origin.y + buttonPanel->getContentSize().height / 2;
  buttonPanel->setPosition(Vec2(positionMenu_x, positionMenu_y));

  createFigure(panel);
  createAction();
  createActionButton(panel);

  auto eventListenerKey = EventListenerKeyboard::create();
  eventListenerKey->onKeyPressed =
      CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
  this->_eventDispatcher->addEventListenerWithSceneGraphPriority(
      eventListenerKey, this);
  return true;
}

void HelloWorld::defaultSettings() {
  isPanelActivated = 0;
  numActiveFigure = 0;
  tagPrevAct = 0;
  tagCount = 1;
  isInvisible = 0;
}

Label *HelloWorld::createLabel(const std::string &letter, float size, float x,
                               float y, const std::string &font,
                               Color3B &color) {
  auto label = Label::createWithTTF(letter, font, size);
  if (label == nullptr) {
    problemLoading(font);
  } else {
    label->setPosition(Vec2(x, y));
    label->setColor(color);
    this->addChild(label, 2);
  }
  return label;
}

ui::Button *HelloWorld::createButton(const std::string &normal,
                                     const std::string &selected,
                                     const std::string &disabled,
                                     const std::string &text,
                                     const Color3B &color,
                                     const std::string &fontName, float size) {
  auto button = ui::Button::create(normal, selected, disabled);
  if (button == nullptr) {
    problemLoading(normal + selected + disabled);
  } else {
    button->setTitleText(text);
    button->setTitleColor(color);
    button->setTitleFontSize(size);
    this->addChild(button, 2);
  }
  return button;
}

cocos2d::Sprite *HelloWorld::createSprite(std::string path) {
  auto sprite = Sprite::create(path);
  if (sprite == nullptr) {
    problemLoading(path);
  } else {
    this->addChild(sprite, 1);
  }
  return sprite;
}

void HelloWorld::createActionButton(Sprite *panel) {
  float positionButtonX =
      panel->getPosition().x - panel->getContentSize().width / 50 * 24;
  float positionButtonY = panel->getPosition().y - 15;

  auto iter = activeAction.begin();

  for (int i = 0; i < COUNT_BUTTON; i++) {
    auto pannelButton = createButton(
        "PanelButton.png", "PanelButtonPressed.png", "PanelButtonPressed.png",
        iter->first, Color3B::BLACK, "fonts/TimesNewRoman.ttf", 25);
    ui::Widget::ccWidgetTouchCallback callback = CC_CALLBACK_2(
        HelloWorld::makeAction, this, iter->first, std::ref(tagCount));
    pannelButton->addTouchEventListener(callback);
    positionButtonX += pannelButton->getContentSize().width / 2;
    pannelButton->setPosition(Vec2(positionButtonX, positionButtonY));
    positionButtonX += pannelButton->getContentSize().width - 13;
    pannelButton->setOpacity(0);
    buttons.insert({pannelButton, 0});
    ++iter;
  }
}

void HelloWorld::createFigure(Sprite *panel) {
  float position_x = panel->getPosition().x;
  float position_y =
      panel->getPosition().y + panel->getContentSize().height * 1.3;

  std::string figureName = "triangle";
  figure = new Triangle();
  auto triangle = figure->create();
  startPosition_x = position_x;
  startPosition_y = position_y;
  figures.insert(std::pair<Sprite *, std::string>(triangle, figureName));
  nameFigure.push_back(figureName);

  figureName = "ellipse";
  figure = new ellipse();
  auto ellipse = figure->create();
  figures.insert(
      std::pair<cocos2d::Sprite *, std::string>(ellipse, figureName));
  nameFigure.push_back(figureName);

  figureName = "square";
  figure = new Square();
  auto square = figure->create();
  figures.insert(std::pair<cocos2d::Sprite *, std::string>(square, figureName));
  nameFigure.push_back(figureName);

  for (auto iter = figures.begin(); iter != figures.end(); ++iter) {
    iter->first->setPosition(Vec2(position_x, position_y));
    iter->first->setOpacity(0);
    this->addChild(iter->first, 1);
  }
}

void HelloWorld::createAction() {
  std::string actionName = "Rotate";
  act = new Rotate(1.5, 90);
  auto rotate = act->create();
  activeAction.insert(std::pair<std::string, bool>(actionName, 0));

  actionName = "Scale";
  act = new Scale(1.0, 2);
  auto scale = act->create();
  activeAction.insert(std::pair<std::string, bool>(actionName, 0));

  actionName = "Hide";
  act = new Hid(5.0, 0);
  auto hide = act->create();
  activeAction.insert(std::pair<std::string, bool>(actionName, 0));

  actionName = "Translate";
  act = new Translate(0.5, 0, 0);
  auto translate = act->create();
  activeAction.insert(std::pair<std::string, bool>(actionName, 0));
}

void HelloWorld::makeAction(Ref *pSender, ui::Widget::TouchEventType type,
                            std::string nameButton, int &tagCount) {
  bool turnManage = 0;
  if (isBeginTouch(type)) {
    turnManage = 1;
  }
  if (turnManage) {
    float degreeRotate = 0, sc = 0, fade = 0;

    if (activeAction[nameButton] == 0) {
      degreeRotate = 90;
      sc = 2;
      fade = 0;
    } else if (activeAction[nameButton] == 1) {
      degreeRotate = 0;
      sc = 1;
      fade = 255;
    }
    if (nameButton == "Rotate") {
      act = new Rotate(1.5, degreeRotate);
    } else if (nameButton == "Scale") {
      act = new Scale(1.0, sc);
    } else if (nameButton == "Translate" && activeAction[nameButton] == 1) {
      act = new Translate(0.5, startPosition_x, startPosition_y);
    } else if (nameButton == "Hide") {
      act = new Hid(5.0, fade);
      if (activeAction[nameButton] == 0) {
        isInvisible = 1;
      } else {
        isInvisible = 0;
      }
    }
    auto currentAct = act->create();
    currentAct->setTag(tagCount);

    for (auto iter = figures.begin(); iter != figures.end(); ++iter) {
      if (nameButton == "Translate" && activeAction[nameButton] != 1) {
        float height = iter->first->getContentSize().height;
        act = new Translate(0.5, startPosition_x, startPosition_y + height * 2);
        currentAct = act->create();
      }
      if (tagPrevAct != 0) {
        stopActionByTag(tagPrevAct);
      }
      if (!(activeAction[nameButton] == 1 && nameButton == "Hide" &&
            nameFigure[numActiveFigure - 1] != iter->second)) {
        iter->first->runAction(currentAct->clone());
      }
    }
    tagPrevAct = tagCount;
    tagCount++;
    if (activeAction[nameButton] == 1) {
      activeAction[nameButton] = 0;
    } else {
      activeAction[nameButton] = 1;
    }
  }
}

void HelloWorld::ManagePanel(Ref *pSender, ui::Widget::TouchEventType type,
                             cocos2d::Sprite *panel) {
  bool turnManage = 0;
  bool isActiveFigure = 0;
  if (isBeginTouch(type)) {
    turnManage = 1;
  }
  if (turnManage) {
    int newOpacity;
    std::string currentFigure;
    if (isPanelActivated) {
      newOpacity = 0;
      isPanelActivated = 0;
    } else {
      newOpacity = 255;
      isPanelActivated = 1;
    }
    panel->setOpacity(newOpacity);
    for (auto iter = buttons.begin(); iter != buttons.end(); iter++) {
      iter->first->setOpacity(newOpacity);
      iter->second = isPanelActivated;
    }
    if (numActiveFigure == 0) {
      currentFigure = "triangle";
      numActiveFigure = 1;
    } else {
      currentFigure = nameFigure[numActiveFigure - 1];
    }
    for (auto iter = figures.begin(); iter != figures.end(); iter++) {
      if (currentFigure == iter->second) {
        iter->first->setOpacity(newOpacity);
        break;
      }
    }
  }
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event) {
  if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
    changeDesignResolutionSize();
  } else if (keyCode == EventKeyboard::KeyCode::KEY_1) {
    getVisibleFigure(1);
  } else if (keyCode == EventKeyboard::KeyCode::KEY_2) {
    getVisibleFigure(2);
  } else if (keyCode == EventKeyboard::KeyCode::KEY_3) {
    getVisibleFigure(3);
  }
}

void HelloWorld::getVisibleFigure(int num_figure) {
  std::string currentFigure;
  if (num_figure == 1) {
    currentFigure = "triangle";
  } else if (num_figure == 2) {
    currentFigure = "ellipse";
  } else if (num_figure == 3) {
    currentFigure = "square";
  }
  for (auto iter = figures.begin(); iter != figures.end(); iter++) {
    if (currentFigure == iter->second) {
      numActiveFigure = num_figure;
      if (isPanelActivated && isInvisible == 0) {
        iter->first->setOpacity(255);
      }
    } else {
      iter->first->setOpacity(0);
    }
  }
}

bool HelloWorld::isBeginTouch(ui::Widget::TouchEventType type) {
  if (type == ui::Widget::TouchEventType::BEGAN) {
    return true;
  } else {
    return false;
  }
}

void HelloWorld::changeDesignResolutionSize() {
  auto smallResolutionSize = cocos2d::Size(960, 540);
  auto largeResolutionSize = cocos2d::Size(1280, 720);
  cocos2d::Scene *tmp_scene = Scene::create();
  cocos2d::Size CurrentSize = Director::getInstance()->getVisibleSize();
  Director::getInstance()->replaceScene(tmp_scene);
  if (CurrentSize.width == smallResolutionSize.width) {
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(
        largeResolutionSize.width, largeResolutionSize.height,
        ResolutionPolicy::EXACT_FIT);
  } else {
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(
        smallResolutionSize.width, smallResolutionSize.height,
        ResolutionPolicy::EXACT_FIT);
  }
  cocos2d::Scene *scene = HelloWorld::createScene();
  Director::getInstance()->replaceScene(scene);
}
