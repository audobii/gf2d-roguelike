#include "ui_common.h"

typedef struct
{
    Uint32  ui_max;
    UIElement* ui_list;
}UIManager;

static UIManager ui_manager = { 0 };

