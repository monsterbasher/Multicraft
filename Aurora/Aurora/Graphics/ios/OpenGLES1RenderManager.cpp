#include <Aurora/Graphics/ios/OpenGLES1RenderManager.h>
#include <Aurora/Graphics/TextureManager.h>
#include "../stb_truetype.h"



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace Aurora
{
	namespace Graphics
	{
		float cubeVert[] = {
			-0.500000f, -0.500000f, -0.500000f,  -0.500000f, 0.500000f, -0.500000f,  0.500000f, 0.500000f, -0.500000f,
			-0.500000f, -0.500000f, -0.500000f,  0.500000f, 0.500000f, -0.500000f,  0.500000f, -0.500000f, -0.500000f,
			-0.500000f, -0.500000f, -0.500000f,  0.500000f, -0.500000f, -0.500000f,  0.500000f, -0.500000f, 0.500000f,
			-0.500000f, -0.500000f, -0.500000f,  0.500000f, -0.500000f, 0.500000f,  -0.500000f, -0.500000f, 0.500000f,
			-0.500000f, -0.500000f, -0.500000f,  -0.500000f, -0.500000f, 0.500000f,  -0.500000f, 0.500000f, 0.500000f,
			-0.500000f, -0.500000f, -0.500000f,  -0.500000f, 0.500000f, 0.500000f,  -0.500000f, 0.500000f, -0.500000f,
			0.500000f, -0.500000f, -0.500000f,  0.500000f, 0.500000f, -0.500000f,  0.500000f, 0.500000f, 0.500000f,
			0.500000f, -0.500000f, -0.500000f,  0.500000f, 0.500000f, 0.500000f,  0.500000f, -0.500000f, 0.500000f,
			-0.500000f, 0.500000f, -0.500000f,  -0.500000f, 0.500000f, 0.500000f,  0.500000f, 0.500000f, 0.500000f,
			-0.500000f, 0.500000f, -0.500000f,  0.500000f, 0.500000f, 0.500000f,  0.500000f, 0.500000f, -0.500000f,
			-0.500000f, -0.500000f, 0.500000f,  0.500000f, -0.500000f, 0.500000f,  0.500000f, 0.500000f, 0.500000f,
			-0.500000f, -0.500000f, 0.500000f,  0.500000f, 0.500000f, 0.500000f,  -0.500000f, 0.500000f, 0.500000f,
		};

		float cubeST[] = {
			1.000000f, 0.000000f,  1.000000f, 1.000000f,  0.000000f, 1.000000f, 
			1.000000f, 0.000000f,  0.000000f, 1.000000f,  0.000000f, 0.000000f, 
			0.000000f, 0.000000f,  1.000000f, 0.000000f,  1.000000f, 1.000000f, 
			0.000000f, 0.000000f,  1.000000f, 1.000000f,  0.000000f, 1.000000f, 
			0.000000f, 0.000000f,  1.000000f, 0.000000f,  1.000000f, 1.000000f, 
			0.000000f, 0.000000f,  1.000000f, 1.000000f,  0.000000f, 1.000000f, 
			1.000000f, 0.000000f,  1.000000f, 1.000000f,  0.000000f, 1.000000f, 
			1.000000f, 0.000000f,  0.000000f, 1.000000f,  0.000000f, 0.000000f, 
			0.000000f, 1.000000f,  0.000000f, 0.000000f,  1.000000f, 0.000000f, 
			0.000000f, 1.000000f,  1.000000f, 0.000000f,  1.000000f, 1.000000f, 
			0.000000f, 0.000000f,  1.000000f, 0.000000f,  1.000000f, 1.000000f, 
			0.000000f, 0.000000f,  1.000000f, 1.000000f,  0.000000f, 1.000000f, 
		};

		OpenGLES1RenderManager::OpenGLES1RenderManager()
		{
			// default values
			_width = 640;
			_height = 480;
			_pov = 53;
			_vSync = true;
			_fullScreen = false;
			_zOtrhoMin = -10;
			_zOtrhoMax = 10;
			_zMin = 0.1f;
			_zMax = 256.0f;
		}

		void OpenGLES1RenderManager::Init()
		{
            // Create & bind the color buffer so that the caller can allocate its space.
            glGenRenderbuffersOES(1, &m_colorRenderbuffer);
            glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_colorRenderbuffer);
            
            // Create the depth buffer.
            glGenRenderbuffersOES(1, &m_depthRenderbuffer);
            glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_depthRenderbuffer);
            glRenderbufferStorageOES(GL_RENDERBUFFER_OES,GL_DEPTH_COMPONENT16_OES,_width,_height);
            
            // Create the framebuffer object; attach the depth and color buffers.
            glGenFramebuffersOES(1, &m_framebuffer);
            glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_framebuffer);
            glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES,
                                         GL_COLOR_ATTACHMENT0_OES,
                                         GL_RENDERBUFFER_OES,
                                         m_colorRenderbuffer);
            
            glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES,
                                         GL_DEPTH_ATTACHMENT_OES,
                                         GL_RENDERBUFFER_OES,
                                         m_depthRenderbuffer);
            
            // Bind the color buffer for rendering.
            glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_colorRenderbuffer);
		}

		void OpenGLES1RenderManager::SetOrtho()
		{

		}

		void OpenGLES1RenderManager::SetTextOrtho()
		{

		}

		void OpenGLES1RenderManager::SetOrtho(float left,float right,float bottom,float top,float zmin,float zmax)
		{

		}

		void OpenGLES1RenderManager::SetPerspective()
		{

		}

		void OpenGLES1RenderManager::SetPerspective(float pov,float aspect,float zmin,float zmax)
		{

		}

		void OpenGLES1RenderManager::bindTexture(int id)
		{

		}

		void OpenGLES1RenderManager::bindTexture(Image* image)
		{
		
		}

		void OpenGLES1RenderManager::bindTexture(std::string filename)
		{

		}

		void OpenGLES1RenderManager::UpdateCurrentCamera()
		{

		}

		void OpenGLES1RenderManager::ClearScreen()
		{

		}

		void OpenGLES1RenderManager::StartFrame()
		{

		}

		void OpenGLES1RenderManager::EndFrame()
		{

		}

		void OpenGLES1RenderManager::_createTexture(Image* image)
		{

		}

		void OpenGLES1RenderManager::_createTexture(unsigned char* pixels,int width,int height,unsigned int &texId)
		{

		}

		void OpenGLES1RenderManager::_createEmptyTexture( Image* image, ImageLocation location )
		{

		}

		void OpenGLES1RenderManager::drawImage(Image* image)
		{

		}

		void OpenGLES1RenderManager::drawImage(Image* image,int posx,int posy)
		{

		}

		void OpenGLES1RenderManager::drawImage(Image* image,int posx,int posy,int width,int height)
		{

		}

		void OpenGLES1RenderManager::drawSprite(Sprite* sprite)
		{

		}

		void OpenGLES1RenderManager::drawSprite3D(Sprite3D* sprite)
		{

		}

		float g_tabStops2[4] = {150, 210, 270, 330};

		void OpenGLES1RenderManager::drawText(TrueTypeFont* font,float x, float y, const char *text, int align, unsigned int col)
		{

		}

		void OpenGLES1RenderManager::StartRenderToTexture(Image* texture)
		{

		}

		void OpenGLES1RenderManager::EndRenderToTexture(Image* texture)
		{

		}

		void OpenGLES1RenderManager::RenderToScreen()
		{
			//in opegl case just set again perspective
			SetPerspective();
		}

		//shapes
		void OpenGLES1RenderManager::drawCube(unsigned int color,Math::Vector3 position,Math::Vector3 scale,Math::Vector3 rotation)
		{

		}

		void OpenGLES1RenderManager::DrawCubeTextured(Image* texture,Math::Vector3 position,Math::Vector3 scale,Math::Vector3 rotation)
		{

		}

		void OpenGLES1RenderManager::_createModelObjVertices(ModelObj* model)
		{

		}

		void OpenGLES1RenderManager::DrawModelObj(ModelObj *model,Math::Vector3 position,Math::Vector3 scale,Math::Vector3 rotation)
		{

		}
	}
}
