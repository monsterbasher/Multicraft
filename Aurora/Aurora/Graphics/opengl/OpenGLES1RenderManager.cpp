#include <Aurora/Graphics/opengl/OpenGLES1RenderManager.h>
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
        
        DeviceOrientation OpenGLES1RenderManager::_deviceOrientation = DeviceOrientationUnknown;
        int OpenGLES1RenderManager::deviceRotationAngle = 0;
        
        void OpenGLES1RenderManager::setDeviceOrientation(DeviceOrientation orientation)
        {
            _deviceOrientation = orientation;
            
            if (_deviceOrientation == DeviceOrientationLandscapeRight)
            {
                deviceRotationAngle = 90;
                
            }else if (_deviceOrientation == DeviceOrientationLandscapeLeft)
            {
                deviceRotationAngle = 270;
                
            }else if (_deviceOrientation == DeviceOrientationPortraitUpsideDown)
            {
                deviceRotationAngle = 180;
                
            }else
            {
                deviceRotationAngle = 0;
            }
        }

		OpenGLES1RenderManager::OpenGLES1RenderManager()
		{
			// default values
			_width = 480;
			_height = 320;
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
			_generateOpenglBuffers();

            // set opengl values
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			//glClearDepth(1.0f);
			glDepthFunc(GL_LEQUAL);
			glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
			glDisable(GL_TEXTURE_2D);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
			_currentTexture = -1;
		}

		void OpenGLES1RenderManager::_generateOpenglBuffers()
		{
			#ifdef AURORA_IOS

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

			#endif
		}

		void OpenGLES1RenderManager::SetOrtho()
		{
            glViewport(0, 0, _width, _height);
            
			glMatrixMode(GL_PROJECTION);	
			glLoadIdentity();
            
            if (_deviceOrientation == DeviceOrientationLandscapeRight)
            {
                glRotatef(90, 0, 0, 1);
                glOrthof(0, _height, _width, 0, _zOtrhoMin, _zOtrhoMax);
                
            }else if (_deviceOrientation == DeviceOrientationLandscapeLeft)
            {
                glRotatef(-90, 0, 0, 1);
                glOrthof(0, _height, _width, 0, _zOtrhoMin, _zOtrhoMax);
                
            }else if (_deviceOrientation == DeviceOrientationPortraitUpsideDown)
            {
                glRotatef(180, 0, 0, 1);
                glOrthof(0, _width, _height, 0, _zOtrhoMin, _zOtrhoMax);
                
            }else
            {
                glOrthof(0, _width, _height, 0, _zOtrhoMin, _zOtrhoMax);
            }
            
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}

		void OpenGLES1RenderManager::SetTextOrtho()
		{
            glViewport(0, 0, _width, _height);
            
            glMatrixMode(GL_PROJECTION);	
            glLoadIdentity();
            
            if (_deviceOrientation == DeviceOrientationLandscapeRight)
            {
                glRotatef(90, 0, 0, 1);
                glOrthof(0, _height, 0, _width, _zOtrhoMin, _zOtrhoMax);
                glTranslatef(0, -(_width / 2), 0);
                
            }else if (_deviceOrientation == DeviceOrientationLandscapeLeft)
            {
                glRotatef(-90, 0, 0, 1);
                glOrthof(0, _height, 0, _width, _zOtrhoMin, _zOtrhoMax);
                glTranslatef(0, -(_width / 2), 0);
                
            }else if (_deviceOrientation == DeviceOrientationPortraitUpsideDown)
            {
                glRotatef(180, 0, 0, 1);
                glOrthof(0, _width, 0, _height, _zOtrhoMin, _zOtrhoMax);
                
            }else
            {
                glOrthof(0, _width, 0, _height, _zOtrhoMin, _zOtrhoMax);
            }
            
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }

		void OpenGLES1RenderManager::SetOrtho(float left,float right,float bottom,float top,float zmin,float zmax)
		{
            glViewport(0, 0, _width, _height);
            
			glMatrixMode(GL_PROJECTION);	
			glLoadIdentity();
            
            if (_deviceOrientation == DeviceOrientationLandscapeRight)
            {
                glRotatef(90, 0, 0, 1);
                glOrthof(left, bottom, right, top, _zOtrhoMin, _zOtrhoMax);
                
            }else if (_deviceOrientation == DeviceOrientationLandscapeLeft)
            {
                glRotatef(-90, 0, 0, 1);
                glOrthof(left, bottom, right, top, _zOtrhoMin, _zOtrhoMax);
                
            }else if (_deviceOrientation == DeviceOrientationPortraitUpsideDown)
            {
                glRotatef(180, 0, 0, 1);
                glOrthof(left, right, bottom, top, _zOtrhoMin, _zOtrhoMax);
                
            }else
            {
                glOrthof(left, right, bottom, top, _zOtrhoMin, _zOtrhoMax);
            }
            
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
        
        void OpenGLES1RenderManager::__gluMakeIdentityf(GLfloat m[16])
        {
            m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
            m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
            m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
            m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
        }
        
        void OpenGLES1RenderManager::__normalize(GLfloat v[3])
        {
            GLfloat r;
            
            r=(GLfloat)sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            if (r==0.0f)
            {
                return;
            }
            
            v[0]/=r;
            v[1]/=r;
            v[2]/=r;
        }
        
        void OpenGLES1RenderManager::__cross(GLfloat v1[3], GLfloat v2[3], GLfloat result[3])
        {
            result[0] = v1[1]*v2[2] - v1[2]*v2[1];
            result[1] = v1[2]*v2[0] - v1[0]*v2[2];
            result[2] = v1[0]*v2[1] - v1[1]*v2[0];
        }
        
        void OpenGLES1RenderManager::gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
        {
            GLfloat m[4][4];
            GLfloat sine, cotangent, deltaZ;
            GLfloat radians = fovy / 2 * 3.14 / 180;
            
            deltaZ = zFar - zNear;
            sine = sin(radians);
            if ((deltaZ == 0) || (sine == 0) || (aspect == 0))
            {
                return;
            }
            cotangent = cos(radians) / sine;
            
            __gluMakeIdentityf(&m[0][0]);
            m[0][0] = cotangent / aspect;
            m[1][1] = cotangent;
            m[2][2] = -(zFar + zNear) / deltaZ;
            m[2][3] = -1;
            m[3][2] = -2 * zNear * zFar / deltaZ;
            m[3][3] = 0;
            glMultMatrixf(&m[0][0]);
        }
        
        void OpenGLES1RenderManager::gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx,
                       GLfloat centery, GLfloat centerz, GLfloat upx, GLfloat upy,GLfloat upz)
        {
            GLfloat forward[3], side[3], up[3];
            GLfloat m[4][4];
            
            forward[0] = centerx - eyex;
            forward[1] = centery - eyey;
            forward[2] = centerz - eyez;
            
            up[0] = upx;
            up[1] = upy;
            up[2] = upz;
            
            __normalize(forward);
            
            /* Side = forward x up */
            __cross(forward, up, side);
            __normalize(side);
            
            /* Recompute up as: up = side x forward */
            __cross(side, forward, up);
            
            __gluMakeIdentityf(&m[0][0]);
            m[0][0] = side[0];
            m[1][0] = side[1];
            m[2][0] = side[2];
            
            m[0][1] = up[0];
            m[1][1] = up[1];
            m[2][1] = up[2];
            
            m[0][2] = -forward[0];
            m[1][2] = -forward[1];
            m[2][2] = -forward[2];
            
            glMultMatrixf(&m[0][0]);
            glTranslatef(-eyex, -eyey, -eyez);
        }

		void OpenGLES1RenderManager::SetPerspective()
		{
            glViewport(0, 0, _width, _height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
            
			gluPerspective(_pov,(GLfloat)_width/(GLfloat)_height,_zMin,_zMax);
            
            if (_deviceOrientation == DeviceOrientationLandscapeRight)
            {
                glRotatef(90, 0, 0, 1);
            }else if (_deviceOrientation == DeviceOrientationLandscapeLeft)
            {
                glRotatef(-90, 0, 0, 1);
            }else if (_deviceOrientation == DeviceOrientationPortraitUpsideDown)
            {
                glRotatef(180, 0, 0, 1);
            }
            
			glMatrixMode(GL_MODELVIEW);	
			glLoadIdentity();
		}

		void OpenGLES1RenderManager::SetPerspective(float pov,float aspect,float zmin,float zmax)
		{
            glViewport(0, 0, _width, _height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
            
			gluPerspective(pov,aspect,zmin,zmax);
            
            if (_deviceOrientation == DeviceOrientationLandscapeRight)
            {
                glRotatef(90, 0, 0, 1);
            }else if (_deviceOrientation == DeviceOrientationLandscapeLeft)
            {
                glRotatef(-90, 0, 0, 1);
            }else if (_deviceOrientation == DeviceOrientationPortraitUpsideDown)
            {
                glRotatef(180, 0, 0, 1);
            }
            
			glMatrixMode(GL_MODELVIEW);	
			glLoadIdentity();
		}

		void OpenGLES1RenderManager::bindTexture(int id)
		{
            if(id != _currentTexture)
			{
				glBindTexture(GL_TEXTURE_2D, id);
				_currentTexture = id;
			}
		}

		void OpenGLES1RenderManager::bindTexture(Image* image)
		{
            if(image->_id != _currentTexture)
			{
				glBindTexture(GL_TEXTURE_2D, image->_id);
				_currentTexture = image->_id;
			}
		}

		void OpenGLES1RenderManager::bindTexture(std::string filename)
		{
            unsigned int id = TextureManager::Instance()->getImage(filename)->_id;
            
			if(id != _currentTexture)
			{
				glBindTexture(GL_TEXTURE_2D, id);
				_currentTexture = id;
			}
		}

		void OpenGLES1RenderManager::UpdateCurrentCamera()
		{
            if(_currentCam != 0)
			{
				gluLookAt(_currentCam->m_vPosition.x + _currentCam->m_vOffset.x,_currentCam->m_vPosition.y + _currentCam->m_vOffset.y,_currentCam->m_vPosition.z + _currentCam->m_vOffset.z,
                    _currentCam->m_vView.x,_currentCam->m_vView.y,_currentCam->m_vView.z,
                    _currentCam->m_vUpVector.x,_currentCam->m_vUpVector.y,_currentCam->m_vUpVector.z);
			}
		}

		void OpenGLES1RenderManager::ClearScreen()
		{
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
		}

		void OpenGLES1RenderManager::StartFrame()
		{
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
		}

		void OpenGLES1RenderManager::EndFrame()
		{

		}

		void OpenGLES1RenderManager::_createTexture(Image* image)
		{
            if (image != 0)
			{
				glGenTextures(1, &image->_id);
				glBindTexture(GL_TEXTURE_2D, image->_id);
				//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image->_width, image->_height, GL_RGBA, GL_UNSIGNED_BYTE, image->_pixels);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->_width, image->_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->_pixels);
                
				//clamp dong like 3d models
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_LINEAR
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
		}

		void OpenGLES1RenderManager::_createTexture(unsigned char* pixels,int width,int height,unsigned int &texId)
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

		void OpenGLES1RenderManager::_createEmptyTexture( Image* image, ImageLocation location )
		{
            if (image != 0)
			{
				glGenTextures(1, &image->_id);
				glBindTexture(GL_TEXTURE_2D, image->_id);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->_width,image->_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
		}
		
		void OpenGLES1RenderManager::_extractFrustumPlanes(Frustum *frustum)
		{
		
		}

		void OpenGLES1RenderManager::drawImage(Image* image)
		{
            bindTexture(image);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glColor4f(1.0f, 1.0f, 1.0f,1.0f);
            
			TexturedVertex *vertices = new TexturedVertex[4];
			vertices[0].u = 0.0f;vertices[0].v = 0.0f;vertices[0].x = 0.0f;vertices[0].y = 0.0f;vertices[0].z = 0.0f;
			vertices[1].u = 0.0f;vertices[1].v = 1.0f;vertices[1].x = 0.0f;vertices[1].y = image->_height;vertices[1].z = 0.0f;
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

		void OpenGLES1RenderManager::drawImage(Image* image,int posx,int posy)
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

		void OpenGLES1RenderManager::drawImage(Image* image,int posx,int posy,int width,int height)
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

		void OpenGLES1RenderManager::drawSprite(Sprite* sprite)
		{
            glPushMatrix();
            
			glTranslatef(sprite->posX,sprite->posY,0.0f);
            
			bindTexture(sprite->imageName);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glColor4f(1.0f, 1.0f, 1.0f,1.0f);
            
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

		void OpenGLES1RenderManager::drawSprite3D(Sprite3D* sprite)
		{
            glPushMatrix();
            
			glTranslatef(sprite->posX,sprite->posY,sprite->posZ);
			glRotatef(sprite->rotationX,1.0f,0.0f,0.0f);
			glRotatef(sprite->rotationY,0.0f,1.0f,0.0f);
			glRotatef(sprite->rotationZ,0.0f,0.0f,1.0f);
			glScalef(sprite->scaleX,sprite->scaleY,sprite->scaleZ);
            
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

		void OpenGLES1RenderManager::drawText(TrueTypeFont* font,float x, float y, const char *text, int align, unsigned int col)
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

		void OpenGLES1RenderManager::StartRenderToTexture(Image* texture)
		{
            glViewport(0, 0, texture->_width, texture->_height);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
            
			gluPerspective(_pov, texture->_width/texture->_height,_zMin,_zMax);
            
			glMatrixMode(GL_MODELVIEW);	
			glLoadIdentity();
            
			// for GL_DRAW_BUFFER and GL_READ_BUFFER
//todo		//glPushAttrib(GL_COLOR_BUFFER_BIT | GL_PIXEL_MODE_BIT); 
			//glDrawBuffer(GL_BACK);
			//glReadBuffer(GL_BACK);
            
			//now render all stuff you want
		}

		void OpenGLES1RenderManager::EndRenderToTexture(Image* texture)
		{
            // copy the frame buffer pixels to a texture
			glBindTexture(GL_TEXTURE_2D, texture->_id);
			glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0,texture->_width, texture->_height);
			glBindTexture(GL_TEXTURE_2D, 0);
            
			// GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
			//glPopAttrib();
		}

		void OpenGLES1RenderManager::RenderToScreen()
		{
			//in opegl case just set again perspective
			SetPerspective();
		}

		//shapes
		void OpenGLES1RenderManager::drawCube(unsigned int color,Math::Vector3 position,Math::Vector3 scale,Math::Vector3 rotation)
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

		void OpenGLES1RenderManager::DrawCubeTextured(Image* texture,Math::Vector3 position,Math::Vector3 scale,Math::Vector3 rotation)
		{
            glPushMatrix();
            
			glRotatef(rotation.x,1.0f,0.0f,0.0f);
			glRotatef(rotation.y,0.0f,1.0f,0.0f);
			glRotatef(rotation.z,0.0f,0.0f,1.0f);
			glTranslatef(position.x,position.y,position.z);
            
			glColor4ub(255, 255, 255, 255);
            
			glBindTexture(GL_TEXTURE_2D, texture->_id);
			glEnable(GL_TEXTURE_2D);
            
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            
			glTexCoordPointer(2,GL_FLOAT,0, cubeST);
			glVertexPointer(3, GL_FLOAT, 0, cubeVert);
            
			glDrawArrays(GL_TRIANGLES,0,36);
            
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            
			glDisable(GL_TEXTURE_2D);
            
			glPopMatrix();
		}

		void OpenGLES1RenderManager::_createModelObjVertices(ModelObj* model)
		{
            for (unsigned int i = 0; i < model->mMeshes.size();i++)
			{
				ObjMesh *mesh = model->mMeshes[i];
                
				std::vector<TexturesPSPVertex> temp;
				std::vector<int> tempIndices;
                
				for(unsigned int f = 0;f < mesh->mFace.size();f++)
				{
					//create temp vertex
					TexturesPSPVertex vert;
					vert.u = model->allUVMap[mesh->mUVFace[f].x].u;
					vert.v = model->allUVMap[mesh->mUVFace[f].x].v;
					vert.x = model->allVertex[mesh->mFace[f].x].x;
					vert.y = model->allVertex[mesh->mFace[f].x].y;
					vert.z = model->allVertex[mesh->mFace[f].x].z;
                    
					//check if exist
					bool found = false;
					for(unsigned int t = 0;t < temp.size();t++)
					{
						if(temp[t].u == vert.u && temp[t].v == vert.v && temp[t].x == vert.x && temp[t].y == vert.y && temp[t].z == vert.z)
						{
							found = true;
							tempIndices.push_back(t);
							break;
						}
					}
                    
					if (found == false)
					{
						mesh->aabb.expandToInclude(model->allVertex[mesh->mFace[f].x]);
						temp.push_back(vert);
						tempIndices.push_back(temp.size()-1);
					}
                    
					//////////////////////////////////////////////////////////////////////////
					vert.u = model->allUVMap[mesh->mUVFace[f].y].u;
					vert.v = model->allUVMap[mesh->mUVFace[f].y].v;
					vert.x = model->allVertex[mesh->mFace[f].y].x;
					vert.y = model->allVertex[mesh->mFace[f].y].y;
					vert.z = model->allVertex[mesh->mFace[f].y].z;
                    
					//check if exist
					found = false;
					for(unsigned int t = 0;t < temp.size();t++)
					{
						if(temp[t].u == vert.u && temp[t].v == vert.v && temp[t].x == vert.x && temp[t].y == vert.y && temp[t].z == vert.z)
						{
							found = true;
							tempIndices.push_back(t);
							break;
						}
					}
                    
					if (found == false)
					{
						mesh->aabb.expandToInclude(model->allVertex[mesh->mFace[f].y]);
						temp.push_back(vert);
						tempIndices.push_back(temp.size()-1);
					}
                    
					//////////////////////////////////////////////////////////////////////////
					vert.u = model->allUVMap[mesh->mUVFace[f].z].u;
					vert.v = model->allUVMap[mesh->mUVFace[f].z].v;
					vert.x = model->allVertex[mesh->mFace[f].z].x;
					vert.y = model->allVertex[mesh->mFace[f].z].y;
					vert.z = model->allVertex[mesh->mFace[f].z].z;
                    
					//check if exist
					found = false;
					for(unsigned int t = 0;t < temp.size();t++)
					{
						if(temp[t].u == vert.u && temp[t].v == vert.v && temp[t].x == vert.x && temp[t].y == vert.y && temp[t].z == vert.z)
						{
							found = true;
							tempIndices.push_back(t);
							break;
						}
					}
                    
					if (found == false)
					{
						mesh->aabb.expandToInclude(model->allVertex[mesh->mFace[f].z]);
						temp.push_back(vert);
						tempIndices.push_back(temp.size()-1);
					}
				}
                
				//copy temp vertices and indieces
				mesh->meshVertices = new TexturesPSPVertex[temp.size()];
				for(int v = 0;v < temp.size();v++)
				{
					memcpy(&mesh->meshVertices[v],&temp[v],sizeof(TexturesPSPVertex));
				}
                
				mesh->indices = new unsigned short[tempIndices.size()];
				for(int v = 0;v < tempIndices.size();v++)
				{
					memcpy(&mesh->indices[v],&tempIndices[v],sizeof(unsigned short));
				}
                
				mesh->triangleCount = mesh->mFace.size();
				mesh->vertexCount = temp.size();
				mesh->indicesCount = tempIndices.size();
                
				//try optimize indieces for cashe misses
				/*VertexCache vertex_cache;
                 VertexCacheOptimizer vco;
                 
                 int misses = vertex_cache.GetCacheMissCount(mesh->indices,mesh->triangleCount);
                 
                 VertexCacheOptimizer::Result res = vco.Optimize(mesh->indices,mesh->triangleCount);
                 int misses2 = vertex_cache.GetCacheMissCount(mesh->indices,mesh->triangleCount);*/
                
				//create triangle strips :D
				/*STRIPERCREATE sc;
                 
                 sc.DFaces			= (unsigned int*)mesh->indices;
                 sc.NbFaces			= mesh->triangleCount;
                 sc.AskForWords		= true;
                 sc.ConnectAllStrips	= true;
                 sc.OneSided			= true;
                 sc.SGIAlgorithm		= true;
                 
                 Striper Strip;
                 Strip.Init(sc);
                 
                 STRIPERRESULT sr;
                 Strip.Compute(sr);
                 
                 std::vector<unsigned int> testor;
                 
                 uword* Refs = (uword*)sr.StripRuns;
                 for(udword i=0;i<sr.NbStrips;i++)
                 {
                 udword NbRefs = sr.StripLengths[i];
                 for(udword j=0;j<NbRefs;j++)
                 {
                 //fprintf(stdout, "%d ", *Refs++);
                 testor.push_back(*Refs++);
                 }
                 }
                 
                 delete [] mesh->indices;
                 
                 mesh->indices = new int[testor.size()];
                 for(int v = 0;v < testor.size();v++)
                 {
                 memcpy(&mesh->indices[v],&testor[v],sizeof(int));
                 }
                 
                 mesh->indicesCount = testor.size();*/
                
				//end
                
				temp.clear();
				tempIndices.clear();
                
				mesh->mUVFace.clear();
				mesh->mFace.clear();
				mesh->mNormalFace.clear();
                
			}
            
			model->allVertex.clear();
			model->allNormal.clear();
			model->allUVMap.clear();
		}

		void OpenGLES1RenderManager::DrawModelObj(ModelObj *model,Math::Vector3 position,Math::Vector3 scale,Math::Vector3 rotation)
		{
            glColor4ub(255, 255, 255, 255);
            
			glPushMatrix();
            
			glRotatef(rotation.x,1.0f,0.0f,0.0f);
			glRotatef(rotation.y,0.0f,1.0f,0.0f);
			glRotatef(rotation.z,0.0f,0.0f,1.0f);
			glTranslatef(position.x,position.y,position.z);
			glScalef(scale.x,scale.y,scale.z);
            
			for (unsigned int i = 0; i < model->mMeshes.size();i++)
			{
				ObjMesh *mesh = model->mMeshes[i];
                
				glBindTexture(GL_TEXTURE_2D, model->mMaterials[mesh->mMaterial]->texturID);
				glEnable(GL_TEXTURE_2D);
                
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                
				glTexCoordPointer(2, GL_FLOAT, GLsizei(sizeof(TexturesPSPVertex)), &mesh->meshVertices[0].u);
				glVertexPointer(3, GL_FLOAT, GLsizei(sizeof(TexturesPSPVertex)), &mesh->meshVertices[0].x);
                
				glDrawElements(GL_TRIANGLES, GLsizei(mesh->indicesCount), GL_UNSIGNED_SHORT, &mesh->indices[0]);
                
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                
				glDisable(GL_TEXTURE_2D);
                
				/*glEnableClientState(GL_VERTEX_ARRAY);
                 glVertexPointer(3, GL_FLOAT, GLsizei(sizeof(TexturesPSPVertex)), &mesh->meshVertices[0].x);
                 
                 if(mesh->mMaterial != -1)
                 {
                 //texture
                 glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                 //glActiveTexture(GL_TEXTURE0);
                 //glClientActiveTexture(GL_TEXTURE0);
                 
                 bindTexture(model->mMaterials[mesh->mMaterial]->texturID);
                 glEnable(GL_TEXTURE_2D);			
                 
                 glTexCoordPointer(2, GL_FLOAT, GLsizei(sizeof(TexturesPSPVertex)), &mesh->meshVertices[0].u);
                 }
                 
                 
                 if (mesh->mMaterial != -1 && model->mMaterials[mesh->mMaterial]->lightmapping)
                 {
                 glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                 glActiveTexture(GL_TEXTURE1);
                 glClientActiveTexture(GL_TEXTURE1);
                 
                 glEnable(GL_TEXTURE_2D);
                 glBindTexture(GL_TEXTURE_2D, model->mMaterials[mesh->mMaterial]->lightMapID);
                 glTexCoordPointer(2, GL_FLOAT, GLsizei(sizeof(TexturesPSPVertex)), &mesh->meshVertices[0].u);
                 
                 glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                 }
                 
                 //glDrawElements(GL_TRIANGLE_STRIP, GLsizei(mesh->indicesCount), GL_UNSIGNED_INT, &mesh->indices[0]);
                 glDrawElements(GL_TRIANGLES, GLsizei(mesh->indicesCount), GL_UNSIGNED_INT, &mesh->indices[0]);
                 
                 glDisable(GL_TEXTURE_2D);
                 glDisableClientState(GL_VERTEX_ARRAY);
                 glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/
			}
            
			glPopMatrix();
		}
	}
}