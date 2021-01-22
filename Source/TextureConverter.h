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
 @file  TextureConverter.h
 @brief Converter for texture
 @author minseob (leeminseob@outlook.com)
 *********************************************************************/
#pragma once

#include <LEIntVector2.h>

class TextureConverter
{
public:
    struct ConvertOptions
    {
        LEMath::IntVector2 FilteredImageSize = LEMath::IntVector2::Zero;
        int SampleCount = 1024;

        bool GenerateIrradiance = false;
        bool GenerateReflection = false;
        bool GenerateEnvironmentBRDF = false;
    };

public:
    TextureConverter() {}
    ~TextureConverter() {}

    bool Convert(const char *InFilename, const char *OutFilename, const ConvertOptions &Options);
};