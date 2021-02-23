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
 @file  ModelConverter.cpp
 @brief model converter
 @author minseob (https://github.com/rasidin)
 *********************************************************************/
#include "ModelConverter.h"

#include <Factories/ModelFactory.h>
#include <Renderer/Model.h>
#include <Managers/ResourceManager.h>

#include "Logger.h"
#include "Definitions.h"

bool ModelConverter::Convert(const char* InFilename, const char* OutFilename, const ConvertOptions& Options)
{
    LOG_SUBLOG << "Convert " | InFilename | " -> " | OutFilename;

    LimitEngine::AutoPointer<LimitEngine::ResourceManager::RESOURCE> loadedresource = LimitEngine::ResourceManager::GetSingleton().GetResourceWithoutRegister(InFilename, LimitEngine::ModelFactory::ID);
    if (loadedresource.Exists()) {
        if (LimitEngine::Model* outmodel = (LimitEngine::Model*)loadedresource->data) {
            LimitEngine::ResourceManager::GetSingleton().SaveResource(OutFilename, outmodel);
            return true;
        }
    }
    return false;
}