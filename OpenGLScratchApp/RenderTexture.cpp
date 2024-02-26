#include "RenderTexture.h"

RenderTexture::RenderTexture()
{
    FBO = 0;
    textureID = 0;
    textureHeight = 0.0f;
    textureWidth = 0.0f;
}

bool RenderTexture::Init()
{
    return false;
}

void RenderTexture::BindFBO()
{
}

void RenderTexture::Write()
{
}

void RenderTexture::Read()
{
}

RenderTexture::~RenderTexture()
{
}
