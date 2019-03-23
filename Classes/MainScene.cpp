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

#include "MainScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
	return MainScene::create();
}


bool MainScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	ball = nullptr;
	moveLeft = false;
	moveRight = false;
	moveUp = false;
	moveDown = false;
	opacityUp = false;
	opacityDown = false;
	currentBall = 1;

	if (!createBackGround())
	{
		return false;
	}

	if (!createBall(currentBall))
	{
		return false;
	}

	if (!createLabel())
	{
		return false;
	}

	if (!createKeybordListener())
	{
		return false;
	}

	scheduleUpdate();

	return true;
}

void MainScene::problemLoading(const char* filename)
{
	log("Error while loading: %s\n", filename);
}

bool MainScene::createBackGround()
{
	auto screenWidth = Director::getInstance()->getVisibleSize().width;
	auto screenHeight = Director::getInstance()->getVisibleSize().height;

	auto bg = Sprite::create("bg.png");
	if (bg == nullptr)
	{
		problemLoading("bg.png");
		return false;
	}

	bg->setPosition(screenWidth / 2.0f, screenHeight / 2.0f);
	addChild(bg);

	return true;
}

bool MainScene::createBall(int num)
{
	Vec2 pos;

	if (ball != nullptr)
	{
		pos = ball->getPosition();
		removeChild(ball);
	}
	else
	{
		auto screenWidth = Director::getInstance()->getVisibleSize().width;
		auto screenHeight = Director::getInstance()->getVisibleSize().height;
		pos = Vec2(screenWidth / 2.0f, screenHeight / 2.0f);
	}

	std::ostringstream stringStream;
	stringStream << "ball0" << num << ".png";
	auto filename = stringStream.str();
	ball = Sprite::create(filename);
	if (ball == nullptr)
	{
		problemLoading(filename.c_str());
		return false;
	}

	ball->setPosition(pos);
	ball->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
	ball->setOpacityModifyRGB(false);
	ball->setColor(Color3B::WHITE);
	addChild(ball, 250);
	currentBall = num;

	return true;
}

bool MainScene::createLabel()
{
	label = Label::createWithSystemFont("My Label Text", "Arial", 20);

	if (label == nullptr)
	{
		log("Error creating label");
		return false;
	}

	addChild(label, 255);
	label->setPosition(0 + label->getContentSize().width / 2, label->getContentSize().height / 2);

	return true;
}

