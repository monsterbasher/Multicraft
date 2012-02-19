#ifndef PSGLRenderManager_H
#define PSGLRenderManager_H

#include <Aurora/Graphics/RenderManager.h>

namespace Aurora
{
	namespace Graphics
	{
		class PSGLRenderManager : public RenderManager
		{

		protected:
		
			//psgl only
			int _getBestRes(const unsigned int *resolutions, unsigned int numResolutions);
			int _getResSize(const unsigned int resolutionId, int &w, int &h);
			void _regVideoCallback();
			void _loadPrxLibs();
			bool _prepareVideoOut();
			void _psglInit();

			void _createTexture(Image* image);
			void _createTexture(unsigned char* pixels,int width,int height,unsigned int &texId);
			void _createEmptyTexture( Image* image, ImageLocation location );
			void _createModelObjVertices(ModelObj* model);
			void _extractFrustumPlanes(Frustum *frustum);

		public:

			PSGLRenderManager();

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