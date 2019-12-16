#pragma once
#include <glad/glad.h>
#include "Window.h"

void bindFrameBuffer(int framebuffer, int width, int height);

void unbindCurrentFrameBuffer(int scrWidth, int scrHeight);
void unbindCurrentFrameBuffer();

unsigned int createFrameBuffer();

unsigned int createTextureAttachment(int width, int height);

unsigned int * createColorAttachments(int width, int height, unsigned int nColorAttachments);

unsigned int createDepthTextureAttachment(int width, int height);

unsigned int createDepthBufferAttachment(int width, int height);

unsigned int createRenderBufferAttachment(int width, int height);

class FrameBufferObject
{
public:
	FrameBufferObject(int W_, int H_);
	FrameBufferObject(int W_, int H_, int numColorAttachments);
	unsigned int getColorAttachmentTex(int i);
	void bind();

public:
	unsigned int FBO, renderBuffer, depthTex;
	unsigned int tex;

private:
	int W, H;
	int nColorAttachments;
	unsigned int * colorAttachments;
};

// 用于绘制计算着色器
class TextureSet
{
public:
	TextureSet(int W, int H, int num);
	void bindTexture(int i, int unit);
	unsigned int getColorAttachmentTex(int i);

	void bind();

	int getNTextures() const
	{
		return nTextures;
	}

private:
	int nTextures;
	unsigned int * texture;
};