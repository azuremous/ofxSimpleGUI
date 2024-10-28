#include "ofxSimpleButton.h"

//--------------------------------------------------------------
/*public */ofxSimpleButton::ofxSimpleButton()
:bAppear(false)
,bRender(false)
,fixPos(true)
,bSelect(false)
,bToggle(false)
,bDoubleTab(false)
,bAnimation(false)
,bang(false)
,startAni(false)
,currentTime(0)
,duration(500)
,alpha(255)
,buttonName("")
,valText("")
,color(ofColor::white)
,toggledColor(ofColor::yellow)
{}

//--------------------------------------------------------------
/*public */ofxSimpleButton::~ofxSimpleButton(){}

//--------------------------------------------------------------
/*public */bool ofxSimpleButton::setup(const float& x, const float& y, const float &w, const float &h, const bool &useEvent){
    return setup(x, y, w, h, useEvent, false);
}

//--------------------------------------------------------------
/*public */bool ofxSimpleButton::setup(const ofRectangle &rect, const bool &useEvent , const bool &manualRender, const BUTTON_TYPE &type, const BUTTON_SHAPES &shape , const ofColor &c){
    return setup(rect.x, rect.y, rect.width, rect.height, useEvent, manualRender, type, shape, c);
}

//--------------------------------------------------------------
/*public */bool ofxSimpleButton::setup(const float& x, const float& y, const float& w, const float& h, const bool& useEvent, const bool &manualRender, const BUTTON_TYPE &type, const BUTTON_SHAPES &shape, const ofColor &c){
    rect.set(x, y, w, h);
    movedPos.set(0, 0);
    
    if(w <= 0 || h <= 0){
        ofLogError("ofxSimpleButton", "Fail to setup: rect size is zero");
        return bAppear;
    }
    
    if (useEvent) {
        if (ofGetTargetPlatform() == OF_TARGET_IOS || ofGetTargetPlatform() == OF_TARGET_ANDROID) {
            ofAddListener(ofEvents().touchDown, this, &ofxSimpleButton::touchDown);
            ofAddListener(ofEvents().touchMoved, this, &ofxSimpleButton::touchMoved);
            ofAddListener(ofEvents().touchDoubleTap, this, &ofxSimpleButton::touchDoubleTap);
            ofAddListener(ofEvents().touchUp, this, &ofxSimpleButton::touchUp);
        }else{
            ofAddListener(ofEvents().mousePressed , this, &ofxSimpleButton::mousePressed);
            ofAddListener(ofEvents().mouseReleased, this, &ofxSimpleButton::mouseReleased);
            ofAddListener(ofEvents().mouseMoved, this, &ofxSimpleButton::mouseMoved);
            ofAddListener(ofEvents().mouseDragged, this, &ofxSimpleButton::mouseDragged);
        }
    }
    
    if (!manualRender) {
        ofAddListener(ofEvents().draw, this, &ofxSimpleButton::render);
    }
    
    setType(type);
    setShape(shape);
    color = c;
    
    bAppear = bRender = true;
    return bAppear;
}

