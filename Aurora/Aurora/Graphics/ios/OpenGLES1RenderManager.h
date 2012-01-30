#ifndef OPENGLES1RenderManager_H
#define OPENGLES1RenderManager_H

#include <Aurora/Graphics/RenderManager.h>

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

namespace Aurora
{
	namespace Graphics
	{
        enum DeviceOrientation
        {
            DeviceOrientationUnknown,
            DeviceOrientationPortrait,
            DeviceOrientationPortraitUpsideDown,
            DeviceOrientationLandscapeLeft,
            DeviceOrientationLandscapeRight,
            DeviceOrientationFaceUp,
            DeviceOrientationFaceDown,
        };
        
		class OpenGLES1RenderManager : public RenderManager
		{
        private:
            
            GLuint m_framebuffer;
            GLuint m_colorRenderbuffer;
            GLuint m_depthRenderbuffer;
            
            static DeviceOrientation _deviceOrientation;
            
        public:
            
            static int deviceRotationAngle;
            
        private:
            
            void __gluMakeIdentityf(GLfloat m[16]);
            void __normalize(GLfloat v[3]);
            void __cross(GLfloat v1[3], GLfloat v2[3], GLfloat result[3]);


            void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
            void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez, GLfloat centerx,
            GLfloat centery, GLfloat centerz, GLfloat upx, GLfloat upy,GLfloat upz);


		protected:

			void _createTexture(Image* image);
			void _createTexture(unsigned char* pixels,int width,int height,unsigned int &texId);
			void _createEmptyTexture( Image* image, ImageLocation location );
			void _createModelObjVertices(ModelObj* model);

		public:

			OpenGLES1RenderManager();

			void Init();

			void SetOrtho();
			void SetOrtho(float left,float right,float bottom,float top,float zmin,float zmax);
			void SetPerspective();			
			void SetPerspective(float pov,float aspect,float zmin,float zmax);

			void ClearScreen();
			void StartFrame();
			void EndFrame();

			void bindTexture(int id);
			void bindTexture(Image* image);
			void bindTexture(std::string filename);

			void drawImage(Image* image);
			void drawImage(Image* image,int posx,int posy);
			void drawImage(Image* image,int posx,int posy,int width,int height);

			void drawSprite(Sprite* sprite);
			void drawSprite3D(Sprite3D* sprite);

			void SetTextOrtho();
			void drawText(TrueTypeFont* font,float x, float y, const char *text, int align, unsigned int col);

			//render to texture
			void StartRenderToTexture(Image* texture);
			void EndRenderToTexture(Image* texture);
			void RenderToScreen();

			void UpdateCurrentCamera();

			//shapes
			void drawCube(unsigned int color,Math::Vector3 position,Math::Vector3 scale,Math::Vector3 rotation);
			void DrawCubeTextured(Image* texture,Math::Vector3 position,Math::Vector3 scale,Math::Vector3 rotation);
			void DrawModelObj(ModelObj *model,Math::Vector3 position,Math::Vector3 scale,Math::Vector3 rotation);
            
        public:
            
            static void setDeviceOrientation(DeviceOrientation orientation);

		};
	}
}

#endif