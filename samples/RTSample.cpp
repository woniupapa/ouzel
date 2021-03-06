// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "RTSample.hpp"
#include "MainMenu.hpp"

using namespace std;
using namespace ouzel;

RTSample::RTSample():
    backButton("button.png", "button_selected.png", "button_down.png", "", "Back", "arial.fnt", 0, Color::BLACK, Color::BLACK, Color::BLACK)
{
    eventHandler.gamepadHandler = bind(&RTSample::handleGamepad, this, placeholders::_1, placeholders::_2);
    eventHandler.uiHandler = bind(&RTSample::handleUI, this, placeholders::_1, placeholders::_2);
    eventHandler.keyboardHandler = bind(&RTSample::handleKeyboard, this, placeholders::_1, placeholders::_2);
    sharedEngine->getEventDispatcher()->addEventHandler(&eventHandler);

    rtLayer.reset(new scene::Layer());
    addLayer(rtLayer.get());

    std::shared_ptr<graphics::Texture> renderTarget = std::make_shared<graphics::Texture>();
    renderTarget->init(Size2(256.0f, 256.0f), graphics::Texture::RENDER_TARGET, 0, 1);
    renderTarget->setClearColor(Color(0, 64, 0));

    rtCamera.reset(new scene::Camera());
    rtCamera->setRenderTarget(renderTarget);

    rtLayer->addChild(rtCamera.get());

    camera1.reset(new scene::Camera());
    camera1->setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    camera1->setTargetContentSize(Size2(400.0f, 600.0f));
    camera1->setViewport(Rectangle(0.0f, 0.0f, 0.5f, 1.0f));

    camera2.reset(new scene::Camera());
    camera2->setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    camera2->setTargetContentSize(Size2(400.0f, 600.0f));
    camera2->setViewport(Rectangle(0.5f, 0.0f, 0.5f, 1.0f));

    layer.addChild(camera1.get());
    layer.addChild(camera2.get());
    addLayer(&layer);

    characterSprite.reset(new ouzel::scene::Sprite("run.json"));
    characterSprite->play(true);

    rtCharacter.reset(new scene::Actor());
    rtCharacter->addComponent(characterSprite.get());

    rtLayer->addChild(rtCharacter.get());

    rtSprite.reset(new scene::Sprite());
    rtSprite->init(renderTarget);
    rtActor.reset(new scene::Actor());
    rtActor->addComponent(rtSprite.get());
    layer.addChild(rtActor.get());

    guiCamera.setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    guiCamera.setTargetContentSize(Size2(800.0f, 600.0f));

    guiLayer.addChild(&guiCamera);
    addLayer(&guiLayer);

    guiLayer.addChild(&menu);

    backButton.setPosition(Vector2(-200.0f, -200.0f));
    menu.addWidget(&backButton);
}

bool RTSample::handleGamepad(Event::Type type, const GamepadEvent& event)
{
    if (type == Event::Type::GAMEPAD_BUTTON_CHANGE)
    {
        if (event.pressed &&
            event.button == input::GamepadButton::FACE_RIGHT)
        {
            sharedEngine->getSceneManager()->setScene(std::unique_ptr<scene::Scene>(new MainMenu()));
        }
    }

    return true;
}

bool RTSample::handleUI(Event::Type type, const UIEvent& event) const
{
    if (type == Event::Type::ACTOR_CLICK && event.actor == &backButton)
    {
        sharedEngine->getSceneManager()->setScene(std::unique_ptr<scene::Scene>(new MainMenu()));
    }

    return true;
}

bool RTSample::handleKeyboard(Event::Type type, const KeyboardEvent& event) const
{
    if (type == Event::Type::KEY_PRESS)
    {
        switch (event.key)
        {
            case input::KeyboardKey::ESCAPE:
            case input::KeyboardKey::MENU:
                sharedEngine->getSceneManager()->setScene(std::unique_ptr<scene::Scene>(new MainMenu()));
                break;
            default:
                break;
        }
    }
    
    return true;
}
