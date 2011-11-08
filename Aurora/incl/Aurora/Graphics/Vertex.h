#ifndef AURORA_VERTEX_H
#define AURORA_VERTEX_H

namespace Aurora
{
	namespace Graphics
	{	
		typedef struct
		{
			float u,v;
			float x,y,z;
		}TexturedVertex;

		typedef struct
		{
			float x,y,z;
		}SimpleVertex;

	}
}

#endif
