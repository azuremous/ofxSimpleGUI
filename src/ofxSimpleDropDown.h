/*-----------------------------------------------------------/
ofxSimpleDropDown.h

github.com/azuremous
Created by Jung un Kim a.k.a azuremous on 10/4/24.
/----------------------------------------------------------*/
#pragma once

#include "ofxSimpleButton.h"

struct ofxSimpleDropDownSetting {
    ofRectangle dropButtonRect;
    ofRectangle menuButtonRect;
    ofRectangle doneButtonRect;
    ofRectangle scrollBarRect;
    
    string title = "";
    string fontName = "";
    string doneButtonImageName = "";
    
    int fontSize = 1;
    
    int textColor = 0;
    int placeHolderColor = 0xFFFFFF;
    int highlightColor = 0xFFFFFF;
    int menuBackColor = 0xFFFFFF;
    
    int maxSize = 5;
    
    float titleOffset = 0;
    float menuHeight = 0;
    
    float menuTitleMargin = 0;
    float itemTextMargin = 0;
    
    bool hideMenuTitle = false;
};

class ofxSimpleDropDown {
private:
    vector<ofPtr<ofxSimpleButton> > list_buttons;
    ofPtr<ofxSimpleSlider> scrollBarSlider;

    ofxSimpleButton doneButton;
    ofxSimpleButton dropButton;
    ofTrueTypeFont listFont;

    ofRectangle rect;
    ofRectangle scrollBarRect;
    ofFbo frameFbo;

    ofTexture buttonTexture;
    
    ofPoint lastTouchPoint;

    string title;
    string menuTitle;

    int selectedID;
    int textColor;
    int placeHolderColor;
    int highlightColor;
    int menuBackColor;

    float fontHeight;
    float itemHeight;
    float titleOffset;
    float scrolledPos;
    float menuHeight;

    float scrollPosition;
    float scrollBarHeight;
    
    float menuTitleHeight;
    float menuTitleMargin;
    float itemTextMargin;
    
    float scrollVal;
    float lastScrollVal;

    bool isScrolling;
    bool bShowDrop;
    bool useScrollBar;
    bool useDoneButton;
    bool hideMenuTitle;
    
protected:
    void setupList(const vector<string>& list, const ofRectangle& menuButtonRect, const int &maxSize){
        rect = menuButtonRect;
        if(!hideMenuTitle){
            menuTitleHeight = rect.height;
        }
        int offset = 1;
        if(hideMenuTitle){ offset = 0; }
        for (int i = 0; i < list.size(); i++) {
            ofPtr<ofxSimpleButton> btn = ofPtr<ofxSimpleButton>(new ofxSimpleButton);
            btn->setup(rect.x, rect.y + (i + offset) * rect.height, rect.width, rect.height, true, true);
            btn->setName(list[i]);
            list_buttons.push_back(btn);
        }
        itemHeight = list_buttons[0]->getHeight();
        
        int fboSize = list.size() > maxSize ? maxSize : list.size();
        float fboHeight = rect.height * fboSize;
        while(menuHeight < fboHeight){
            fboSize--;
            fboHeight = rect.height * fboSize;
        }
        useScrollBar = list.size() * rect.height > menuHeight ? true : false;
        
        frameFbo.allocate(rect.width, menuHeight);
        
        if(useScrollBar){
            scrollPosition = 0;
            scrollBarHeight = (menuHeight / (list_buttons.size() * itemHeight)) * menuHeight;
        }
    }
    
    void setFont(const string& s, int size) {
        ofTrueTypeFontSettings fontSetting = ofTrueTypeFontSettings(ofToDataPath(s, true), size);
        fontSetting.addRanges(ofAlphabet::Latin);
        fontSetting.addRanges(ofAlphabet::Japanese);
        fontSetting.addRange(ofUnicode::KatakanaHalfAndFullwidthForms);

        listFont.load(fontSetting);

        fontHeight = listFont.stringHeight(title);
    }

    void setTitle(const string& s, const float& offset) {
        menuTitle = title = s;
        titleOffset = offset;
    }
    
    void showMenu() {
        for (int i = 0; i < list_buttons.size(); i++) {
            list_buttons[i]->resetMove();
            list_buttons[i]->show();
        }
        if(useDoneButton){
            doneButton.show();
        }
        dropButton.hide();
        scrollPosition = 0;
    }

    void hideMenu() {
        bShowDrop = false;
        for (int i = 0; i < list_buttons.size(); i++) {
            list_buttons[i]->hide();
        }
        if(useDoneButton){
            doneButton.hide();
        }
        dropButton.show();
        scrollPosition = 0;
        if(useScrollBar){
            scrollBarSlider->reset();
        }
    }
    
