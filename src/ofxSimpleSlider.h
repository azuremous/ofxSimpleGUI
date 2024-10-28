/*-----------------------------------------------------------/
ofxSimpleSlider.h

github.com/azuremous
Created by Jung un Kim a.k.a azuremous on 8/12/14.
/----------------------------------------------------------*/
#pragma once

#include "ofxSimpleButton.h"

class ofxSimpleSlider {
private:
    ofPtr<ofxSimpleButton> sliderButton;

    ofPtr<ofxSimpleButton> plusButton;
    ofPtr<ofxSimpleButton> minusButton;

    ofRectangle sliderRect;
    float maxValue;
    float minValue;
    float scaleValue;
    float startingValue;
    float currentValue;
    float slideValue;
    
    float slideMaxValue;
    float slideMinValue;
    float buttonSize;
    float buttonLimitSize;

    bool useCountButton;
    bool hideValue;
    bool bAppear;
    bool isVertical;
    bool bSelected;
    ofColor s_c;

    string name;
    
protected:
    void setupSlider(const string &name, const float &x, const float &y, const float &w, const float &h, const float &min, const float &max, const float &starting, const bool &manualRender) {
        this->name = name;
        sliderRect.set(x, y, w, h);
        
        slideMinValue = x;
        slideMaxValue = x + w;
        buttonSize = h;
        
        if(w < h){
            isVertical = true;
            slideMinValue = y;
            slideMaxValue = y + h;
            buttonSize = w;
        }
        
        minValue = min;
        maxValue = max;
        
        if (starting < minValue || starting > maxValue) {
            startingValue = (maxValue - minValue) / 2.0;
        }
        else {
            startingValue = starting;
        }
        
        slideValue = ((maxValue - minValue) / scaleValue) * slideMaxValue;
        
        currentValue = ofMap(startingValue, minValue, maxValue, slideMinValue, slideMaxValue - buttonSize);

        if (ofGetTargetPlatform() == OF_TARGET_IOS) {
            ofAddListener(ofEvents().touchDown, this, &ofxSimpleSlider::touchDown);
            ofAddListener(ofEvents().touchMoved, this, &ofxSimpleSlider::touchMoved);
            ofAddListener(ofEvents().touchDoubleTap, this, &ofxSimpleSlider::touchDoubleTap);
            ofAddListener(ofEvents().touchUp, this, &ofxSimpleSlider::touchUp);
        }
        else {
            ofAddListener(ofEvents().mousePressed, this, &ofxSimpleSlider::mousePressed);
            ofAddListener(ofEvents().mouseReleased, this, &ofxSimpleSlider::mouseReleased);
            ofAddListener(ofEvents().mouseMoved, this, &ofxSimpleSlider::mouseMoved);
            ofAddListener(ofEvents().mouseDragged, this, &ofxSimpleSlider::mouseDragged);
        }
    }
    
    void updateSliderButtonPos(){
        if(isVertical){
            float val = currentValue - sliderButton->getY();
            sliderButton->update(0, val);
        }else{
            float val = currentValue - sliderButton->getX();
            sliderButton->update(val, 0);
        }
    }

public:
    explicit ofxSimpleSlider() :maxValue(0.0), minValue(1.0), scaleValue(100.0), startingValue(0.5), useCountButton(false), hideValue(false), bAppear(false), isVertical(false), bSelected(false), s_c(ofColor::white) {}
    
    ~ofxSimpleSlider() {}
    
    void setup(const string &name, const ofRectangle &rect, const float &min, const float &max, const float &starting, const bool &manualRender, const float &buttonWidth = 0, const float &buttonHeight = 0){
        setup(name, rect.x, rect.y, rect.width, rect.height, min, max, starting, manualRender, buttonWidth, buttonHeight);
    }
    
    void setup(const string &name, const float &x, const float &y, const float &w, const float &h, const float &min, const float &max, const float &starting, const bool &manualRender, const float &buttonWidth = 0, const float &buttonHeight = 0){
        setupSlider(name, x, y, w, h, min, max, starting, manualRender);
        
        float width = buttonSize;
        float height = buttonSize;
        buttonLimitSize = buttonSize;
        if(buttonWidth != 0){
            width = buttonWidth;
            if(!isVertical){
                buttonLimitSize = width;
            }
        }
        if(buttonHeight != 0){
            height = buttonHeight;
            if(isVertical){
                buttonLimitSize = height;
            }
        }
        sliderButton = ofPtr<ofxSimpleButton>(new ofxSimpleButton);
        sliderButton->setup(x, y, width, height, true, manualRender);
        
        if(isVertical){
            sliderButton->setPos(sliderButton->getX(), currentValue);
        }else{
            sliderButton->setPos(currentValue, sliderButton->getY());
        }
        
        sliderButton->hide();
        
        if (!manualRender) {
            ofAddListener(ofEvents().draw, this, &ofxSimpleSlider::render);
            sliderButton->show();
            bAppear = true;
        }
    }

    void show() {
        bAppear = true;
        sliderButton->show();
    }

    void hide() {
        bAppear = false;
        sliderButton->hide();
    }
    
    void setScaleValue(const float &val) {
        scaleValue = val;
        slideValue = ((maxValue - minValue) / scaleValue) * slideMaxValue;
    }

    void setSlideValueWithDivide(const float &val) { slideValue = (maxValue - minValue) / val; }
    
