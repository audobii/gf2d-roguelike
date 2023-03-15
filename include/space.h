#ifndef __SPACE_H__
#define __SPACE_H__

#include "gfc_shape.h"
#include "body.h"

typedef struct
{
	List* dynamicBodyList;    
	List* staticShapes;
}Space;

#endif