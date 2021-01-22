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
@file  Logger.h
@brief Logger
@author minseob(leeminseob@outlook.com)
*********************************************************************/

#pragma once

#include <stdio.h>

struct Logger {
    enum class Tag {
        NextLine = 1,
        Tab
    };

    bool IsLogging = false;
    int Verbose = 0;
    int CurrentVerbose = 0;
    int TabSpace = 4;

    Logger& SetVerbose(int InVerbose) {
        Verbose = InVerbose;
        return *this;
    }
    Logger& SetCurrentVerbose(int InVerbose) {
        CurrentVerbose = InVerbose;
        return *this;
    }
    Logger& operator << (const char *Text) {
        if (Verbose < CurrentVerbose) return *this;
        if (IsLogging) { printf("\n"); }
        printf(Text);
        IsLogging = true;
        return *this;
    }
    Logger& operator << (int Number) {
        if (Verbose < CurrentVerbose) return *this;
        if (IsLogging) { printf("\n"); }
        printf("%d", Number);
        IsLogging = true;
        return *this;
    }
    Logger& operator << (float Decimal) {
        if (Verbose < CurrentVerbose) return *this;
        if (IsLogging) { printf("\n"); }
        printf("%f", Decimal);
        IsLogging = true;
        return *this;
    }
    Logger& operator << (Tag InTag) {
        if (Verbose < CurrentVerbose) return *this;
        if (IsLogging) { printf("\n"); }
        switch (InTag) {
        case Tag::NextLine:
            printf("\n");
            break;
        case Tag::Tab:
            for (int i = 0; i < TabSpace; i++) printf(" ");
            break;
        }
        IsLogging = true;
        return *this;
    }
    Logger& operator | (const char *Text) {
        if (Verbose < CurrentVerbose) return *this;
        printf(Text);
        IsLogging = true;
        return *this;
    }
    Logger& operator | (int Number) {
        if (Verbose < CurrentVerbose) return *this;
        printf("%d", Number);
        IsLogging = true;
        return *this;
    }
    Logger& operator | (float Decimal) {
        if (Verbose < CurrentVerbose) return *this;
        printf("%f", Decimal);
        IsLogging = true;
        return *this;
    }
    Logger& operator | (Tag InTag) {
        if (Verbose < CurrentVerbose) return *this;
        switch (InTag) {
        case Tag::NextLine:
            printf("\n");
            break;
        case Tag::Tab:
            for (int i = 0; i < TabSpace; i++) printf(" ");
            break;
        }
        IsLogging = true;
        return *this;
    }
    Logger& operator % (int Verbose) {
        if (Verbose < CurrentVerbose) return *this;
        SetCurrentVerbose(Verbose);
        return *this;
    }
};
extern Logger sLog;
