// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#import "ViewIOS.h"
#include "core/Engine.h"
#include "core/Window.h"

@implementation ViewIOS

-(void)touchesBegan:(NSSet*)touches withEvent:(__unused ::UIEvent*)event
{
    for (UITouch* touch in touches)
    {
        CGPoint location = [touch locationInView:self];

        ouzel::sharedEngine->getInput()->touchBegin(reinterpret_cast<uint64_t>(touch),
                                                    ouzel::sharedEngine->getWindow()->convertWindowToNormalizedLocation(ouzel::Vector2(static_cast<float>(location.x),
                                                                                                                                       static_cast<float>(location.y))));
    }
}

-(void)touchesMoved:(NSSet*)touches withEvent:(__unused ::UIEvent*)event
{
    for (UITouch* touch in touches)
    {
        CGPoint location = [touch locationInView:self];

        ouzel::sharedEngine->getInput()->touchMove(reinterpret_cast<uint64_t>(touch),
                                                   ouzel::sharedEngine->getWindow()->convertWindowToNormalizedLocation(ouzel::Vector2(static_cast<float>(location.x),
                                                                                                                                      static_cast<float>(location.y))));
    }
}

-(void)touchesEnded:(NSSet*)touches withEvent:(__unused ::UIEvent*)event
{
    for (UITouch* touch in touches)
    {
        CGPoint location = [touch locationInView:self];

        ouzel::sharedEngine->getInput()->touchEnd(reinterpret_cast<uint64_t>(touch),
                                                  ouzel::sharedEngine->getWindow()->convertWindowToNormalizedLocation(ouzel::Vector2(static_cast<float>(location.x),
                                                                                                                                     static_cast<float>(location.y))));
    }
}

-(void)touchesCancelled:(NSSet*)touches withEvent:(__unused ::UIEvent*)event
{
    for (UITouch* touch in touches)
    {
        CGPoint location = [touch locationInView:self];

        ouzel::sharedEngine->getInput()->touchCancel(reinterpret_cast<uint64_t>(touch),
                                                     ouzel::sharedEngine->getWindow()->convertWindowToNormalizedLocation(ouzel::Vector2(static_cast<float>(location.x),
                                                                                                                                        static_cast<float>(location.y))));
    }
}

@end