#include "UI_Marker.h"
#include "j1Render.h"

void Marker::BlitElement()
{
	iPoint globalPos = {0,0};

	if (is_valid_iterator == true)
	{
		globalPos.x = (*(*target))->localPosition.x + (*(*target))->section.w + margin.x;
		globalPos.y = (*(*target))->localPosition.y + ((*(*target))->section.h/4) + margin.y;
	}
	App->render->Blit(texture, globalPos.x, globalPos.y, &section);
}

void Marker::SetTarget(std::list<UI_element*>::iterator* tar)
{
	target = tar;
	is_valid_iterator = true;
}
