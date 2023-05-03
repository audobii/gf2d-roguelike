#ifndef __UI_COMMON_H__
#define __UI_COMMON_H__

#include "gfc_vector.h"
#include "entity.h"
#include "gfc_text.h"

typedef struct UIElement_S{
	int width;
	int height;
	Vector2D pos;

	void (*draw) (struct UIElement_S* self);
	void (*update) (struct UIElement_S* self);

	TextLine name;
}UIElement;

#endif