    void setSlideValue(const float &val) { slideValue = val; }
    
    void reset() {
        currentValue = ofMap(startingValue, minValue, maxValue, slideMinValue, slideMaxValue - buttonSize);
        sliderButton->resetMove();
    }

    void setColor(const ofColor& c) {
        s_c = c;
        sliderButton->setColor(s_c);
    }

    void showButtons() {
        if (!useCountButton) {
            useCountButton = true;
            minusButton = ofPtr<ofxSimpleButton>(new ofxSimpleButton);
            plusButton = ofPtr<ofxSimpleButton>(new ofxSimpleButton);
            
            if(isVertical){
                minusButton->setup(sliderRect.x, sliderRect.y - (buttonSize + 10), buttonSize, buttonSize, false, true);
                plusButton->setup(sliderRect.x, sliderRect.y + sliderRect.getHeight() + 10, buttonSize, buttonSize, false, true);
            }else{
                minusButton->setup(sliderRect.x - (buttonSize + 10), sliderRect.y, buttonSize, buttonSize, false, true);
                plusButton->setup(sliderRect.x + sliderRect.getWidth() + 10, sliderRect.y, buttonSize, buttonSize, false, true);
            }
            minusButton->setName("-", buttonSize / 2 - 3, buttonSize/2 + 3);
            plusButton->setName("+", buttonSize / 2 - 3, buttonSize/2 + 3);
            
        }
        plusButton->show();
        minusButton->show();

        plusButton->setColor(s_c);
        minusButton->setColor(s_c);
    }

    void update() { }

    void hideButtons() {
        plusButton->hide();
        minusButton->hide();
    }

    void mouseDragged(int x, int y) {
        setButtonPos(x, y);
    }

    void mousePressed(int x, int y) {
        setButtonPos(x, y);

        if (useCountButton) {
            minusButton->mousePressed(x, y);
            plusButton->mousePressed(x, y);

            if (minusButton->isSelected()) {
                currentValue -= slideValue;
                if (currentValue <= slideMinValue) { currentValue = slideMinValue; }
                updateSliderButtonPos();
            }
            else if (plusButton->isSelected()) {
                currentValue += slideValue;
                if (currentValue >= slideMaxValue - buttonLimitSize) { currentValue = slideMaxValue - buttonLimitSize;  }
                updateSliderButtonPos();
            }
        }
    }

    void mouseReleased(int x, int y) {
        if (useCountButton) {
            minusButton->mouseReleased();
            plusButton->mouseReleased();
        }
        bSelected = false;
    }

    void mouseMoved(int x, int y) {

    }

    void setButtonPos(int x, int y) {
        if (y >= sliderRect.y && y <= sliderRect.y + sliderRect.getHeight() && x >= sliderRect.x && x <= sliderRect.x + sliderRect.getWidth()) {
            if(isVertical){
                currentValue = y - buttonSize / 2;
                currentValue = ofClamp(currentValue, slideMinValue, slideMaxValue - buttonLimitSize);
                float val = currentValue - sliderButton->getY();
                sliderButton->update(0, val);
            }else{
                currentValue = x - buttonSize / 2;
                currentValue = ofClamp(currentValue, slideMinValue, slideMaxValue - buttonLimitSize);
                float val = currentValue - sliderButton->getX();
                sliderButton->update(val, 0);
            }
            bSelected = true;
        }
    }

    void render(ofEventArgs& event) { render(); }
    void mousePressed(ofMouseEventArgs& mouse) { mousePressed(mouse.x, mouse.y); }
    void mouseDragged(ofMouseEventArgs& mouse) { mouseDragged(mouse.x, mouse.y); }
    void mouseReleased(ofMouseEventArgs& mouse) { mouseReleased(mouse.x, mouse.y); }
    void mouseMoved(ofMouseEventArgs& mouse) { mouseMoved(mouse.x, mouse.y); }

    void touchDown(ofTouchEventArgs& touch) { mousePressed(touch.x, touch.y); }
    void touchUp(ofTouchEventArgs& touch) { mouseReleased(touch.x, touch.y);  }
    void touchMoved(ofTouchEventArgs& touch) { mouseDragged(touch.x, touch.y);  }
    void touchDoubleTap(ofTouchEventArgs& touch) { }

    void render() {
        if (bAppear) {
            sliderButton->render();

            ofPushStyle();
            ofNoFill();
            ofSetColor(s_c);
            ofDrawRectangle(sliderRect);
            ofPopStyle();

            if (useCountButton) {
                plusButton->render();
                minusButton->render();
            }
            if (!hideValue) {
                ofPushStyle();
                ofSetColor(s_c);
                ofDrawBitmapString(name +" val:" + ofToString(currentValue)+ " , " + ofToString(getValue()) + "%", sliderRect.x, sliderRect.y - 2);
                ofPopStyle();
            }
        }
    }

    float getButtonX() const { return sliderButton->getCurrentX(); }
    float getButtonY() const { return sliderButton->getCurrentY(); }
    float getValue() const { return ofMap(currentValue, slideMinValue, slideMaxValue - buttonLimitSize, minValue, maxValue); }
    bool isMoved() const { return currentValue != ofMap(startingValue, minValue, maxValue, slideMinValue, slideMaxValue - buttonLimitSize); }
    bool isSelected() const { return bSelected; }
};
