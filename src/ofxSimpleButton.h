/*-----------------------------------------------------------/
ofxSimpleButton.h

github.com/azuremous
Created by Jung un Kim a.k.a azuremous on 4/15/12.
/----------------------------------------------------------*/
#pragma once

#include "ofMain.h"

typedef enum {
    TYPE_BUTTON = 0,
    TYPE_TOGGLE = 1,
    
}BUTTON_TYPE;

typedef enum {
    BUTTON_RECT = 1,
    BUTTON_CIRCLE = 2,
    BUTTON_IMAGE = 3
}BUTTON_SHAPES;

//--------------------------------------------------------------
class ofxSimpleButton {
private:
    ofRectangle rect;
    ofPoint namePos;
    ofPoint valuePos;
    ofPoint movedPos;
    
    ofColor color;
    ofColor toggledColor;
    
    vector<ofTexture> buttonTexture;
    
    BUTTON_TYPE buttonType;
    BUTTON_SHAPES buttonShape;
    
    string buttonName;
    string valText;
    
    bool bAppear;
    bool bRender;
    bool fixPos;
    
    bool bSelect;
    bool bToggle;
    bool bDoubleTab;
    bool bAnimation;
    
    bool bang;
    bool startAni;
    
    int duration;
    int currentTime;
    
    int alpha;
    
protected:
    
    void resetAniTime(bool start);
    
    void update(ofEventArgs &event);
    void render(ofEventArgs &event);
    
    void touchDown(ofTouchEventArgs &touch);
    void touchUp(ofTouchEventArgs &touch);
    void touchMoved(ofTouchEventArgs &touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    
    void mousePressed(ofMouseEventArgs &mouse);
    void mouseReleased(ofMouseEventArgs &mouse);
    void mouseMoved(ofMouseEventArgs &mouse);
    void mouseDragged(ofMouseEventArgs &mouse);
    
    void buttonAction(bool b);
    bool isInside(float x, float y);
    
    string checkExtension(const string &route, const string &add = "");

public:
    explicit ofxSimpleButton();
    virtual~ofxSimpleButton();
    
    bool setup(const float& x, const float& y, const float &w = 50, const float &h = 50, const bool &useEvent = true);
    bool setup(const ofRectangle &rect, const bool &useEvent = true, const bool &manualRender = false, const BUTTON_TYPE &type = TYPE_BUTTON, const BUTTON_SHAPES &shape = BUTTON_RECT, const ofColor &c = ofColor::white);
    bool setup(const float &x, const float &y, const float &w, const float &h, const bool &useEvent, const bool &manualRender, const BUTTON_TYPE &type = TYPE_BUTTON, const BUTTON_SHAPES &shape = BUTTON_RECT, const ofColor &c = ofColor::white);
    bool setup(const float &x, const float &y, const string &imgRoute, const bool &useAnimation = false, const bool &useEvent = true, const bool &manualRender = false, const BUTTON_TYPE &type = TYPE_BUTTON);
    
    void setAnimationTime(const int &time = 500);
    void setType(const BUTTON_TYPE &type);
    void setShape(const BUTTON_SHAPES &shape);
    void setColor(const ofColor &c);
    void setToggleColor(const ofColor &c);
    void setAlpha(const int &a);
    void setAlpha(const float &a);
    void setName(string name, float x = 0, float y = -10);
    void setValue(string val, float x = 0, float y = -10);
    void setFixPos(bool fix);
    void setPos(const ofPoint & p);
    void setPos(const float &x, const float &y);
    void update(const float& x, const float& y);
    
    void resetToggle();
    void toggleShow();
    void hide();
    void show();
    void resetMove();

    void render();
    
    void touchDown(float x, float y);
    void touchUp();
    void touchMoved(float x, float y);
    void touchDoubleTap(float x, float y);
    
    void mousePressed(float x, float y);
    void mouseReleased();
    void mouseMoved(float x, float y);
    void mouseDragged(float x, float y);
    
    bool isRender() const { return bRender; }
    bool isSelected() const { return bSelect; }
    bool isToggled() const { return bToggle; }
    bool isDoubleTabed() const { return bDoubleTab; }
    
    float getX() const { return rect.x; }
    float getY() const { return rect.y; }
    float getCurrentX() const { return ( rect.x + movedPos.x); }
    float getCurrentY() const { return ( rect.y + movedPos.y); }
    float getWidth() const { return rect.width; }
    float getHeight() const {return rect.height; }
    ofRectangle getRect() const { return rect; }

    string getName() const { return buttonName; }

    ofEvent<void> mousePressedEvent;
};

