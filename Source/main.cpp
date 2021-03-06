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
 @file  main.cpp
 @brief main
 @author minseob (https://github.com/rasidin)
 *********************************************************************/
#include <string.h>
#include <malloc.h>

#include <vector>

#include <Managers/ResourceManager.h>
#include <Managers/TaskManager.h>

#include "TextureConverter.h"
#include "FontConverter.h"
#include "ModelConverter.h"

#include "Definitions.h"
#include "Logger.h"

Logger sLog;

enum class ReturnValue {
    OK = 0,
    UnknownCommand,
    FailedToConvert,
    UnknownError,
};

void PrintUsage()
{
    LOG_USAGE<<"LimitEngineTextureTool";
    LOG_USAGE<<"by minseob";
    LOG_USAGE<<Logger::Tag::NextLine;
    LOG_USAGE<<"Usage : lett [command] -i=[InputFile] -o=[OutputFile]";
    LOG_USAGE<<"Commands :";
    LOG_USAGE<<" let - convert to LimitEngineTexture";
    LOG_USAGE<<" font - convert font";
    LOG_USAGE<<" irr - generate irradiance map (Spherical)";
    LOG_USAGE<<" pfr - generate prefiltered reflection map (Spherical)";
    LOG_USAGE<<" ebl - generate environment BRDF 2D LUT";
    LOG_USAGE<<"Options :";
    LOG_USAGE<<" -i=Input file (or Image file)";
    LOG_USAGE<<" -o=Output file";
    LOG_USAGE<<" -t=Text file";
}

class Arguments
{
    struct Argument
    {
        char *CopiedArg = nullptr;
        char *Name = nullptr;
        char *Value = nullptr;
        Argument() {}
        Argument(const Argument &In) {
            CopyFrom(In);
        }
        Argument(const char *argword) {
            CopiedArg = _strdup(argword);
            size_t arglen = strlen(argword);
            size_t splitter_pos = 0u;
            for (splitter_pos = 0; splitter_pos < arglen; splitter_pos++) {
                if (argword[splitter_pos] == '=') {
                    break;
                }
            }
            if (splitter_pos < arglen) {
                CopiedArg[splitter_pos] = 0;
                Name = CopiedArg;
                Value = &CopiedArg[splitter_pos + 1];
            }
            else {
                Name = CopiedArg;
            }
        }
        ~Argument() {
            if (CopiedArg) free(CopiedArg);
        }
        void operator = (const Argument &In) {
            CopyFrom(In);
        }
        void CopyFrom(const Argument &In) {
            size_t copiedArgSize = strlen(In.Name) + ((In.Value) ? strlen(In.Value) : 0) + 2;
            CopiedArg = (char*)malloc(copiedArgSize);
            memset(CopiedArg, 0, copiedArgSize);
            memcpy(CopiedArg, In.Name, strlen(In.Name));
            Name = CopiedArg;
            if (In.Value) {
                uintptr_t valueOffset = reinterpret_cast<uintptr_t>(In.Value) - reinterpret_cast<uintptr_t>(In.CopiedArg);
                memcpy(CopiedArg + valueOffset, In.Value, strlen(In.Value));
                Value = CopiedArg + valueOffset;
            }
        }
    };
    Argument command;
    std::vector<Argument> arguments;

public:
    Arguments(int Count, char **args) 
    {
        for (int idx = 1; idx < Count; idx++) {
            Argument argument(args[idx]);
            if (argument.Value == nullptr)
                command = argument;
            else
                arguments.push_back(argument);
        }
    }
    bool Contains(const char *Name) const {
        for (size_t idx = 0; idx < arguments.size(); idx++) {
            if (strcmp(arguments[idx].Name, Name) == 0) {
                return true;
            }
        }
        return false;
    }
    char* GetCommand() const { return command.Name; }
    char* operator [](const char *Name) {
        for (size_t idx = 0; idx < arguments.size(); idx++) {
            if (strcmp(arguments[idx].Name, Name) == 0) {
                return arguments[idx].Value;
            }
        }
        return nullptr;
    }
    char* operator [](int Index) {
        return arguments[Index].Value;
    }
    void PrintToLog() const {
        for (size_t idx = 0; idx < arguments.size(); idx++) {
            printf("\n{ ");
            if (arguments[idx].Name && arguments[idx].Value) {
                printf("%s = %s ", arguments[idx].Name, arguments[idx].Value);
            }
            else {
                printf("%s ", arguments[idx].Name);
            }
            printf("}\n");
        }
    }
};

int PrintError(ReturnValue Value)
{
    LOG_ERROR << "!!Error!! ";
    switch (Value)
    {
    case ReturnValue::UnknownCommand:
        LOG_ERROR | "Unkown Command";
        break;
    default:
        LOG_ERROR | "Unkown Error";
        Value = ReturnValue::UnknownError;
        break;
    }
    return static_cast<int>(Value);
}

int main(int argc, char **argv)
{
    sLog.SetVerbose(3);

    PrintUsage();

    Arguments arguments(argc, argv);
    arguments.PrintToLog();

    const char *command = arguments.GetCommand();
    if (command) {
        sLog% LogLevelDebug << "Command : " | command;
    }

    LimitEngine::ResourceManager *leResourceManager = new LimitEngine::ResourceManager();
    LimitEngine::TaskManager *leTaskManager = new LimitEngine::TaskManager();

    leTaskManager->Init();

    if (strcmp(command, "let") == 0) {
        TextureConverter converter;
        TextureConverter::ConvertOptions Options;
        if (!converter.Convert(arguments["-i"], arguments["-o"], Options)) {
            return PrintError(ReturnValue::FailedToConvert);
        }
    }
    else if (strcmp(command, "irr") == 0) {
        TextureConverter converter;
        TextureConverter::ConvertOptions Options;
        if (arguments.Contains("-w") && arguments.Contains("-h")) {
            Options.FilteredImageSize.SetWidth(atoi(arguments["-w"]));
            Options.FilteredImageSize.SetHeight(atoi(arguments["-h"]));
        }
        if (arguments.Contains("-s")) {
            Options.SampleCount = atoi(arguments["-s"]);
        }
        Options.Filter = TextureConverter::ConvertOptions::FilterType::Irradiance;
        if (!converter.Convert(arguments["-i"], arguments["-o"], Options)) {
            return PrintError(ReturnValue::FailedToConvert);
        }
    }
    else if (strcmp(command, "pfe") == 0) {
        TextureConverter converter;
        TextureConverter::ConvertOptions Options;
        if (arguments.Contains("-w") && arguments.Contains("-h")) {
            Options.FilteredImageSize.SetWidth(atoi(arguments["-w"]));
            Options.FilteredImageSize.SetHeight(atoi(arguments["-h"]));
        }
        if (arguments.Contains("-s")) {
            Options.SampleCount = atoi(arguments["-s"]);
        }
        Options.Filter = TextureConverter::ConvertOptions::FilterType::PrefilteredEnvironment;
        if (!converter.Convert(arguments["-i"], arguments["-o"], Options)) {
            return PrintError(ReturnValue::FailedToConvert);
        }
    }
    else if (strcmp(command, "ebl") == 0) {
        TextureConverter converter;
        TextureConverter::ConvertOptions Options;
        if (arguments.Contains("-w") && arguments.Contains("-h")) {
            Options.FilteredImageSize.SetWidth(atoi(arguments["-w"]));
            Options.FilteredImageSize.SetHeight(atoi(arguments["-h"]));
        }
        if (arguments.Contains("-s")) {
            Options.SampleCount = atoi(arguments["-s"]);
        }
        Options.Filter = TextureConverter::ConvertOptions::FilterType::EnvironmentBRDFLUT;
        if (!converter.Convert(nullptr, arguments["-o"], Options)) {
            return PrintError(ReturnValue::FailedToConvert);
        }
    }
    else if (strcmp(command, "mdl") == 0) {
        ModelConverter converter;
        ModelConverter::ConvertOptions options;
        if (!converter.Convert(arguments["-i"], arguments["-o"], options)) {
            return PrintError(ReturnValue::FailedToConvert);
        }
    }
    else if (strcmp(command, "font") == 0) {
        FontConverter converter;
        if (!converter.Convert(arguments["-i"], arguments["-t"], arguments["-o"])) {
            return PrintError(ReturnValue::FailedToConvert);
        }
    }
    else {
        return PrintError(ReturnValue::UnknownCommand);
    }
    
    leTaskManager->Term();

    delete leResourceManager;
    delete leTaskManager;

    return static_cast<int>(ReturnValue::OK);
}