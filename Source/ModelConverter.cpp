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

#include <LEFloatVector3.h>
#include <LEFloatVector4.h>
#include <LEIntVector3.h>

#include <Containers/VectorArray.h>
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
            for (uint32 meshidx = 0; meshidx < outmodel->GetMeshCount(); meshidx++) {
                LimitEngine::Model::MESH* mesh = outmodel->GetMesh(meshidx);
                LimitEngine::RigidVertexBuffer* vtxbuf = (LimitEngine::RigidVertexBuffer*)mesh->vertexbuffer.Get();
                LimitEngine::RigidVertex *vertices = vtxbuf->GetVertices();

                // Need to generate normal
                if (vertices->GetNormal().IsZero()) {
                    // CPU compute version
                    for (LimitEngine::Model::DRAWGROUP* drwgrp : mesh->drawgroups) {
                        LimitEngine::VectorArray<LEMath::FloatVector3> polynormals;

                        for (const LEMath::IntVector3& index : drwgrp->indices) {
                            LEMath::FloatVector3 v0 = vertices[index.X()].GetPosition();
                            LEMath::FloatVector3 v1 = vertices[index.Y()].GetPosition();
                            LEMath::FloatVector3 v2 = vertices[index.Z()].GetPosition();

                            LEMath::FloatVector3 v01 = v0 - v1;
                            LEMath::FloatVector3 v21 = v2 - v1;

                            LEMath::FloatVector3 normal = v01.CrossProduct(v21);
                            normal = normal.Normalize();

                            vertices[index.X()].SetNormal((vertices[index.X()].GetNormal() + normal).Normalize());
                            vertices[index.Y()].SetNormal((vertices[index.Y()].GetNormal() + normal).Normalize());
                            vertices[index.Z()].SetNormal((vertices[index.Z()].GetNormal() + normal).Normalize());
                        }
                    }
                }
            }
            LimitEngine::ResourceManager::GetSingleton().SaveResource(OutFilename, outmodel);
            return true;
        }
    }
    return false;
}