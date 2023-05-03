#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include "gfc_vector.h"
#include "gfc_text.h"

typedef struct {
	TextLine str;
	Color color;
}UIText;

/**
 * @brief create a new UI text element
 */
void ui_text_new(Vector2D position);

#endif