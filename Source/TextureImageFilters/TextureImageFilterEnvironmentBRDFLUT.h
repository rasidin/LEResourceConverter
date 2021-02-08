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
@file  TextureImageFilterEnvironmentBRDFLUT.h
@brief Texture image filter for environment BRDF 2D LUT
@author minseob (leeminseob@outlook.com)
*********************************************************************/
#ifndef LERESOURCECONVERTER_TEXTUREIMAGEFILTERENVIRONMENTBRDFLUT_H_
#define LERESOURCECONVERTER_TEXTUREIMAGEFILTERENVIRONMENTBRDFLUT_H_

#include <Factories/TextureFactory.h>
#include <GenerateEnvironmentBRDFLUT.h>

class TextureImageFilterEnvironmentBRDFLUT : public LimitEngine::TextureImageFilter
{
    uint32 mSampleNum = 1024u;
public:
    TextureImageFilterEnvironmentBRDFLUT(uint32 samplenum) : mSampleNum(samplenum) {}

    virtual bool FilterImage(LimitEngine::TextureSourceImage* srcimg, LimitEngine::SerializedTextureSource* tarimg)
    {
        float* tarcolor = (float*)tarimg->GetColorData();
        CUDAImageUtilities::GenerateEnvironmentBRDFLUT(tarcolor, tarimg->GetSize().Width(), tarimg->GetSize().Height(), mSampleNum);

        return true;
    }
    virtual LimitEngine::RendererFlag::BufferFormat GetFilteredImageFormat() const override { return LimitEngine::RendererFlag::BufferFormat::R32G32_Float; }
    virtual uint32 GetMipCount() const override { return 1u; }
};

#endif // LERESOURCECONVERTER_TEXTUREIMAGEFILTERENVIRONMENTBRDFLUT_H_