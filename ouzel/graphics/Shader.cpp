// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "Shader.hpp"
#include "ShaderResource.hpp"
#include "Renderer.hpp"
#include "RenderDevice.hpp"
#include "core/Engine.hpp"
#include "files/FileSystem.hpp"

namespace ouzel
{
    namespace graphics
    {
        Shader::Shader()
        {
            resource = sharedEngine->getRenderer()->getDevice()->createShader();
        }

        Shader::~Shader()
        {
            if (sharedEngine && resource) sharedEngine->getRenderer()->getDevice()->deleteResource(resource);
        }

        bool Shader::init(const std::string& newPixelShader,
                          const std::string& newVertexShader,
                          const std::vector<VertexAttribute>& newVertexAttributes,
                          const std::vector<ConstantInfo>& newPixelShaderConstantInfo,
                          const std::vector<ConstantInfo>& newVertexShaderConstantInfo,
                          uint32_t newPixelShaderDataAlignment,
                          uint32_t newVertexShaderDataAlignment,
                          const std::string& newPixelShaderFunction,
                          const std::string& newVertexShaderFunction)
        {
            vertexAttributes = newVertexAttributes;

            pixelShaderFilename = newPixelShader;
            vertexShaderFilename = newVertexShader;

            std::vector<uint8_t> pixelShaderData;

            if (!sharedEngine->getFileSystem()->readFile(newPixelShader, pixelShaderData))
            {
                return false;
            }

            std::vector<uint8_t> vertexShaderData;

            if (!sharedEngine->getFileSystem()->readFile(newVertexShader, vertexShaderData))
            {
                return false;
            }

            sharedEngine->getRenderer()->executeOnRenderThread(std::bind(&ShaderResource::init,
                                                                         resource,
                                                                         pixelShaderData,
                                                                         vertexShaderData,
                                                                         newVertexAttributes,
                                                                         newPixelShaderConstantInfo,
                                                                         newVertexShaderConstantInfo,
                                                                         newPixelShaderDataAlignment,
                                                                         newVertexShaderDataAlignment,
                                                                         newPixelShaderFunction,
                                                                         newVertexShaderFunction));

            return true;
        }

        bool Shader::init(const std::vector<uint8_t>& newPixelShader,
                          const std::vector<uint8_t>& newVertexShader,
                          const std::vector<VertexAttribute>& newVertexAttributes,
                          const std::vector<Shader::ConstantInfo>& newPixelShaderConstantInfo,
                          const std::vector<Shader::ConstantInfo>& newVertexShaderConstantInfo,
                          uint32_t newPixelShaderDataAlignment,
                          uint32_t newVertexShaderDataAlignment,
                          const std::string& newPixelShaderFunction,
                          const std::string& newVertexShaderFunction)
        {
            vertexAttributes = newVertexAttributes;
            pixelShaderFilename.clear();
            vertexShaderFilename.clear();

            sharedEngine->getRenderer()->executeOnRenderThread(std::bind(&ShaderResource::init,
                                                                         resource,
                                                                         newPixelShader,
                                                                         newVertexShader,
                                                                         newVertexAttributes,
                                                                         newPixelShaderConstantInfo,
                                                                         newVertexShaderConstantInfo,
                                                                         newPixelShaderDataAlignment,
                                                                         newVertexShaderDataAlignment,
                                                                         newPixelShaderFunction,
                                                                         newVertexShaderFunction));

            return  true;
        }

        const std::vector<VertexAttribute>& Shader::getVertexAttributes() const
        {
            return vertexAttributes;
        }
    } // namespace graphics
} // namespace ouzel
