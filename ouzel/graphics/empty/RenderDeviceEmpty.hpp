// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "graphics/RenderDevice.hpp"

namespace ouzel
{
    namespace graphics
    {
        class RenderDeviceEmpty: public RenderDevice
        {
            friend Renderer;
        protected:
            RenderDeviceEmpty();

            virtual bool init(Window* newWindow,
                              const Size2& newSize,
                              uint32_t newSampleCount,
                              Texture::Filter newTextureFilter,
                              uint32_t newMaxAnisotropy,
                              bool newVerticalSync,
                              bool newExclusiveFullscreen,
                              bool newDepth,
                              bool newDebugRenderer) override;

            virtual bool draw(const std::vector<DrawCommand>& drawCommands) override;

            virtual BlendStateResource* createBlendState() override;
            virtual TextureResource* createTexture() override;
            virtual ShaderResource* createShader() override;
            virtual MeshBufferResource* createMeshBuffer() override;
            virtual BufferResource* createBuffer() override;
        };
    } // namespace graphics
} // namespace ouzel
