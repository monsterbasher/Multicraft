#ifndef GuRenderManager_H
#define GuRenderManager_H

#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Graphics/Vertex.h>
#include <Aurora/Graphics/gu/vram.h>

namespace Aurora
{
	namespace Graphics
	{
		class GuRenderManager : public RenderManager
		{
		private:

			//unsigned int __attribute__((aligned(64))) _list[262144];

			void* _fbp0;
			void* _fbp1;
			void* _zbp;
			void* _frameBuffer;

			int _imageIdCounter;

		protected:

			void _createTexture(Image* image);
			void _createTexture(unsigned char* pixels,int width,int height,unsigned int &texId);
			void _createEmptyTexture( Image* image, ImageLocation location );
			void _createModelObjVertices(ModelObj* model);
			void _extractFrustumPlanes(Frustum *frustum);

		public:

			GuRenderManager();

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
		};
	}
}

#endif
