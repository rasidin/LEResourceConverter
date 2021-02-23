/*********************************************************************
Copyright (c) 2020 LIMITGAME

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------
 @file  TextureConverter.cpp
 @brief Texture converter
 @author minseob (https://github.com/rasidin)
 *********************************************************************/
#include "TextureConverter.h"

#include <Factories/TextureFactory.h>
#include <Renderer/Texture.h>
#include <Managers/ResourceManager.h>

#include "Logger.h"
#include "Definitions.h"
#include "EXRSourceFactory.h"

#include "TextureImageFilters/TextureImageFilterIrradianceMap.h"
#include "TextureImageFilters/TextureImageFilterPrefilteredReflectionMap.h"
#include "TextureImageFilters/TextureImageFilterEnvironmentBRDFLUT.h"

bool TextureConverter::Convert(const char *InFilename, const char *OutFilename, const TextureConverter::ConvertOptions &Options)
{
    LOG_SUBLOG << "Convert " | InFilename | " -> " | OutFilename;

    LimitEngine::EXRSourceFactory *EXRFactory = new LimitEngine::EXRSourceFactory();
    LimitEngine::ResourceManager::GetSingleton().AddSourceFactory("exr", EXRFactory);

    LimitEngine::AutoPointer<LimitEngine::ResourceManager::RESOURCE> loadedResource = nullptr;
    LimitEngine::AutoPointer<LimitEngine::Texture> createdTexture = nullptr;
    LimitEngine::TextureImageFilter* filter = nullptr;
    LimitEngine::Texture* outtexture = nullptr;
    if (LimitEngine::TextureFactory *textureFactory = (LimitEngine::TextureFactory*)LimitEngine::ResourceManager::GetSingleton().GetFactory(LimitEngine::TextureFactory::ID)) {
        if (Options.FilteredImageSize != LEMath::IntVector2::Zero)
            textureFactory->SetSizeFilteredImage(Options.FilteredImageSize);
        textureFactory->SetSampleCount(Options.SampleCount);

        switch (Options.Filter) {
        case ConvertOptions::FilterType::Irradiance:
            filter = new TextureImageFilterIrradianceMap(Options.SampleCount);
            textureFactory->SetImageFilter(filter);
            loadedResource = LimitEngine::ResourceManager::GetSingleton().GetResourceWithoutRegister(InFilename, LimitEngine::TextureFactory::ID);
            if (loadedResource.Exists()) {
                outtexture = (LimitEngine::Texture*)loadedResource->data;
            }
            delete filter;
            filter = nullptr;
            break;
        case ConvertOptions::FilterType::PrefilteredEnvironment:
            filter = new TextureImageFilterPrefilteredReflectionMap(Options.SampleCount, (int32)log2f((float)Options.FilteredImageSize.Width()));
            textureFactory->SetImageFilter(filter);
            loadedResource = LimitEngine::ResourceManager::GetSingleton().GetResourceWithoutRegister(InFilename, LimitEngine::TextureFactory::ID);
            if (loadedResource.Exists()) {
                outtexture = (LimitEngine::Texture*)loadedResource->data;
            }
            delete filter;
            filter = nullptr;
            break;
        case ConvertOptions::FilterType::EnvironmentBRDFLUT:
            filter = new TextureImageFilterEnvironmentBRDFLUT(Options.SampleCount);
            textureFactory->SetImageFilter(filter);
            createdTexture = static_cast<LimitEngine::Texture*>(textureFactory->CreateEmpty(Options.FilteredImageSize, LimitEngine::RendererFlag::BufferFormat::R32G32_Float));
            if (createdTexture.Exists()) {
                outtexture = createdTexture.Get();
            }
            delete filter;
            filter = nullptr;
            break;
        }
    }

    if (outtexture) {
        LimitEngine::ResourceManager::GetSingleton().SaveResource(OutFilename, outtexture);
    }

    return true;
}