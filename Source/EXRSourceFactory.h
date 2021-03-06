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
 @file  EXRSourceFactory.h
 @brief EXRSourceFactory
 @author minseob (https://github.com/rasidin)
 *********************************************************************/
#pragma once

#include <ImathInt64.h>
#include <ImfIO.h>
#include <ImfRGBAFile.h>

#include <Factories/ResourceSourceFactory.h>
#include <Renderer/Texture.h>

namespace OPENEXR_IMF_INTERNAL_NAMESPACE {
class EXRStream : public IStream {
public:
    EXRStream(const void *Data, size_t Size) : IStream(""), mData(Data), mSize(Size), mCurrentDataPtr((char*)Data)
    {}
    virtual ~EXRStream() {}

    virtual bool read(char c[], int n) override { ::memcpy(c, mCurrentDataPtr, n); mCurrentDataPtr += n; return true; }
    virtual char* readMemoryMapped(int n) override { char *Output = mCurrentDataPtr; mCurrentDataPtr += n; return Output; }
    virtual Int64 tellg() { return (Int64)mCurrentDataPtr - (Int64)mData; }
    virtual void seekg(Int64 pos) override { mCurrentDataPtr = (char*)mData + pos; }
private:
    const void *mData = nullptr;
    char *mCurrentDataPtr = nullptr;
    size_t mSize = 0u;
};
}

namespace LimitEngine {
class EXRImage : public TextureSourceImage
{
    LEMath::IntVector2 mSize;
    uint32 mRowPitch;
    RendererFlag::BufferFormat mFormat;
    void* mColorData;
    size_t mColorDataSize;
public:
    EXRImage() {}
    virtual ~EXRImage() {}
    virtual bool Load(const void *Data, size_t Size) final override {
        OPENEXR_IMF_INTERNAL_NAMESPACE::EXRStream IOStream(Data, Size);
        Imf::RgbaInputFile EXRInputFile(IOStream);

        Imath::Box2i dw = EXRInputFile.dataWindow();
        mSize = LEMath::IntSize(dw.max.x - dw.min.x + 1, dw.max.y - dw.min.y + 1);

        mFormat = RendererFlag::BufferFormat::R16G16B16A16_Float;
        mRowPitch = sizeof(OPENEXR_IMF_INTERNAL_NAMESPACE::Rgba) * mSize.Width();

        mColorDataSize = sizeof(OPENEXR_IMF_INTERNAL_NAMESPACE::Rgba) * mSize.Size();
        mColorData = ::malloc(mColorDataSize);

        EXRInputFile.setFrameBuffer(static_cast<OPENEXR_IMF_INTERNAL_NAMESPACE::Rgba*>(mColorData), 1, mSize.Width());
        EXRInputFile.readPixels(dw.min.y, dw.max.y);
        
        return true;
    }
    virtual LEMath::IntSize GetSize() const final override { return mSize; }
    virtual uint32 GetDepth() const final override { return 1; }
    virtual bool IsCubemap() const final override { return false; }
    virtual RendererFlag::BufferFormat GetFormat() const final override { return mFormat; }
    virtual uint32 GetRowPitch() const final override { return mRowPitch; }
    virtual void* GetColorData() const final override { return mColorData; }
    virtual size_t GetColorDataSize() const final override { return mColorDataSize; }
    virtual uint32 GetMipCount() const final override { return 1u; }
};
class EXRSourceFactory : public ResourceSourceFactory 
{
public:
    EXRSourceFactory() {}
    virtual ~EXRSourceFactory() {}

    virtual ID GetID() const override { return GENERATE_RESOURCEFACTORY_ID("EXRT"); }

    virtual void* ConvertRawData(const void *Data, size_t Size) const {
        EXRImage *Output = new EXRImage();
        if (Output->Load(Data, Size)) {
            return dynamic_cast<void*>(Output);
        }
        delete Output;
        return nullptr;
    }
};
}