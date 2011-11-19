#include <Aurora/Graphics/psp/GuRenderManager.h>

#include <Aurora/Graphics/TextureManager.h>

#include <pspge.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <psputils.h>
#include <pspkernel.h>
#include <pspdebug.h>

#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 			480
#define SCREEN_HEIGHT 			272
#define SCREEN_WIDTH_F 			480.0f
#define SCREEN_HEIGHT_F			272.0f
#define	BUFFER_FORMAT			GU_PSM_8888
#define BUF_WIDTH 512
#define SCR_WIDTH 480
#define SCR_HEIGHT 272
#define PIXEL_SIZE				4
#define	FRAME_BUFFER_WIDTH 		512
#define FRAME_BUFFER_SIZE		FRAME_BUFFER_WIDTH*SCR_HEIGHT*PIXEL_SIZE

typedef struct
{
//unsigned char u, v;
//unsigned short color __attribute__((aligned(2)));
char x, y, z;
}__attribute__((packed)) Vertex8;

struct Vertex32
{
	float x,y,z;
};

struct Vertex32 __attribute__((aligned(16))) _cubeVertices[12*3] =
{
	{-0.5f,-0.5f, 0.5f}, // 0
	{-0.5f, 0.5f, 0.5f}, // 4
	{ 0.5f, 0.5f, 0.5f}, // 5

	{-0.5f,-0.5f, 0.5f}, // 0
	{ 0.5f, 0.5f, 0.5f}, // 5
	{ 0.5f,-0.5f, 0.5f}, // 0.5f

	{-0.5f,-0.5f,-0.5f}, // 3
	{ 0.5f,-0.5f,-0.5f}, // 2
	{ 0.5f, 0.5f,-0.5f}, // 6

	{-0.5f,-0.5f,-0.5f}, // 3
	{ 0.5f, 0.5f,-0.5f}, // 6
	{-0.5f, 0.5f,-0.5f}, // 7

	{ 0.5f,-0.5f,-0.5f}, // 0
	{ 0.5f,-0.5f, 0.5f}, // 3
	{ 0.5f, 0.5f, 0.5f}, // 7

	{ 0.5f,-0.5f,-0.5f}, // 0
	{ 0.5f, 0.5f, 0.5f}, // 7
	{ 0.5f, 0.5f,-0.5f}, // 4

	{-0.5f,-0.5f,-0.5f}, // 0
	{-0.5f, 0.5f,-0.5f}, // 3
	{-0.5f, 0.5f, 0.5f}, // 7

	{-0.5f,-0.5f,-0.5f}, // 0
	{-0.5f, 0.5f, 0.5f}, // 7
	{-0.5f,-0.5f, 0.5f}, // 4

	{-0.5f, 0.5f,-0.5f}, // 0
	{ 0.5f, 0.5f,-0.5f}, // 0.5f
	{ 0.5f, 0.5f, 0.5f}, // 2

	{-0.5f, 0.5f,-0.5f}, // 0
	{ 0.5f, 0.5f, 0.5f}, // 2
	{-0.5f, 0.5f, 0.5f}, // 3

	{-0.5f,-0.5f,-0.5f}, // 4
	{-0.5f,-0.5f, 0.5f}, // 7
	{ 0.5f,-0.5f, 0.5f}, // 6

	{-0.5f,-0.5f,-0.5f}, // 4
	{ 0.5f,-0.5f, 0.5f}, // 6
	{ 0.5f,-0.5f,-0.5f}, // 5
};

static unsigned int __attribute__((aligned(16))) _list[262144];

namespace Aurora
{
	namespace Graphics
	{
		GuRenderManager::GuRenderManager()
		{
			// default values
			_width = 480;
			_height = 272;
			_pov = 53;
			_vSync = true;
			_fullScreen = false;
			_zMin = 0.1f;
			_zMax = 256.0f;

			//gu settings
			_fbp0 = 0;
			_fbp1 = 0;
			_zbp = 0;

			//texture stuff
			_currentTexture = -1;
			_imageIdCounter = 0;
		}

