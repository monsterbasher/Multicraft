#ifndef OPENGLRenderManager_H
#define OPENGLRenderManager_H

#include <Aurora/Graphics/RenderManager.h>

namespace Aurora
{
	namespace Graphics
	{
		class OpengGLRenderManager : public RenderManager
		{

		protected:

			void _createTexture(Image* image);
			void _createTexture(unsigned char* pixels,int width,int height,unsigned int &texId);

		public:

			OpengGLRenderManager();

			void Init();

			void SetOrtho();
			void SetOrtho(float left,float right,float bottom,float top,float zmin,float zmax);
			void SetPerspective();			
			void SetPerspective(float pov,float aspect,float zmin,float zmax);

			void StartFrame();
			void EndFrame();

			void bindTexture(Image* image);
			void bindTexture(std::string filename);

			void drawImage(Image* image);
			void drawImage(Image* image,int posx,int posy);
			void drawImage(Image* image,int posx,int posy,int width,int height);

			void drawSprite(Sprite* sprite);
			void drawSprite3D(Sprite3D* sprite);

			void SetTextOrtho();
			void drawText(TrueTypeFont* font,float x, float y, const char *text, int align, unsigned int col);

			//shapes
			void drawCube(unsigned int color,Math::Vector3 position,Math::Vector3 scale,Math::Vector3 rotation);
		};
	}
}

#endif