//--------------------------------------------------------------
/*public */bool ofxSimpleButton::setup(const float &x, const float &y, const string &imgRoute, const bool &useAnimation, const bool &useEvent, const bool &manualRender, const BUTTON_TYPE &type){
    movedPos.set(0, 0);
    
    ofImage img;
    
    if(!img.load(checkExtension(imgRoute))) {
        ofLogError("ofxSimpleButton", "Fail to load image");
        return false;
        
    }
    
    buttonTexture.push_back(img.getTexture());
    
    if(useAnimation){
        if(!img.load(checkExtension(imgRoute, "_"))){
            ofLogError("ofxSimpleButton", "Fail to load image for animation");
            return false;
        }
        buttonTexture.push_back(img.getTexture());
        bAnimation = true;
        ofAddListener(ofEvents().update, this, &ofxSimpleButton::update);
    }else{//check toggle
        if(img.load(checkExtension(imgRoute, "_"))){
            buttonTexture.push_back(img.getTexture());//for toggle
        }
    }
    
    if (buttonTexture.size() > 0) {
        float w = buttonTexture[0].getWidth();
        float h = buttonTexture[0].getHeight();
        if(w <= 0 || h <= 0){
            ofLogError("ofxSimpleButton", "Fail to setup: image rect size is zero");
            return bAppear;
        }
        
        rect.set(x, y, w, h);
        
        if (useEvent) {
            if (ofGetTargetPlatform() == OF_TARGET_IOS || ofGetTargetPlatform() == OF_TARGET_ANDROID) {
                ofAddListener(ofEvents().touchDown, this, &ofxSimpleButton::touchDown);
                ofAddListener(ofEvents().touchMoved, this, &ofxSimpleButton::touchMoved);
                ofAddListener(ofEvents().touchDoubleTap, this, &ofxSimpleButton::touchDoubleTap);
                ofAddListener(ofEvents().touchUp, this, &ofxSimpleButton::touchUp);
            }else{
                ofAddListener(ofEvents().mousePressed , this, &ofxSimpleButton::mousePressed);
                ofAddListener(ofEvents().mouseReleased, this, &ofxSimpleButton::mouseReleased);
                ofAddListener(ofEvents().mouseMoved, this, &ofxSimpleButton::mouseMoved);
                ofAddListener(ofEvents().mouseDragged, this, &ofxSimpleButton::mouseDragged);
            }
        }
        
        if (!manualRender) {
            ofAddListener(ofEvents().draw, this, &ofxSimpleButton::render);
        }
    }
    
    setType(type);
    setShape(BUTTON_IMAGE);
    
    bAppear = bRender = true;
    currentTime = ofGetElapsedTimeMillis();
    return bAppear;
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::setAnimationTime(const int &time){
    currentTime = ofGetElapsedTimeMillis();
    duration = time;
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::setType(const BUTTON_TYPE &type){
    buttonType = type;
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::setShape(const BUTTON_SHAPES &shape){
    buttonShape = shape;
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::setColor(const ofColor &c){
    color = c;
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::setToggleColor(const ofColor &c){
    toggledColor = c;
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::setAlpha(const int &a){
    alpha = a;
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::setAlpha(const float &a){
    int result = (int)ofMap(a, 0.0, 1.0, 0., 255.);
    setAlpha(result);
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::setName(string name, float x, float y){
    if (!bAppear) {
        ofLogError("ofxSimpleButton", "Button is not initialized. First you need to setup.");
        return;
    }
    buttonName = name;
    namePos.set(x, y);
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::setValue(string val, float x, float y){
    if (!bAppear) {
        ofLogError("ofxSimpleButton", "Button is not initialized. First you need to setup.");
        return;
    }
    valText = val;
    valuePos.set(x, y);
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::setFixPos(bool fix){
    fixPos = fix;
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::setPos(const ofPoint &p){
    setPos(p.x, p.y);
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::setPos(const float &x, const float &y){
    rect.x = x;
    rect.y = y;
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::update(const float& x, const float& y){
    if (x != 0) {
        movedPos.x = x;
    }
    if (y != 0) {
        movedPos.y = y;
    }
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::resetToggle(){
    bToggle = false;
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::toggleShow(){
    bAppear = !bAppear;
    bRender = !bRender;
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::hide(){
    bAppear = bRender = false;
    if (bAnimation) { resetAniTime(bAppear); }
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::show(){
    bAppear = bRender = true;
    if (bAnimation) { resetAniTime(bAppear); }
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::resetMove(){
    movedPos.set(0, 0);
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::render(){
    if (bRender) {
        ofPushMatrix();
        ofTranslate(rect.x + movedPos.x, rect.y + movedPos.y);
        
        if (buttonTexture.size() > 0 && buttonShape == BUTTON_IMAGE) {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(255, alpha);
            
            if((bAnimation && bang) || bToggle){
                buttonTexture[1].draw(0, 0);
            }else{ buttonTexture[0].draw(0, 0); }
            
            ofDisableAlphaBlending();
            ofPopStyle();
        }else { //shape
            ofPushStyle();
            ofNoFill();
            ofSetLineWidth(2);
            ofColor col = bToggle ? toggledColor : color;
            ofSetColor(col, alpha);
            if (buttonShape == BUTTON_CIRCLE) {
                ofDrawEllipse(rect.width/2, rect.height/2, rect.width, rect.height);
            }else if (buttonShape == BUTTON_RECT){
                ofDrawRectangle(0, 0, rect.width, rect.height);
            }
            ofPopStyle();
        }
        
        if (buttonName != "") {
            ofPushStyle();
            ofSetColor(color, alpha);
            ofDrawBitmapString(buttonName, namePos);
            ofPopStyle();
        }
        
        if (valText != "") {
            ofPushStyle();
            ofSetColor(color, alpha);
            ofDrawBitmapString(valText, valuePos);
            ofPopStyle();
        }
        ofPopMatrix();
    }
    
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::touchDown(float x, float y){
    if (isInside(x, y)) {
        buttonAction(true);
    }
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::touchUp(){
    if (buttonType == TYPE_BUTTON) {
        bSelect = bToggle = false;
    }else if (buttonType == TYPE_TOGGLE) {
        bSelect = false;
    }
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::touchMoved(float x, float y){
    if (!fixPos && bSelect) {
        setPos(x - rect.width/2, y - rect.height/2);
    }
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::touchDoubleTap(float x, float y){
    if (isInside(x, y)) { bDoubleTab = !bDoubleTab; }
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::mousePressed(float x, float y){
    if (isInside(x, y)) {
        ofNotifyEvent(mousePressedEvent, this);
        buttonAction(true);
    }
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::mouseReleased(){
    if (buttonType == TYPE_BUTTON) { bSelect = bToggle = false; }
    else if (buttonType == TYPE_TOGGLE) { bSelect = false; }
    
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::mouseMoved(float x, float y){
    if (!fixPos && bSelect) {
        setPos(x, y);
    }
}

//--------------------------------------------------------------
/*public */void ofxSimpleButton::mouseDragged(float x, float y){
    if (!fixPos && bSelect) {
        setPos(x, y);
    }
}

//--------------------------------------------------------------
/*protected */void ofxSimpleButton::resetAniTime(bool start){
    startAni = start;
    bang = false;
    currentTime = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
/*protected */void ofxSimpleButton::update(ofEventArgs &event){
    if (startAni) {
        if (ofGetElapsedTimeMillis() - currentTime >= duration) {
            bang =! bang;
            currentTime = ofGetElapsedTimeMillis();
        }
    }
}

//--------------------------------------------------------------
/*protected */void ofxSimpleButton::render(ofEventArgs &event){
    render();
}

//--------------------------------------------------------------
/*protected */void ofxSimpleButton::touchDown(ofTouchEventArgs &touch){
    if (touch.id == 0 && isInside(touch.x, touch.y)) {
        buttonAction(true);
    }
    
}

//--------------------------------------------------------------
/*protected */void ofxSimpleButton::touchUp(ofTouchEventArgs &touch){
    if (buttonType == TYPE_BUTTON) {
        bSelect = bToggle = false;
    }else if (buttonType == TYPE_TOGGLE) {
        bSelect = false;
    }
}

//--------------------------------------------------------------
/*protected */void ofxSimpleButton::touchMoved(ofTouchEventArgs &touch){
    if (!fixPos && bSelect) {
        setPos(touch.x - rect.width/2, touch.y - rect.height/2);
    }
}

//--------------------------------------------------------------
/*protected */void ofxSimpleButton::touchDoubleTap(ofTouchEventArgs &touch){
    if (isInside(touch.x, touch.y)) { bDoubleTab = !bDoubleTab; }
}

//--------------------------------------------------------------
/*protected */void ofxSimpleButton::mousePressed(ofMouseEventArgs &mouse){
    mousePressed(mouse.x, mouse.y);
}

//--------------------------------------------------------------
/*protected */void ofxSimpleButton::mouseReleased(ofMouseEventArgs &mouse){
    if (buttonType == TYPE_BUTTON) { bSelect = bToggle = false; }
    else if (buttonType == TYPE_TOGGLE) { bSelect = false; }
}

//--------------------------------------------------------------
/*protected */void ofxSimpleButton::mouseMoved(ofMouseEventArgs &mouse){
    if (!fixPos && bSelect) {
        setPos(mouse.x, mouse.y);
    }
}

//--------------------------------------------------------------
/*protected */void ofxSimpleButton::mouseDragged(ofMouseEventArgs &mouse){
    if (!fixPos && bSelect) {
        setPos(mouse.x, mouse.y);
    }
}

//--------------------------------------------------------------
/*protected */void ofxSimpleButton::buttonAction(bool b){
    if (buttonType == TYPE_BUTTON) {
        bToggle = true;
    }else if (buttonType == TYPE_TOGGLE) {
        bToggle = !bToggle;
    }
}

//--------------------------------------------------------------
/*protected */bool ofxSimpleButton::isInside(float x, float y){
    if (!bAppear) return false;
    return  bSelect = x>= (rect.x + movedPos.x) && x <= (rect.x + movedPos.x) + rect.width && y >= (rect.y + movedPos.y) && y <= (rect.y + movedPos.y) + rect.height ;
}

//--------------------------------------------------------------
/*protected */string ofxSimpleButton::checkExtension(const string &route, const string &add){
    string result = "";
    string extension = ".png";
    string extensionType[3] = { ".png", ".jpg", ".jpeg" };
    for(int i = 0; i < 3; i++){
        int pos = route.find(extensionType[i]);
        if(pos != string::npos){
            result = i;
            extension = extensionType[i];
            result.append(route, 0, pos);
        }
    }
    if(add != ""){ result += add; }
    result += extension;
    return result;
}
