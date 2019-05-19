#include "UI_Marker.h"
#include "j1Render.h"

void Marker::BlitElement()
{
	iPoint globalPos = {0,0};

	if (is_valid_iterator == true)
	{
		if (distance_moved >= max_movement_distance)
		{
			distance_moved = max_movement_distance;
			movement_timer.Start();
		}
		distance_moved = int(movement_timer.ReadSec() * speed);
		/*int horizontal_anim = distance_moved;*/

		//This Hardcode really hurts my heart...
		if ((*(*target))->parent != nullptr &&  
			(*(*target))->parent->element_type == SLIDER)
		{
			globalPos.x = (*(*target))->parent->localPosition.x + (*(*target))->parent->section.w + margin.x + distance_moved;
			globalPos.y = (*(*target))->parent->localPosition.y + ((*(*target))->parent->section.h / 6) + margin.y;
		}
		else
		{
			globalPos.x = (*(*target))->localPosition.x + (*(*target))->section.w + margin.x + distance_moved;
			globalPos.y = (*(*target))->localPosition.y + ((*(*target))->section.h / 6) + margin.y;
		}

	}
	App->render->Blit(texture, globalPos.x, globalPos.y, &section);
}

void Marker::SetTarget(std::list<UI_element*>::iterator* tar)
{
	target = tar;
	is_valid_iterator = true;
}
