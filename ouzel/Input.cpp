// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include <cstring>
#include "Input.h"
#include "Engine.h"
#include "EventDispatcher.h"
#include "Scene.h"
#include "Node.h"
#include "Utils.h"

namespace ouzel
{
    namespace input
    {
        Input::Input()
        {
			memset(keyboardKeyStates, 0, sizeof(keyboardKeyStates));
			memset(mouseButtonStates, 0, sizeof(mouseButtonStates));
        }

        Input::~Input()
        {

        }

        void Input::update()
        {

        }

        void Input::setCursorVisible(bool visible)
        {
            OUZEL_UNUSED(visible);
        }

        bool Input::isCursorVisible() const
        {
            return false;
        }

        void Input::startGamepadDiscovery()
        {

        }

        void Input::stopGamepadDiscovery()
        {

        }

        void Input::keyDown(KeyboardKey key, uint32_t modifiers)
        {
            KeyboardEventPtr event = std::make_shared<KeyboardEvent>();
            event->key = key;
            event->modifiers = modifiers;

            if (!keyboardKeyStates[static_cast<uint32_t>(key)])
            {
                keyboardKeyStates[static_cast<uint32_t>(key)] = true;

                event->type = Event::Type::KEY_DOWN;
                sharedEngine->getEventDispatcher()->dispatchEvent(event, shared_from_this());
            }
            else
            {
                event->type = Event::Type::KEY_REPEAT;
                sharedEngine->getEventDispatcher()->dispatchEvent(event, shared_from_this());
            }
        }

        void Input::keyUp(KeyboardKey key, uint32_t modifiers)
        {
            keyboardKeyStates[static_cast<uint32_t>(key)] = false;

            KeyboardEventPtr event = std::make_shared<KeyboardEvent>();
            event->type = Event::Type::KEY_UP;
            event->key = key;
            event->modifiers = modifiers;

            sharedEngine->getEventDispatcher()->dispatchEvent(event, shared_from_this());
        }

        void Input::mouseDown(MouseButton button, const Vector2& position, uint32_t modifiers)
        {
            mouseButtonStates[static_cast<uint32_t>(button)] = true;

            MouseEventPtr event = std::make_shared<MouseEvent>();
            event->type = Event::Type::MOUSE_DOWN;
            event->button = button;
            event->position = position;
            event->modifiers = modifiers;

            sharedEngine->getEventDispatcher()->dispatchEvent(event, shared_from_this());

            if (scene::ScenePtr scene = sharedEngine->getSceneManager()->getScene())
            {
                scene::NodePtr node = scene->pickNode(position);
                mouseDownOnNode(node, position);
            }
        }

        void Input::mouseUp(MouseButton button, const Vector2& position, uint32_t modifiers)
        {
            mouseButtonStates[static_cast<uint32_t>(button)] = false;

            MouseEventPtr event = std::make_shared<MouseEvent>();
            event->type = Event::Type::MOUSE_UP;
            event->button = button;
            event->position = position;
            event->modifiers = modifiers;

            sharedEngine->getEventDispatcher()->dispatchEvent(event, shared_from_this());

            if (scene::ScenePtr scene = sharedEngine->getSceneManager()->getScene())
            {
                scene::NodePtr node = scene->pickNode(position);
                mouseUpOnNode(node, position);
            }
        }

        void Input::mouseMove(const Vector2& position, uint32_t modifiers)
        {
            cursorPosition = position;

            MouseEventPtr event = std::make_shared<MouseEvent>();
            event->type = Event::Type::MOUSE_MOVE;
            event->position = position;
            event->modifiers = modifiers;

            sharedEngine->getEventDispatcher()->dispatchEvent(event, shared_from_this());

            if (scene::ScenePtr scene = sharedEngine->getSceneManager()->getScene())
            {
                scene::NodePtr node = scene->pickNode(position);
                mouseEnterNode(node, position);
            }

            if (!mouseDownNode.expired())
            {
                mouseDragNode(mouseDownNode.lock(), position);
            }
        }

        void Input::mouseScroll(const Vector2& scroll, const Vector2& position, uint32_t modifiers)
        {
            MouseEventPtr event = std::make_shared<MouseEvent>();
            event->type = Event::Type::MOUSE_SCROLL;
            event->position = position;
            event->scroll = scroll;
            event->modifiers = modifiers;

            sharedEngine->getEventDispatcher()->dispatchEvent(event, shared_from_this());
        }

