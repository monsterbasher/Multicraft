#ifndef RenderManager_H
#define RenderManager_H

#include <Aurora/Graphics/Image.h>
#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/Sprite3D.h>
#include <Aurora/Graphics/TrueTypeFont.h>
#include <Aurora/Math/Vector3.h>
#include <Aurora/Math/Vector2.h>

namespace Aurora
{
	namespace Graphics
	{
		class TextureManager;
		class TrueTypeFont;

		class RenderManager
		{
		protected:

			int _width;
			int _height;
			bool _fullScreen;
			bool _vSync;
			float _pov;
			float _zMin;
			float _zMax;

			int _currentTexture;

			static RenderManager *_renderManager;
		
		protected:

			virtual void _createTexture(Image* image) = 0;
			virtual void _createTexture(unsigned char* pixels,int width,int height,unsigned int &texId) = 0;

		public:

			static RenderManager* Instance();

			RenderManager();

			void setPov(float pov);
			void setZminMax(float zmin,float zmax);
			void setSesize(int width,int height);
			void setFulscreen(bool state);
			void setVSync(bool state);

			inline float getPov() { return _pov; }
			inline float getZmin() { return _zMin; }
			inline float getZmax() { return _zMax; }
			inline int getWidth() { return _width; }
			inline int getHeight() { return _height; }
			inline bool getVsync() { return _vSync; }
			inline bool getFulscreen() { return _fullScreen;}

			friend class TextureManager;
			friend class TrueTypeFont;

		public:

			static inline unsigned int RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
			{
				return (r) | (g << 8) | (b << 16) | (a << 24);
			}

		public:

			virtual void Init() = 0;

			virtual void SetOrtho() = 0;
			virtual void SetOrtho(float left,float right,float bottom,float top,float zmin,float zmax) = 0;
			virtual void SetPerspective() = 0;			
			virtual void SetPerspective(float pov,float aspect,float zmin,float zmax) = 0;

			virtual void StartFrame() = 0;
			virtual void EndFrame() = 0;

			virtual void bindTexture(Image* image) = 0;
			virtual void bindTexture(std::string filename) = 0;

			virtual void drawImage(Image* image) = 0;
			virtual void drawImage(Image* image,int posx,int posy) = 0;
			virtual void drawImage(Image* image,int posx,int posy,int width,int height) = 0;

			virtual void drawSprite(Sprite* sprite) = 0;
			virtual void drawSprite3D(Sprite3D* sprite) = 0;

			virtual void SetTextOrtho() = 0;
			virtual void drawText(TrueTypeFont* font,float x, float y, const char *text, int align, unsigned int col) = 0;

			//shapes
			virtual void drawCube(unsigned int color,Math::Vector3 position,Math::Vector3 scale,Math::Vector3 rotation) = 0;
		};
	}
}

#endif