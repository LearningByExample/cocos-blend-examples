/****************************************************************************
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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class MainScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(MainScene);

private:
	cocos2d::Sprite *ball;
	cocos2d::Label *label;

	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;
	bool opacityUp;
	bool opacityDown;
	unsigned short int currentBall;

	virtual void update(float delta);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	void updatePos(float delta);
	void updateOpacity(float delta);
	void updateLabel();

	void problemLoading(const char* filename);
	bool createBackGround();
	bool createBall(int num);
	bool createLabel();
	bool createKeybordListener();
	bool createEmitter(bool isFireWorks);

	void changeBlend();
	void changeColor();

	bool blendEqual(cocos2d::BlendFunc blend1, cocos2d::BlendFunc blend2);
	bool colorEqual(cocos2d::Color3B color1, cocos2d::Color3B color2);

};

#endif // __HELLOWORLD_SCENE_H__