        void Input::touchBegin(uint64_t touchId, const Vector2& position)
        {
            TouchEventPtr event = std::make_shared<TouchEvent>();
            event->type = Event::Type::TOUCH_BEGIN;
            event->touchId = touchId;
            event->position = position;

            sharedEngine->getEventDispatcher()->dispatchEvent(event, shared_from_this());
        }

        void Input::touchEnd(uint64_t touchId, const Vector2& position)
        {
            TouchEventPtr event = std::make_shared<TouchEvent>();
            event->type = Event::Type::TOUCH_END;
            event->touchId = touchId;
            event->position = position;

            sharedEngine->getEventDispatcher()->dispatchEvent(event, shared_from_this());
        }

        void Input::touchMove(uint64_t touchId, const Vector2& position)
        {
            TouchEventPtr event = std::make_shared<TouchEvent>();
            event->type = Event::Type::TOUCH_MOVE;
            event->touchId = touchId;
            event->position = position;

            sharedEngine->getEventDispatcher()->dispatchEvent(event, shared_from_this());
        }

        void Input::touchCancel(uint64_t touchId, const Vector2& position)
        {
            TouchEventPtr event = std::make_shared<TouchEvent>();
            event->type = Event::Type::TOUCH_CANCEL;
            event->touchId = touchId;
            event->position = position;

            sharedEngine->getEventDispatcher()->dispatchEvent(event, shared_from_this());
        }

        void Input::mouseEnterNode(const scene::NodePtr& node, const Vector2& position)
        {
            scene::NodePtr mouseOnNode = mouseNode.lock();

            if (mouseOnNode)
            {
                if (mouseOnNode == node)
                {
                    return;
                }
                else
                {
                    mouseLeaveNode(mouseOnNode, position);
                }
            }

            mouseNode = node;

            if (node && node->isReceivingInput())
            {
                MouseEventPtr enterEvent = std::make_shared<MouseEvent>();
                enterEvent->type = Event::Type::UI_ENTER_NODE;
                enterEvent->position = node->convertWorldToLocal(position);

                sharedEngine->getEventDispatcher()->dispatchEvent(enterEvent, node);
            }
        }

        void Input::mouseLeaveNode(const scene::NodePtr& node, const Vector2& position)
        {
            if (node && node->isReceivingInput())
            {
                MouseEventPtr leaveEvent = std::make_shared<MouseEvent>();
                leaveEvent->type = Event::Type::UI_LEAVE_NODE;
                leaveEvent->position = node->convertWorldToLocal(position);

                sharedEngine->getEventDispatcher()->dispatchEvent(leaveEvent, node);
            }
        }

        void Input::mouseDownOnNode(const scene::NodePtr& node, const Vector2& position)
        {
            mouseDownNode = node;

            if (node && node->isReceivingInput())
            {
                MouseEventPtr enterEvent = std::make_shared<MouseEvent>();
                enterEvent->type = Event::Type::UI_PRESS_NODE;
                enterEvent->position = node->convertWorldToLocal(position);

                sharedEngine->getEventDispatcher()->dispatchEvent(enterEvent, node);
            }
        }

        void Input::mouseUpOnNode(const scene::NodePtr& node, const Vector2& position)
        {
            scene::NodePtr mouseDownOnNode = mouseDownNode.lock();

            if (mouseDownOnNode && mouseDownOnNode->isReceivingInput())
            {
                MouseEventPtr pressEvent = std::make_shared<MouseEvent>();
                pressEvent->type = Event::Type::UI_RELEASE_NODE;
                pressEvent->position = mouseDownOnNode->convertWorldToLocal(position);

                sharedEngine->getEventDispatcher()->dispatchEvent(pressEvent, mouseDownOnNode);

                if (mouseDownOnNode == node)
                {
                    MouseEventPtr clickEvent = std::make_shared<MouseEvent>();
                    clickEvent->type = Event::Type::UI_CLICK_NODE;
                    clickEvent->position = mouseDownOnNode->convertWorldToLocal(position);

                    sharedEngine->getEventDispatcher()->dispatchEvent(clickEvent, mouseDownOnNode);
                }
            }

            mouseDownNode.reset();
        }

        void Input::mouseDragNode(const scene::NodePtr& node, const Vector2& position)
        {
            MouseEventPtr enterEvent = std::make_shared<MouseEvent>();
            enterEvent->type = Event::Type::UI_DRAG_NODE;
            enterEvent->position = node->convertWorldToLocal(position);

            sharedEngine->getEventDispatcher()->dispatchEvent(enterEvent, node);
        }

    } // namespace input
} // namespace ouzel
