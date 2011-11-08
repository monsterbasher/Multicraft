#include <Aurora/Graphics/win/OpenGLRenderManager.h>
#include <Aurora/Graphics/TextureManager.h>
#include "../stb_truetype.h"
#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <glew.h>

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

		OpengGLRenderManager::OpengGLRenderManager()
		{
			// default values
			_width = 640;
			_height = 480;
			_pov = 53;
			_vSync = true;
			_fullScreen = false;
			_zMin = 0.1f;
			_zMax = 256.0f;
		}

		void OpengGLRenderManager::Init()
		{
			// init glew
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
				fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			}

			// set opengl values
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glClearDepth(1.0f);
			glDepthFunc(GL_LEQUAL);
			glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
			glDisable(GL_TEXTURE_2D);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			_currentTexture = -1;
		}

		void OpengGLRenderManager::SetOrtho()
		{
			glViewport(0, 0, _width, _height);

			glMatrixMode(GL_PROJECTION);	
			glLoadIdentity();

			glOrtho(0, _width, _height, 0, _zMin, _zMax);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}

		void OpengGLRenderManager::SetTextOrtho()
		{
			glViewport(0, 0, _width, _height);

			glMatrixMode(GL_PROJECTION);	
			glLoadIdentity();

			glOrtho(0, _width, 0, _height, _zMin, _zMax);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}

		void OpengGLRenderManager::SetOrtho(float left,float right,float bottom,float top,float zmin,float zmax)
		{
			glViewport(0, 0, _width, _height);

			glMatrixMode(GL_PROJECTION);	
			glLoadIdentity();

			glOrtho(left, right, bottom, top, zmin, zmax);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}

		void OpengGLRenderManager::SetPerspective()
		{
			glViewport(0, 0, _width, _height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			gluPerspective(_pov,(GLfloat)_width/(GLfloat)_height,_zMin,_zMax);

			glMatrixMode(GL_MODELVIEW);	
			glLoadIdentity();
		}

		void OpengGLRenderManager::SetPerspective(float pov,float aspect,float zmin,float zmax)
		{
			glViewport(0, 0, _width, _height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			gluPerspective(pov,aspect,zmin,zmax);

			glMatrixMode(GL_MODELVIEW);	
			glLoadIdentity();
		}

		void OpengGLRenderManager::bindTexture(Image* image)
		{
			if(image->_id != _currentTexture)
			{
				glBindTexture(GL_TEXTURE_2D, image->_id);
				_currentTexture = image->_id;
			}			
		}

		void OpengGLRenderManager::bindTexture(std::string filename)
		{
			unsigned int id = TextureManager::Instance()->getImage(filename)->_id;

			if(id != _currentTexture)
			{
				glBindTexture(GL_TEXTURE_2D, id);
				_currentTexture = id;
			}
		}

		void OpengGLRenderManager::StartFrame()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();

		}

		void OpengGLRenderManager::EndFrame()
		{

		}

		void OpengGLRenderManager::_createTexture(Image* image)
		{
			if (image != 0)
			{
				glGenTextures(1, &image->_id);
				glBindTexture(GL_TEXTURE_2D, image->_id);
				gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image->_width, image->_height, GL_RGBA, GL_UNSIGNED_BYTE, image->_pixels);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_LINEAR
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
		}

		void OpengGLRenderManager::_createTexture(unsigned char* pixels,int width,int height,unsigned int &texId)
		{
			if(pixels != 0)
			{
				glGenTextures(1, &texId);
				glBindTexture(GL_TEXTURE_2D, texId);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

				delete [] pixels;
				pixels = 0;
			}
		}

		void OpengGLRenderManager::drawImage(Image* image)
		{
			bindTexture(image);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glColor4f(1.0f, 1.0f, 1.0f,1.0f);

			TexturedVertex *vertices = new TexturedVertex[4];
			vertices[0].u = 0.0f;vertices[0].v = 0.0f;vertices[0].x = 0;vertices[0].y = 0;vertices[0].z = 0.0f;
			vertices[1].u = 0.0f;vertices[1].v = 1.0f;vertices[1].x = 0;vertices[1].y = image->_height;vertices[1].z = 0.0f;
			vertices[2].u = 1.0f;vertices[2].v = 0.0f;vertices[2].x = image->_width;vertices[2].y = 0;vertices[2].z = 0.0f;
			vertices[3].u = 1.0f;vertices[3].v = 1.0f;vertices[3].x = image->_width = image->_height;vertices[3].z = 0.0f;

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glVertexPointer(3, GL_FLOAT, GLsizei(sizeof(TexturedVertex)), &vertices[0].x);
			glTexCoordPointer(2, GL_FLOAT, GLsizei(sizeof(TexturedVertex)), &vertices[0].u);

			glDrawArrays(GL_TRIANGLE_STRIP,0,4);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);

			delete [] vertices;
		}

		void OpengGLRenderManager::drawImage(Image* image,int posx,int posy)
		{
			bindTexture(image);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glColor4f(1.0f, 1.0f, 1.0f,1.0f);

			TexturedVertex *vertices = new TexturedVertex[4];
			vertices[0].u = 0.0f;vertices[0].v = 0.0f;vertices[0].x = posx;vertices[0].y = posy;vertices[0].z = 0.0f;
			vertices[1].u = 0.0f;vertices[1].v = 1.0f;vertices[1].x = posx;vertices[1].y = posy+image->_height;vertices[1].z = 0.0f;
			vertices[2].u = 1.0f;vertices[2].v = 0.0f;vertices[2].x = posx+image->_width;vertices[2].y = posy;vertices[2].z = 0.0f;
			vertices[3].u = 1.0f;vertices[3].v = 1.0f;vertices[3].x = posx+image->_width;vertices[3].y = posy+image->_height;vertices[3].z = 0.0f;

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glVertexPointer(3, GL_FLOAT, GLsizei(sizeof(TexturedVertex)), &vertices[0].x);
			glTexCoordPointer(2, GL_FLOAT, GLsizei(sizeof(TexturedVertex)), &vertices[0].u);

			glDrawArrays(GL_TRIANGLE_STRIP,0,4);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);

			delete [] vertices;
		}

		void OpengGLRenderManager::drawImage(Image* image,int posx,int posy,int width,int height)
		{
			bindTexture(image);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glColor4f(1.0f, 1.0f, 1.0f,1.0f);

			TexturedVertex *vertices = new TexturedVertex[4];
			vertices[0].u = 0.0f;vertices[0].v = 0.0f;vertices[0].x = posx;vertices[0].y = posy;vertices[0].z = 0.0f;
			vertices[1].u = 0.0f;vertices[1].v = 1.0f;vertices[1].x = posx;vertices[1].y = posy+height;vertices[1].z = 0.0f;
			vertices[2].u = 1.0f;vertices[2].v = 0.0f;vertices[2].x = posx+width;vertices[2].y = posy;vertices[2].z = 0.0f;
			vertices[3].u = 1.0f;vertices[3].v = 1.0f;vertices[3].x = posx+width;vertices[3].y = posy+height;vertices[3].z = 0.0f;

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glVertexPointer(3, GL_FLOAT, GLsizei(sizeof(TexturedVertex)), &vertices[0].x);
			glTexCoordPointer(2, GL_FLOAT, GLsizei(sizeof(TexturedVertex)), &vertices[0].u);

			glDrawArrays(GL_TRIANGLE_STRIP,0,4);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);

			delete [] vertices;
		}

		void OpengGLRenderManager::drawSprite(Sprite* sprite)
		{
			glPushMatrix();

			glTranslatef(sprite->posX,sprite->posY,0.0f);

			bindTexture(sprite->imageName);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glColor3f(1,1,1);

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glVertexPointer(3, GL_FLOAT, GLsizei(sizeof(TexturedVertex)), &sprite->vertices[0].x);
			glTexCoordPointer(2, GL_FLOAT, GLsizei(sizeof(TexturedVertex)), &sprite->vertices[0].u);

			glDrawArrays(GL_TRIANGLE_STRIP,0,4);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			glPopMatrix();
		}

		void OpengGLRenderManager::drawSprite3D(Sprite3D* sprite)
		{
			glPushMatrix();

			glTranslatef(sprite->posX,sprite->posY,sprite->posZ);
			glRotatef(sprite->rotationX,1.0f,0.0f,0.0f);
			glRotatef(sprite->rotationY,0.0f,1.0f,0.0f);
			glRotatef(sprite->rotationZ,0.0f,0.0f,1.0f);

			int posx = 0;
			int posy = 0;

			for (int y = sprite->height/2;y > -sprite->height/2;y--)
			{
				for (int x = -sprite->width/2;x < sprite->width/2;x++)
				{
					unsigned int colour = sprite->colours[posx + posy*sprite->height];
					if(colour != 0)
						drawCube(colour,Math::Vector3(x,y,0),Math::Vector3(1,1,1),Math::Vector3(0,0,0));
					posx++;
				}
				posx = 0;
				posy++;
			}

			glPopMatrix();
		}

		float g_tabStops2[4] = {150, 210, 270, 330};

		void OpengGLRenderManager::drawText(TrueTypeFont* font,float x, float y, const char *text, int align, unsigned int col)
		{
			if (!font->g_ftex) return;
			if (!text) return;

			y = _height - y;

			if (align == Graphics::ALIGN_CENTER)
				x -= font->getTextLength(font->g_cdata, text)/2;
			else if (align == Graphics::ALIGN_RIGHT)
				x -= font->getTextLength(font->g_cdata, text);

			glColor4ub(col&0xff, (col>>8)&0xff, (col>>16)&0xff, (col>>24)&0xff);
			glEnable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);

			// assume orthographic projection with units = screen pixels, origin at top left
			glBindTexture(GL_TEXTURE_2D, font->g_ftex);
			_currentTexture = font->g_ftex;
			const float ox = x;

			TexturedVertex *vertices = new TexturedVertex[strlen(text)*6];
			int ver = 0;

			while (*text)
			{
				int c = (unsigned char)*text;
				if (c == '\t')
				{
					for (int i = 0; i < 4; ++i)
					{
						if (x < g_tabStops2[i]+ox)
						{
							x = g_tabStops2[i]+ox;
							break;
						}
					}
				}
				else if (c >= 32 && c < 128)
				{                       
					stbtt_aligned_quad2 q;
					font->getBakedQuad(font->g_cdata, 256,256, c-32, &x,&y,&q);

					vertices[ver].u = q.s0;vertices[ver].v = q.t0;vertices[ver].x = q.x0;vertices[ver].y = q.y0;vertices[ver].z = 0.0f;ver++;
					vertices[ver].u = q.s1;vertices[ver].v = q.t1;vertices[ver].x = q.x1;vertices[ver].y = q.y1;vertices[ver].z = 0.0f;ver++;
					vertices[ver].u = q.s1;vertices[ver].v = q.t0;vertices[ver].x = q.x1;vertices[ver].y = q.y0;vertices[ver].z = 0.0f;ver++;

					vertices[ver].u = q.s0;vertices[ver].v = q.t0;vertices[ver].x = q.x0;vertices[ver].y = q.y0;vertices[ver].z = 0.0f;ver++;
					vertices[ver].u = q.s0;vertices[ver].v = q.t1;vertices[ver].x = q.x0;vertices[ver].y = q.y1;vertices[ver].z = 0.0f;ver++;
					vertices[ver].u = q.s1;vertices[ver].v = q.t1;vertices[ver].x = q.x1;vertices[ver].y = q.y1;vertices[ver].z = 0.0f;ver++;
				}
				++text;
			}

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glVertexPointer(3, GL_FLOAT, GLsizei(sizeof(TexturedVertex)), &vertices[0].x);
			glTexCoordPointer(2, GL_FLOAT, GLsizei(sizeof(TexturedVertex)), &vertices[0].u);

			glDrawArrays(GL_TRIANGLES,0,ver);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			        
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);

			delete [] vertices;
		}

		//shapes
		void OpengGLRenderManager::drawCube(unsigned int color,Math::Vector3 position,Math::Vector3 scale,Math::Vector3 rotation)
		{
			glPushMatrix();

			glRotatef(rotation.x,1.0f,0.0f,0.0f);
			glRotatef(rotation.y,0.0f,1.0f,0.0f);
			glRotatef(rotation.z,0.0f,0.0f,1.0f);
			glTranslatef(position.x,position.y,position.z);

			glColor4ub(color&0xff, (color>>8)&0xff, (color>>16)&0xff, (color>>24)&0xff);

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, cubeVert);

			glDrawArrays(GL_TRIANGLES,0,36);

			glDisableClientState(GL_VERTEX_ARRAY);
						
			glPopMatrix();
		}
	}
}