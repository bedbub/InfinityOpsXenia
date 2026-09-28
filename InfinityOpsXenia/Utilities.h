#pragma once
#include <stdio.h>
#include <xtl.h>

void CreateFilename(char* Output, const char* filename);
bool FileExists(const char* filename);
bool ResolveRawPath(char* Output, const char* filename);