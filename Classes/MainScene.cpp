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

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
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

	auto screenWidth = Director::getInstance()->getVisibleSize().width;
	auto screenHeight = Director::getInstance()->getVisibleSize().height;

	auto bg = Sprite::create("bg.png");
	if (bg == nullptr)
	{
		problemLoading("bg.png");
		return false;
	}

	bg->setPosition(screenWidth / 2.0f, screenHeight / 2.0f);
	this->addChild(bg);

	this->createBall(currentBall);

	this->scheduleUpdate();

	// creating a keyboard event listener
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	label = Label::createWithSystemFont("My Label Text", "Arial", 20);
	this->addChild(label);
	label->setPosition(0 + label->getContentSize().width / 2, label->getContentSize().height / 2);

	return true;
}

void MainScene::createBall(int num)
{
	Vec2 pos;

	if (ball != nullptr)
	{
		pos = ball->getPosition();
		this->removeChild(ball);
	}
	else
	{
		auto screenWidth = Director::getInstance()->getVisibleSize().width;
		auto screenHeight = Director::getInstance()->getVisibleSize().height;
		pos = Vec2(screenWidth / 2.0f, screenHeight / 2.0f);
	}

	std::ostringstream stringStream;
	stringStream << "ball0" << num << ".png";
	ball = Sprite::create(stringStream.str());
	if (ball == nullptr)
	{
		problemLoading("ball.png");
		return;
	}

	ball->setPosition(pos);
	ball->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
	this->addChild(ball);
	currentBall = num;
}
void MainScene::update(float delta)
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

	auto lastString = label->getString();
	std::ostringstream stringStream;
	stringStream << "Ball: " << currentBall;

	stringStream << " Opacity: " << (int)((opacity / 255.0f) * 100) << "%";

	stringStream << " Blend: ";
	auto blend = ball->getBlendFunc();

	if ((blend.dst == BlendFunc::ALPHA_NON_PREMULTIPLIED.dst) && (blend.src == BlendFunc::ALPHA_NON_PREMULTIPLIED.src))
	{
		stringStream << "ALPHA_NON_PREMULTIPLIED (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)";
	}
	else if ((blend.dst == BlendFunc::ALPHA_PREMULTIPLIED.dst) && (blend.src == BlendFunc::ALPHA_PREMULTIPLIED.src))
	{
		stringStream << "ALPHA_PREMULTIPLIED (GL_ONE, GL_ONE_MINUS_SRC_ALPHA)";
	}
	else if ((blend.dst == BlendFunc::ADDITIVE.dst) && (blend.src == BlendFunc::ADDITIVE.src))
	{
		stringStream << "ADDITIVE (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)";
	}
	else if ((blend.dst == BlendFunc::DISABLE.dst) && (blend.src == BlendFunc::DISABLE.src))
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

// Implementation of the keyboard event callback function prototype
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
		if (ball != nullptr)
		{
			auto blend = ball->getBlendFunc();

			if ((blend.dst == BlendFunc::ALPHA_PREMULTIPLIED.dst) && (blend.src == BlendFunc::ALPHA_PREMULTIPLIED.src))
			{
				blend = BlendFunc::ALPHA_NON_PREMULTIPLIED;
			}
			else if ((blend.dst == BlendFunc::ALPHA_NON_PREMULTIPLIED.dst) && (blend.src == BlendFunc::ALPHA_NON_PREMULTIPLIED.src))
			{
				blend = BlendFunc::ADDITIVE;
			}
			else if ((blend.dst == BlendFunc::ADDITIVE.dst) && (blend.src == BlendFunc::ADDITIVE.src))
			{
				blend = BlendFunc::DISABLE;
			}
			else if ((blend.dst == BlendFunc::DISABLE.dst) && (blend.src == BlendFunc::DISABLE.src))
			{
				blend = BlendFunc::ALPHA_PREMULTIPLIED;
			}

			ball->setBlendFunc(blend);
		}
		break;
	case EventKeyboard::KeyCode::KEY_1:
		this->createBall(1);
		break;
	case EventKeyboard::KeyCode::KEY_2:
		this->createBall(2);
		break;
	case EventKeyboard::KeyCode::KEY_3:
		this->createBall(3);
		break;
	case EventKeyboard::KeyCode::KEY_4:
		this->createBall(4);
		break;
	case EventKeyboard::KeyCode::KEY_DELETE:
		if (ball != nullptr)
		{
			auto emitter = ParticleFire::create();
			emitter->setDuration(1);
			emitter->setOpacity(ball->getOpacity());
			emitter->setBlendFunc(ball->getBlendFunc());
			emitter->setPosition(ball->getPosition());
			emitter->setDisplayFrame(ball->getSpriteFrame());

			addChild(emitter, 10);
		}
		break;
	case EventKeyboard::KeyCode::KEY_ENTER:
		if (ball != nullptr)
		{
			auto emitter = ParticleFireworks::create();
			emitter->setDuration(1);
			emitter->setOpacity(ball->getOpacity());
			emitter->setBlendFunc(ball->getBlendFunc());
			emitter->setPosition(ball->getPosition());
			emitter->setDisplayFrame(ball->getSpriteFrame());

			addChild(emitter, 10);
		}
		break;
	default:
		break;
	}
}
