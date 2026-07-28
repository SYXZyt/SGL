#pragma once
#include <SGL/SGL.h>
#include <SGL/Util/String.h>

SGL_BEGIN

static const char* sgl_ANSI_Reset = "\033[0m";

static const char* sgl_ANSI_Colours_Black = "\033[30m";
static const char* sgl_ANSI_Colours_Red = "\033[31m";
static const char* sgl_ANSI_Colours_Green = "\033[32m";
static const char* sgl_ANSI_Colours_Yellow = "\033[33m";
static const char* sgl_ANSI_Colours_Blue = "\033[34m";
static const char* sgl_ANSI_Colours_Magenta = "\033[35m";
static const char* sgl_ANSI_Colours_Cyan = "\033[36m";
static const char* sgl_ANSI_Colours_White = "\033[37m";

static const char* sgl_ANSI_Effect_Bold = "\033[1m";
static const char* sgl_ANSI_Effect_Underline = "\033[4m";
static const char* sgl_ANSI_Effect_Italic = "\033[3m";
static const char* sgl_ANSI_Effect_Dim = "\033[2m";
static const char* sgl_ANSI_Effect_Blink = "\033[5m";
static const char* sgl_ANSI_Effect_Reverse = "\033[7m";
static const char* sgl_ANSI_Effect_Hidden = "\033[8m";
static const char* sgl_ANSI_Effect_Strike = "\033[9m";

SGL_API extern sgl_String sgl_ANSI_MakeRGB(uint8 r, uint8 g, uint8 b);

SGL_END