    void renderDoneButtonImg() {
        if (buttonTexture.isAllocated() && useDoneButton) {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(255);
            buttonTexture.draw(doneButton.getX(), doneButton.getY());
            ofPopStyle();
        }
    }

    void renderDropMenu() {
        //frameFbo for drop menu
        frameFbo.begin();
        ofPushStyle();
        ofSetHexColor(menuBackColor);
        ofDrawRectangle(0, 0, frameFbo.getWidth(), frameFbo.getHeight());
        ofPopStyle();

        ofPushMatrix();
        ofTranslate(0, scrolledPos);
        renderMenuText();
        ofPopMatrix();

        frameFbo.end();
        float offset = itemHeight;
        if(hideMenuTitle){
            offset = 0;
        }
        frameFbo.draw(rect.x, rect.y + offset);
    }

    void renderMenuText() {
        ofPushStyle();
        
        float fontOffset = itemHeight / 2 - fontHeight / 2;
        for (int i = 0; i < list_buttons.size(); i++) {
            if (i == selectedID) {
                ofSetHexColor(highlightColor);
                ofDrawRectangle(0, list_buttons[i]->getHeight() * i, frameFbo.getWidth(), itemHeight);
            }

            ofSetHexColor(textColor);
            listFont.drawString(list_buttons[i]->getName(), list_buttons[i]->getX() + itemTextMargin, fontHeight + fontOffset + list_buttons[i]->getHeight() * i);
        }

        for (int i = 0; i < list_buttons.size()+1; i++) {
            ofPushStyle();
            ofSetLineWidth(2);
            ofSetHexColor(placeHolderColor);
            ofDrawLine(0, itemHeight * i, frameFbo.getWidth(), itemHeight * i);
            ofPopStyle();
        }

        ofPopStyle();
    }

    void renderscrollBar() {
        if (useScrollBar) {
            ofPushMatrix();
            ofTranslate(0, list_buttons[0]->getY());
            ofPushStyle();
            ofSetHexColor(placeHolderColor);
            ofDrawRectangle(scrollBarRect);
            ofSetHexColor(textColor);
            ofDrawRectangle(scrollBarRect.x, scrollBarRect.y + scrollPosition, scrollBarRect.width, scrollBarHeight);
            ofPopStyle();
            ofPopMatrix();
        }
    }

    void renderDropButtonTitle() {
        ofPushStyle();
        ofSetHexColor(textColor);
        float fontOffset = dropButton.getHeight() / 2 - fontHeight / 2;
        listFont.drawString(title, titleOffset + dropButton.getX(), dropButton.getY() + fontHeight + fontOffset);
        ofPopStyle();
    }
    
    void renderMenuTitle(){
        if(!hideMenuTitle){
            ofPushStyle();
            ofSetHexColor(placeHolderColor);
            ofDrawRectangle(rect);
            ofSetHexColor(textColor);
            float fontOffset = rect.height / 2 - fontHeight / 2;
            listFont.drawString(menuTitle, rect.x + menuTitleMargin, rect.y + fontHeight + fontOffset);
            ofPopStyle();
        }
    }
    
    bool checkClickIsInside(const float &x, const float &y){
        return y > rect.y && y < rect.y + menuHeight + menuTitleHeight && x > rect.x && x < rect.x + rect.width;
    }

public:
    ofxSimpleDropDown():bShowDrop(false), useScrollBar(false),useDoneButton(false), selectedID(-1)
    {}
    
    bool setup(const ofxSimpleDropDownSetting& setting, const vector<string>& list) {
        lastTouchPoint.set(0, 0);
        
        menuBackColor = setting.menuBackColor;
        highlightColor = setting.highlightColor;
        textColor = setting.textColor;
        placeHolderColor = setting.placeHolderColor;
        
        menuHeight = setting.menuHeight;
        if(menuHeight == 0){
            ofLogError("ofxSimpleDropDown", "menuHeight is zero, menuHeight must be higher than height of item");
            return false;
        }
        menuTitleHeight = 0;
        menuTitleMargin = setting.itemTextMargin;
        itemTextMargin = setting.itemTextMargin;
        
        if(setting.doneButtonRect.width * setting.doneButtonRect.height != 0){
            useDoneButton = true;
        }
        
        hideMenuTitle = setting.hideMenuTitle;
        
        setupList(list, setting.menuButtonRect, setting.maxSize);
        dropButton.setup(setting.dropButtonRect.x, setting.dropButtonRect.y, setting.dropButtonRect.width, setting.dropButtonRect.height, true, true);
        
        if(useScrollBar){
            scrollBarRect = setting.scrollBarRect;
            scrollBarSlider = ofPtr<ofxSimpleSlider>(new ofxSimpleSlider);
           
            scrollBarSlider->setup("scrollBar", scrollBarRect.x, scrollBarRect.y + list_buttons[0]->getY(), scrollBarRect.width, scrollBarRect.height, 0, 1, 0, true, 0, scrollBarHeight);
            scrollBarSlider->show();
        }
        
        if(useDoneButton){
            doneButton.setup(setting.doneButtonRect.x, setting.doneButtonRect.y, setting.doneButtonRect.width, setting.doneButtonRect.height, true, true);
        }
        
        setTitle(setting.title, setting.titleOffset);
        
        if(setting.fontName == ""){
            ofLogError("ofxSimpleDropDown", "font name is empty");
            return false;
        }
        setFont(setting.fontName, setting.fontSize);
        
        if(setting.doneButtonImageName != ""){
            ofImage img;
            img.load(setting.doneButtonImageName);
            buttonTexture = img.getTexture();
        }
        
        hide();
        return true;
    }