		void GuRenderManager::Init()
		{
			//_fbp0 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
			//_fbp1 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
			//_zbp = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_4444);

			sceGuInit();
			
			_fbp0 = ( u32* ) valloc ( FRAME_BUFFER_SIZE ) ;
			_fbp1 = ( u32* ) valloc ( FRAME_BUFFER_SIZE );
			_zbp = ( u16* )  valloc ( FRAME_BUFFER_WIDTH*SCREEN_HEIGHT*2);

			sceGuStart(GU_DIRECT,_list);

			//sceGuDrawBuffer(GU_PSM_8888,_fbp0,BUF_WIDTH);
			//sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,_fbp1,BUF_WIDTH);
			//sceGuDepthBuffer(_zbp,BUF_WIDTH);
			
			sceGuDrawBuffer(BUFFER_FORMAT, vrelptr (_fbp0), FRAME_BUFFER_WIDTH);
			sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, vrelptr (_fbp1), FRAME_BUFFER_WIDTH);
			sceGuDepthBuffer(vrelptr (_zbp), FRAME_BUFFER_WIDTH);

			sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
			sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);
			sceGuDepthRange(65535,0);
			sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
			sceGuEnable(GU_SCISSOR_TEST);
			sceGuDepthFunc(GU_GEQUAL);
			sceGuEnable(GU_DEPTH_TEST);

			sceGuDisable(GU_TEXTURE_2D);
			sceGuEnable(GU_CLIP_PLANES);

			sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

			sceGuFinish();
			sceGuSync(0,0);

			sceDisplayWaitVblankStart();
			sceGuDisplay(GU_TRUE);

			sceCtrlSetSamplingCycle(0);
			sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

