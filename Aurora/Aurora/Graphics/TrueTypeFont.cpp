#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Graphics/TrueTypeFont.h>
#include <stdio.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

namespace Aurora
{
	namespace Graphics
	{
		float g_tabStops[4] = {150, 210, 270, 330};

		TrueTypeFont::TrueTypeFont()
		{
			bitmap = 0;
			g_ftex = 0;
			fontLoaded = false;
		}

		TrueTypeFont::TrueTypeFont(const char *fontFileName)
		{
			fontLoaded = loadFont(fontFileName,15);
		}

		TrueTypeFont::TrueTypeFont(const char *fontFileName,float size)
		{
			fontLoaded = loadFont(fontFileName,size);
		}

		TrueTypeFont::~TrueTypeFont()
		{
			if(bitmap != 0)
				delete [] bitmap;
		}

		static unsigned char compute_y(int r, int g, int b)
		{
			return (unsigned char) (((r*77) + (g*150) +  (29*b)) >> 8);
		}

		unsigned char *convert_format2(unsigned char *data, int img_n, int req_comp, int x, int y)
		{
			int i,j;
			unsigned char *good;

			if (req_comp == img_n) return data;

			good = new unsigned char[req_comp * x * y];//(unsigned char *) malloc(req_comp * x * y);
			if (good == NULL)
			{
				free(data);
				return 0;
			}

			for (j=0; j < (int) y; ++j)
			{
				unsigned char *src  = data + j * x * img_n   ;
				unsigned char *dest = good + j * x * req_comp;

				#define COMBO(a,b)  ((a)*8+(b))
				#define CASE(a,b)   case COMBO(a,b): for(i=x-1; i >= 0; --i, src += a, dest += b)
				// convert source image with img_n components to one with req_comp components;
				// avoid switch per pixel, so use switch per scanline and massive macros
				switch (COMBO(img_n, req_comp))
				{
					CASE(1,2) dest[0]=src[0], dest[1]=255; break;
					CASE(1,3) dest[0]=dest[1]=dest[2]=src[0]; break;
					CASE(1,4) 
					{
						dest[0] = dest[1] = dest[2] = 255 - src[0];
						if(src[0] == 0)
							dest[3]=0;
						else
							dest[3]=255;
					}
					break;
					CASE(2,1) dest[0]=src[0]; break;
					CASE(2,3) dest[0]=dest[1]=dest[2]=src[0]; break;
					CASE(2,4) dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1]; break;
					CASE(3,4) dest[0]=src[0],dest[1]=src[1],dest[2]=src[2],dest[3]=255; break;
					CASE(3,1) dest[0]=compute_y(src[0],src[1],src[2]); break;
					CASE(3,2) dest[0]=compute_y(src[0],src[1],src[2]), dest[1] = 255; break;
					CASE(4,1) dest[0]=compute_y(src[0],src[1],src[2]); break;
					CASE(4,2) dest[0]=compute_y(src[0],src[1],src[2]), dest[1] = src[3]; break;
					CASE(4,3) dest[0]=src[0],dest[1]=src[1],dest[2]=src[2]; break;

				}
				#undef CASE
			}

			free(data);
			return good;
		}

		bool TrueTypeFont::loadFont(const char *fontFileName,float fontSize)
		{
			// Load font.
			FILE* fp = fopen(fontFileName, "rb");
			if (!fp) return false;
			fseek(fp, 0, SEEK_END);
			int size = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			unsigned char* ttfBuffer = new unsigned char[size]; 
			if (!ttfBuffer)
			{
				fclose(fp);
				return false;
			}

			fread(ttfBuffer, 1, size, fp);
			fclose(fp);
			fp = 0;

			unsigned char* temp = new unsigned char[256*256];
			if (!temp)
			{
				free(ttfBuffer);
				return false;
			}

			stbtt_BakeFontBitmap(ttfBuffer,0, fontSize, temp,256,256, 32,96,(stbtt_bakedchar*)g_cdata);

			bitmap = convert_format2(temp,1,4,256,256);

			RenderManager::Instance()->_createTexture(bitmap,256,256,g_ftex);

			delete [] ttfBuffer;

			return true;
		}

		void TrueTypeFont::getBakedQuad(stbtt_bakedchar2 *chardata, int pw, int ph, int char_index,float *xpos, float *ypos, stbtt_aligned_quad2 *q)
		{
			stbtt_bakedchar2 *b = chardata + char_index;
			int round_x = STBTT_ifloor(*xpos + b->xoff);
			int round_y = STBTT_ifloor(*ypos - b->yoff);

			q->x0 = (float)round_x;
			q->y0 = (float)round_y;
			q->x1 = (float)round_x + b->x1 - b->x0;
			q->y1 = (float)round_y - b->y1 + b->y0;

			q->s0 = b->x0 / (float)pw;
			q->t0 = b->y0 / (float)pw;
			q->s1 = b->x1 / (float)ph;
			q->t1 = b->y1 / (float)ph;

			*xpos += b->xadvance;
		}

		float TrueTypeFont::getTextLength(stbtt_bakedchar2 *chardata, const char* text)
		{
			float xpos = 0;
			float len = 0;
			while (*text)
			{
				int c = (unsigned char)*text;
				if (c == '\t')
				{
					for (int i = 0; i < 4; ++i)
					{
						if (xpos < g_tabStops[i])
						{
							xpos = g_tabStops[i];
							break;
						}
					}
				}
				else if (c >= 32 && c < 128)
				{
					stbtt_bakedchar2 *b = chardata + c-32;
					int round_x = STBTT_ifloor((xpos + b->xoff) + 0.5);
					len = round_x + b->x1 - b->x0 + 0.5f;
					xpos += b->xadvance;
				}
				++text;
			}
			return len;
		}		
	}
}