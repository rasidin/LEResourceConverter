/*********************************************************************
Copyright(c) 2020 LIMITGAME
Permission is hereby granted, free of charge, to any person
obtaining a copy of this softwareand associated documentation
files(the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and /or sell copies of
the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :
The above copyright noticeand this permission notice shall be
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
----------------------------------------------------------------------
@file ImageUtilities.h
@brief Image utilities
@author minseob (leeminseob@outlook.com)
**********************************************************************/
#ifndef LERESOURCECONVERTER_IMAGEUTILITIES_H_
#define LERESOURCECONVERTER_IMAGEUTILITIES_H_

#include <LERenderer>
#include <LEHalfVector4.h>
#include <LEFloatVector4.h>

void ConvertImageToR32G32B32A32(const void* srcdata, const LimitEngine::RendererFlag::BufferFormat &srcformat, int width, int height, void* outdata)
{
    LEMath::FloatVector4* outfvec4 = (LEMath::FloatVector4*)outdata;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint32 pixelindex = x + y * width;
            switch (srcformat)
            {
            case LimitEngine::RendererFlag::BufferFormat::R16G16B16A16_Float:
                outfvec4[pixelindex] = LEMath::FloatVector4(
                    ((LEMath::HalfVector4*)srcdata)[pixelindex].X(),
                    ((LEMath::HalfVector4*)srcdata)[pixelindex].Y(),
                    ((LEMath::HalfVector4*)srcdata)[pixelindex].Z(),
                    ((LEMath::HalfVector4*)srcdata)[pixelindex].W()
                );
                break;
            default:
                LEASSERT(false);
            }
        }
    }
}

#endif // LERESOURCECONVERTER_IMAGEUTILITIES_H_