    void reset() {
        title = menuTitle;
        selectedID = -1;
        bShowDrop = false;
        hide();
    }

    void show() {
        dropButton.show();
    }
    
    void hide() {
        dropButton.hide();
        hideMenu();
    }

    void update() {
        if (useScrollBar) {
            float pos = lastScrollVal;
            float maxScrollPos = list_buttons.size() * itemHeight;
            float currentScrollPos = ofMap(pos, 0, maxScrollPos, 0, scrollBarRect.height - scrollBarHeight);
            
            float maxDistance = itemHeight * 2 + list_buttons[0]->getY();
            
            if(scrollBarSlider->isSelected()){
                pos = scrollBarSlider->getValue();
                currentScrollPos = ofMap(pos, 0, 1, 0, scrollBarRect.height - scrollBarHeight);
                lastScrollVal = ofMap(pos, 0, 1, 0, maxScrollPos);
            }
            
            scrollPosition = currentScrollPos;
            if (list_buttons[list_buttons.size() - 1]->getCurrentY() >= maxDistance) {
                scrolledPos = -lastScrollVal;
                float newPos = list_buttons[list_buttons.size() - 1]->getY() + scrolledPos;
                float distance = newPos - maxDistance;
                if (distance < 0) { scrolledPos -= distance; }
                else {
                    scrollPosition = currentScrollPos;
                }
            }
            
            for (int i = 0; i < list_buttons.size(); i++) {
                list_buttons[i]->update(0, scrolledPos);
            }
        }
    }
    
    void checkPressed(const float &x, const float &y){
        isScrolling = true;
        scrollVal = 0;
        lastTouchPoint.set(x, y);
    }
    
    void checkDrag(const float &x, const float &y){
        if (isScrolling) {
            scrollVal = (y - lastTouchPoint.y);
            lastTouchPoint.y = y;

            lastScrollVal += scrollVal;
            if (lastScrollVal <= 0) {
                lastScrollVal = 0;
            }
        }
    }

    bool checkDropDown(const float &x, const float &y) {
        isScrolling = false;
        
        if (bShowDrop) {
            if(useDoneButton){
                if (doneButton.isSelected()) {
                    if (selectedID >= 0 && selectedID < list_buttons.size()) {
                        title = list_buttons[selectedID]->getName();
                    }
                    hideMenu();
                    return true;
                }
            }else{
                if(!checkClickIsInside(x, y)){ //check inside
                    hideMenu();
                    return true;
                }
            }
            int n = -1;
            float offset = list_buttons[0]->getY();
            if(hideMenuTitle){
                offset = 0;
            }
            for (int i = 0; i < list_buttons.size(); i++) {
                if (list_buttons[i]->getCurrentY() >= offset - itemHeight * 0.35 && list_buttons[i]->getCurrentY() < rect.y + menuHeight) {
                    if (list_buttons[i]->isSelected()) {
                        n = i;
                    }
                }
            }
            if (n != -1) {
                selectedID = n;
                if(!useDoneButton) {
                    title = list_buttons[selectedID]->getName();
                    //hideMenu();
                    return true;
                }
            }
        }
        else {
            if (dropButton.isSelected()) {
                bShowDrop = !bShowDrop;
                if (bShowDrop) {
                    showMenu();
                }
                else {
                    hideMenu();
                }
            }
        }
        return false;
    }

    void render() {
        renderDropButtonTitle();
        
        if (bShowDrop) {
            //draw menu back rectangle
            ofPushStyle();
            ofSetHexColor(menuBackColor);
            ofDrawRectangle(rect.x, rect.y, rect.width, menuHeight);
            ofPopStyle();
            
            renderMenuTitle();
            renderDropMenu();
            renderDoneButtonImg();
            renderscrollBar();
        }
    }

    string getTitle() const { return title; }
    int getSelectedID() const { return selectedID; }
    float getBoxY() const { return dropButton.getCurrentY(); }
    bool isShowDrop() const { return bShowDrop; }
    bool isSelectedScrollBar() const { return scrollBarSlider->isSelected(); }
};
