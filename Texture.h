#pragma once
#include "GameExtras.h"

class Texture {
  public:
    Texture(std::string path , int spriteSheetWidth , int spriteSheetHeight , int textureWidth , int textureHeight , int nFrames , int nPerRow);
    
    Texture();

    std::vector<float> getTextureVertices(int frameIndex);

    GLuint getTextureId();

    int getNumberPerRow();

  private:
    GLuint mId;
    int mSpriteSheetWidth, mSpriteSheetHeight;
    int mTextureWidth, mTextureHeight;
    int mNumberOfFrames;
    int mNumberPerRow;
};