bool MainScene::createKeybordListener()
{
	auto listener = EventListenerKeyboard::create();
	if (listener == nullptr)
	{
		log("Error creating keyboard listener");
		return false;
	}

	listener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

bool MainScene::createEmitter(bool isFireWorks)
{
	if (ball != nullptr)
	{
		auto emitter = isFireWorks ? (ParticleSystemQuad*)ParticleFireworks::create() : ParticleFire::create();
		if (emitter == nullptr)
		{
			log("Error creating emitter");
			return false;
		}
		emitter->setDuration(1);
		emitter->setOpacity(ball->getOpacity());
		emitter->setBlendFunc(ball->getBlendFunc());
		emitter->setPosition(ball->getPosition());
		emitter->setColor(ball->getColor());
		emitter->setOpacityModifyRGB(false);
		emitter->setDisplayFrame(ball->getSpriteFrame());

		addChild(emitter);
		return true;
	}

	return false;
}

void MainScene::update(float delta)
{
	updatePos(delta);
	updateOpacity(delta);
	updateLabel();
}

void MainScene::updatePos(float delta)
{
	if (ball == nullptr)
	{
		return;
	}

	auto savedPos = ball->getPosition();
	auto pos = savedPos;

	float moveX = (moveLeft ? 0.0f : 1.0f) - (moveRight ? 0.0f : 1.0f);
	float moveY = (moveDown ? 0.0f : 1.0f) - (moveUp ? 0.0f : 1.0f);
	auto move = Vec2(moveX, moveY) * (delta * 500.f);

	pos += move;

	if (pos != savedPos)
	{
		ball->setPosition(pos);
	}
}

void MainScene::updateOpacity(float delta)
{
	if (ball == nullptr)
	{
		return;
	}

	GLubyte savedOpacity = ball->getOpacity();
	float opacity = (float)savedOpacity;
	float changeOpacity = (opacityUp ? 1.0f : 0.0f) - (opacityDown ? 1.0f : 0.0f);
	opacity += (changeOpacity * (delta * 150.0f));
	if (opacity < 0.0f)
	{
		opacity = 0.0f;
	}
	else if (opacity > 255.0f)
	{
		opacity = 255.0f;
	}
	if (savedOpacity != (GLubyte)opacity)
	{
		ball->setOpacity((GLubyte)opacity);
	}
}

void MainScene::updateLabel()
{
	if (ball == nullptr)
	{
		return;
	}

	auto opacity = ball->getOpacity();
	auto lastString = label->getString();
	std::ostringstream stringStream;
	stringStream << "Ball: " << currentBall;

	stringStream << " Opacity: " << (int)((opacity / 255.0f) * 100) << "%";

	stringStream << " Blend: ";
	auto blend = ball->getBlendFunc();
	if (blendEqual(blend, BlendFunc::ALPHA_NON_PREMULTIPLIED))
	{
		stringStream << "ALPHA_NON_PREMULTIPLIED (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)";
	}
	else if (blendEqual(blend, BlendFunc::ALPHA_PREMULTIPLIED))
	{
		stringStream << "ALPHA_PREMULTIPLIED (GL_ONE, GL_ONE_MINUS_SRC_ALPHA)";
	}
	else if (blendEqual(blend, BlendFunc::ADDITIVE))
	{
		stringStream << "ADDITIVE (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)";
	}
	else if (blendEqual(blend, BlendFunc::DISABLE))
	{
		stringStream << "DISABLE (GL_ONE, GL_ZERO)";
	}

	std::string newString = stringStream.str();
	if (newString != lastString)
	{
		label->setString(newString);
		label->setPosition(0 + label->getContentSize().width / 2, label->getContentSize().height / 2);
		lastString = newString;
	}
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		moveUp = true;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		moveDown = true;
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		moveLeft = true;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		moveRight = true;
		break;
	case EventKeyboard::KeyCode::KEY_SHIFT:
		opacityUp = true;
		break;
	case EventKeyboard::KeyCode::KEY_CTRL:
		opacityDown = true;
		break;
	default:
		break;
	}
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		moveUp = false;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		moveDown = false;
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		moveLeft = false;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		moveRight = false;
		break;
	case EventKeyboard::KeyCode::KEY_SHIFT:
		opacityUp = false;
		break;
	case EventKeyboard::KeyCode::KEY_CTRL:
		opacityDown = false;
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		changeBlend();
		break;
	case EventKeyboard::KeyCode::KEY_1:
		createBall(1);
		break;
	case EventKeyboard::KeyCode::KEY_2:
		createBall(2);
		break;
	case EventKeyboard::KeyCode::KEY_3:
		createBall(3);
		break;
	case EventKeyboard::KeyCode::KEY_0:
		changeColor();
		break;
	case EventKeyboard::KeyCode::KEY_BACKSPACE:
		createEmitter(false);
		break;
	case EventKeyboard::KeyCode::KEY_ENTER:
		createEmitter(true);
		break;
	default:
		break;
	}
}

void MainScene::changeBlend()
{
	if (ball != nullptr)
	{
		auto blend = ball->getBlendFunc();

		if (blendEqual(blend, BlendFunc::ALPHA_PREMULTIPLIED))
		{
			blend = BlendFunc::ALPHA_NON_PREMULTIPLIED;
		}
		else if (blendEqual(blend, BlendFunc::ALPHA_NON_PREMULTIPLIED))
		{
			blend = BlendFunc::ADDITIVE;
		}
		else if (blendEqual(blend, BlendFunc::ADDITIVE))
		{
			blend = BlendFunc::DISABLE;
		}
		else if (blendEqual(blend, BlendFunc::DISABLE))
		{
			blend = BlendFunc::ALPHA_PREMULTIPLIED;
		}

		ball->setBlendFunc(blend);
	}
}

void MainScene::changeColor()
{
	if (ball != nullptr)
	{
		auto color = ball->getColor();
		if (colorEqual(color, Color3B::WHITE))
		{
			color = Color3B::RED;
		}
		else if (colorEqual(color, Color3B::RED))
		{
			color = Color3B::GREEN;
		}
		else if (colorEqual(color, Color3B::GREEN))
		{
			color = Color3B::BLUE;
		}
		else if (colorEqual(color, Color3B::BLUE))
		{
			color = Color3B::WHITE;
		}
		ball->setColor(color);
	}
}

bool MainScene::blendEqual(BlendFunc blend1, BlendFunc blend2)
{
	return (blend1.src == blend2.src) && (blend1.dst == blend2.dst);
}

bool MainScene::colorEqual(cocos2d::Color3B color1, cocos2d::Color3B color2)
{
	return (color1.r == color2.r) && (color1.g == color2.g) && (color1.b == color2.b);
}