			sceKernelDcacheWritebackInvalidateAll();

		}

		void GuRenderManager::SetOrtho()
		{
			sceGumMatrixMode(GU_PROJECTION);
			sceGumLoadIdentity();
			sceGumOrtho(0, _width, _height, 0, _zMin, _zMax);

			sceGumMatrixMode(GU_VIEW);
			sceGumLoadIdentity();

			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
		}

		void GuRenderManager::SetTextOrtho()
		{
			sceGumMatrixMode(GU_PROJECTION);
			sceGumLoadIdentity();
			sceGumOrtho(0, _width, 0, _height, _zMin, _zMax);

			sceGumMatrixMode(GU_VIEW);
			sceGumLoadIdentity();

			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
		}

		void GuRenderManager::SetOrtho(float left,float right,float bottom,float top,float zmin,float zmax)
		{
			sceGumMatrixMode(GU_PROJECTION);
			sceGumLoadIdentity();
			sceGumOrtho(left, right, bottom, top, zmin, zmax);

			sceGumMatrixMode(GU_VIEW);
			sceGumLoadIdentity();

			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
		}

		void GuRenderManager::SetPerspective()
		{
			sceGumMatrixMode(GU_PROJECTION);
			sceGumLoadIdentity();
			sceGumPerspective(_pov,(float)_width/(float)_height,_zMin,_zMax);

			sceGumMatrixMode(GU_VIEW);
			sceGumLoadIdentity();

			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
		}

		void GuRenderManager::SetPerspective(float pov,float aspect,float zmin,float zmax)
		{
			sceGumMatrixMode(GU_PROJECTION);
			sceGumLoadIdentity();
			sceGumPerspective(pov,aspect,zmin,zmax);

			sceGumMatrixMode(GU_VIEW);
			sceGumLoadIdentity();

			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
		}

		void GuRenderManager::StartFrame()
		{
			sceGuStart(GU_DIRECT,_list);

			sceGuClearColor(0xff554433);
			sceGuClearDepth(0);
			sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
		}

		void GuRenderManager::EndFrame()
		{
			sceGuFinish();
			sceGuSync(0,0);

			sceDisplayWaitVblankStart();
			sceGuSwapBuffers();
		}

		void GuRenderManager::_createTexture(Image* image)
		{
			if (image != 0)
			{
				image->_id = _imageIdCounter;
				_imageIdCounter++;
			}
		}

		void GuRenderManager::_createTexture(unsigned char* pixels,int width,int height,unsigned int &texId)
		{
			texId = _imageIdCounter;
			_imageIdCounter++;
		}

		void GuRenderManager::bindTexture(Image* image)
		{
			if(image->_id != _currentTexture)
			{
				sceGuTexMode(GU_PSM_8888,0,0,0);
				sceGuTexImage(0,image->_width,image->_height,image->_width,image->_pixels);

				sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);
				sceGuTexFilter(GU_NEAREST,GU_NEAREST);
				sceGuTexOffset( 0.0f, 0.0f );
				sceGuTexWrap(GU_REPEAT,GU_REPEAT);

				_currentTexture = image->_id;
			}
		}

		void GuRenderManager::bindTexture(std::string filename)
		{
			Image* image = TextureManager::Instance()->getImage(filename);

			if(image->_id != _currentTexture)
			{
				sceGuTexMode(GU_PSM_8888,0,0,0);
				sceGuTexImage(0,image->_width,image->_height,image->_width,image->_pixels);

				sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);
				sceGuTexFilter(GU_NEAREST,GU_NEAREST);
				sceGuTexOffset( 0.0f, 0.0f );
				sceGuTexWrap(GU_REPEAT,GU_REPEAT);

				_currentTexture = image->_id;
			}
		}

		void GuRenderManager::drawImage(Image* image)
		{
			bindTexture(image);

			sceGuEnable(GU_TEXTURE_2D);
			sceGuEnable(GU_BLEND);

			TexturedVertex* vertices = (TexturedVertex*)sceGuGetMemory(4 * sizeof(TexturedVertex));

			vertices[0].u = 0.0f; vertices[0].v = 0.0f; vertices[0].x = 0; vertices[0].y = 0; vertices[0].z =0;
			vertices[1].u = 0.0f; vertices[1].v = 1.0f; vertices[1].x = 0; vertices[1].y = image->_height; vertices[1].z =0;
			vertices[2].u = 1.0f; vertices[2].v = 0.0f; vertices[2].x = image->_width; vertices[2].y = 0; vertices[2].z =0;
			vertices[3].u = 1.0f; vertices[3].v = 1.0f; vertices[3].x = image->_width; vertices[3].y = image->_height; vertices[3].z =0;

			sceGuColor(0xffffffff);
			sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, vertices);

			sceGuDisable(GU_BLEND);
			sceGuDisable(GU_TEXTURE_2D);
		}

		void GuRenderManager::drawImage(Image* image,int posx,int posy)
		{
			bindTexture(image);
		}

		void GuRenderManager::drawImage(Image* image,int posx,int posy,int width,int height)
		{
			bindTexture(image);

			sceGuEnable(GU_TEXTURE_2D);
			sceGuEnable(GU_BLEND);

			TexturedVertex* vertices = (TexturedVertex*)sceGuGetMemory(4 * sizeof(TexturedVertex));

			vertices[0].u = 0.0f; vertices[0].v = 0.0f; vertices[0].x = posx; vertices[0].y = posy; vertices[0].z =0;
			vertices[1].u = 0.0f; vertices[1].v = 1.0f; vertices[1].x = posx; vertices[1].y = posy + height; vertices[1].z =0;
			vertices[2].u = 1.0f; vertices[2].v = 0.0f; vertices[2].x = posx+width; vertices[2].y = posy; vertices[2].z =0;
			vertices[3].u = 1.0f; vertices[3].v = 1.0f; vertices[3].x = posx+width; vertices[3].y = posy + height; vertices[3].z =0;

			sceGuColor(0xffffffff);
			sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, vertices);

			sceGuDisable(GU_BLEND);
			sceGuDisable(GU_TEXTURE_2D);
		}

		float g_tabStops2[4] = {150, 210, 270, 330};
		void GuRenderManager::drawText(TrueTypeFont* font,float x, float y, const char *text, int align, unsigned int col)
		{
			if (!font->g_ftex) return;
			if (!text) return;

			_currentTexture = font->g_ftex;

			y = _height - y;

			if (align == Graphics::ALIGN_CENTER)
				x -= font->getTextLength(font->g_cdata, text)/2;
			else if (align == Graphics::ALIGN_RIGHT)
				x -= font->getTextLength(font->g_cdata, text);

			sceGuTexMode(GU_PSM_8888,0,0,0);
			sceGuTexImage(0,256,256,256,font->bitmap);

			sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
			sceGuTexFilter(GU_LINEAR,GU_LINEAR);
			sceGuTexOffset( 0.0f, 0.0f );
			sceGuTexWrap(GU_REPEAT,GU_REPEAT);

			sceGuColor(GU_RGBA(col&0xff, (col>>8)&0xff, (col>>16)&0xff, (col>>24)&0xff));
			sceGuEnable(GU_TEXTURE_2D);
			sceGuEnable(GU_BLEND);

			const float ox = x;

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

					TexturedVertex* vertices = (TexturedVertex*)sceGuGetMemory(6 * sizeof(TexturedVertex));

					vertices[0].u = q.s0; vertices[0].v = q.t0; vertices[0].x =q.x0; vertices[0].y = q.y0; vertices[0].z =0;
					vertices[1].u = q.s1; vertices[1].v = q.t1; vertices[1].x =q.x1; vertices[1].y = q.y1; vertices[1].z =0;
					vertices[2].u = q.s1; vertices[2].v = q.t0; vertices[2].x =q.x1; vertices[2].y = q.y0; vertices[2].z =0;

					vertices[3].u = q.s0; vertices[3].v = q.t0; vertices[3].x =q.x0; vertices[3].y = q.y0; vertices[3].z =0;
					vertices[4].u = q.s0; vertices[4].v = q.t1; vertices[4].x =q.x0; vertices[4].y = q.y1; vertices[4].z =0;
					vertices[5].u = q.s1; vertices[5].v = q.t1; vertices[5].x =q.x1; vertices[5].y = q.y1; vertices[5].z =0;

					sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 6, 0, vertices);
				}
				++text;
			}


			sceGuDisable(GU_BLEND);
			sceGuDisable(GU_TEXTURE_2D);

			sceGuColor(0xffffffff);
		}

		void GuRenderManager::drawSprite(Sprite* sprite)
		{
			bindTexture(sprite->imageName);

			sceGumPushMatrix();

			ScePspFVector3 loc = {sprite->posX,sprite->posY,0.0f};
			sceGumTranslate(&loc);

			sceGuEnable(GU_TEXTURE_2D);
			sceGuEnable(GU_BLEND);

			sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, sprite->vertices);

			sceGuDisable(GU_BLEND);
			sceGuDisable(GU_TEXTURE_2D);

			sceGumPopMatrix();
		}

		void GuRenderManager::drawCube(unsigned int color,Math::Vector3 position,Math::Vector3 scale,Math::Vector3 rotation)
		{
			sceGumPushMatrix();

			ScePspFVector3 rot = {Aurora::Math::Math::degreesToRadians(rotation.x),Aurora::Math::Math::degreesToRadians(rotation.y),Aurora::Math::Math::degreesToRadians(rotation.z)};
			sceGumRotateXYZ(&rot);

			ScePspFVector3 loc = {position.x,position.y,position.z};
			sceGumTranslate(&loc);

			sceGuColor(GU_RGBA(color&0xff, (color>>8)&0xff, (color>>16)&0xff, (color>>24)&0xff));

			sceGumDrawArray(GU_TRIANGLES,GU_VERTEX_32BITF|GU_TRANSFORM_3D,12*3, 0,_cubeVertices);

			sceGuColor(0xffffffff);
			sceGumPopMatrix();
		}

		void GuRenderManager::drawSprite3D(Sprite3D* sprite)
		{
			sceGumPushMatrix();

			ScePspFVector3 loc = {sprite->posX,sprite->posY,sprite->posZ};
			ScePspFVector3 rot = {Aurora::Math::Math::degreesToRadians(sprite->rotationX),Aurora::Math::Math::degreesToRadians(sprite->rotationY),Aurora::Math::Math::degreesToRadians(sprite->rotationZ)};

			sceGumTranslate(&loc);
			sceGumRotateXYZ(&rot);

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

			sceGumPopMatrix();
		}
	}
}
