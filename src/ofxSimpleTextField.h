/*-----------------------------------------------------------/
ofxSimpleTextField.h

github.com/azuremous
Created by Jung un Kim a.k.a azuremous on 10/4/24.
/----------------------------------------------------------*/
#pragma once

#include "ofxSimpleButton.h"

class ofxSimpleTextField {
private:
    ofPtr<ofxSimpleButton> boxButton;
    ofTrueTypeFont font;
    ofRectangle fieldRect;

    bool bActive;
    bool bang;

    int textColor;
    int speed;
    int currentTime;

    float barX;
    ofPoint fontOffset;
    float barOffset;
    float heightOffset;
    float fontHeight;
    
protected:
    void setup(const float& x, const float& y, const float& w, const float& h, const float& offset = 0, const int& speed = 1000) {
        fieldRect.set(x, y, w, h);
        bActive = false;
        heightOffset = offset;
        barX = 0;
        this->speed = speed;
    }

    void setButtonArea(const float& x, const float& y, const float& w, const float& h, const bool& useEvent = true, const bool& manualRender = true) {
        boxButton = ofPtr<ofxSimpleButton>(new ofxSimpleButton);
        boxButton->setup(x, y, w, h, useEvent, manualRender);
    }
    
    void setFont(const ofTrueTypeFontSettings &fontSetting,const float& offsetX = 0, const float& offsetY = 0, const int& color = 0xFFFFFF, const string &textSample = "Q") {
        font.load(fontSetting);
        fontOffset.set(offsetX, offsetY);
        textColor = color;
        fontHeight = font.stringHeight(textSample);
    }
    
    void active() {
        bActive = true;
        bang = false;
        currentTime = ofGetElapsedTimeMillis();
    }

    void deactive() {
        bang = bActive = false;
    }

public:
    
    bool setup(const ofTrueTypeFontSettings &fontSetting, const ofRectangle &fieldRect, const ofRectangle &buttonRect, const float &barOffset = 4, const bool& useEvent = true, const bool& manualRender = true, const float& offsetX = 0, const float& offsetY = 0, const int& color = 0xFFFFFF, const string &textSample = "Q", const float& offset = 0, const int& speed = 1000){
        if(fontSetting.fontName == ""){
            ofLogError("ofxSimpleTextField", "font name is empty");
            return false;
        }
        this->barOffset = barOffset;
        setFont(fontSetting, offsetX, offsetY, color, textSample);
        setup(fieldRect.x, fieldRect.y, fieldRect.width, fieldRect.height, offset, speed);
        setButtonArea(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height, useEvent, manualRender);
        return true;
    }
    
    void setFontOffset(const float &x, const float &y){
        fontOffset.set(x,y);
    }
    
    void setSpeed(const int & spd){
        speed = spd;
    }
    
    void setFontColor(const int &color){
        textColor = color;
    }
    
    void setFontColor(const int &r, const int &g, const int &b){
        textColor = ofColor(r,g,b).getHex();
    }

    void setBarX(const int &n, const string &type) {
        float width = font.stringWidth(type);
        barX += width * n;
    }

    void reset() {
        barX = 0;
    }

    void show() {
        active();
        boxButton->show();
    }

    void hide() {
        deactive();
        boxButton->hide();
    }

    void update() {
        if (bActive) {
            if (ofGetElapsedTimeMillis() - currentTime > speed) {
                bang = !bang;
                currentTime = ofGetElapsedTimeMillis();
            }
        }
    }

    void render(const string& text) {
        float fontWidth = 0;
        if (text != "") { fontWidth = font.stringWidth(text); }
        ofPushMatrix();
        ofTranslate(fieldRect.x, fieldRect.y + (fieldRect.height / 2 - fontHeight / 2) + heightOffset);
        if (bActive) {
            ofPushStyle();
            if (bang) {
                ofSetHexColor(textColor);
                ofDrawRectangle(fontWidth + 2.5 + barX, barOffset, 2, fontHeight);
            }
            ofPopStyle();
        }
        if (text != "") {
            ofPushMatrix();
            ofTranslate(fontOffset);
            ofPushStyle();
            ofSetHexColor(textColor);
            font.drawString(text, 0, fontHeight);
            ofPopStyle();
            ofPopMatrix();
        }
        ofPopMatrix();
    }

    float getX() const { return fieldRect.x; }
    float getY() const { return fieldRect.y; }
    ofRectangle getRect() const { return fieldRect; }
    bool isSelected() const { return boxButton->isSelected(); }
};
