#include "Texture.h"

Texture::Texture(std::string path, int spriteSheetWidth, int spriteSheetHeight, int textureWidth, int textureHeight , int nFrames , int nPerRow)
{

  mId = loadTexture(path);


  mSpriteSheetWidth = spriteSheetWidth;
  mSpriteSheetHeight = spriteSheetHeight;
  mTextureWidth = textureWidth;
  mTextureHeight = textureHeight;

  mNumberOfFrames = nFrames;

  mNumberPerRow = nPerRow;
}

Texture::Texture()
{

}

std::vector<float> Texture::getTextureVertices(int frameIndex)
{

  const float textureWidthRatio = static_cast<float>(mTextureWidth) / static_cast<float>(mSpriteSheetWidth);
  const float textureHeightRatio = static_cast<float>(mTextureHeight) / static_cast<float>(mSpriteSheetHeight);
  const int numberPerRow = mSpriteSheetWidth / mTextureWidth;

  float texPosX = (frameIndex % numberPerRow) * textureWidthRatio;
  float texPosY = (frameIndex / numberPerRow + 1) * textureHeightRatio;


  std::vector<float> textureCoords = {
    texPosX , texPosY , texPosX + textureWidthRatio , texPosY + textureHeightRatio
  };

  return textureCoords;
}

GLuint Texture::getTextureId()
{
  return mId;
}

int Texture::getNumberPerRow()
{
  return mNumberPerRow;
}