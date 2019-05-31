#include "UI_Marker.h"
#include "j1Render.h"

void Marker::BlitElement()
{
	iPoint globalPos = {0,0};

	if (is_valid_iterator == true)
	{
		if (anim_data.anim == Marker_anim::RIGHT_TO_LEFT)
		{
			if (distance_moved >= (anim_data.max_movement_distance * int(anim_data.anim)))
			{
				distance_moved = anim_data.max_movement_distance * int(anim_data.anim);
				movement_timer.Start();
			}
			distance_moved = int(movement_timer.ReadSec() * anim_data.speed * int(anim_data.anim));
			/*int horizontal_anim = distance_moved;*/
		}
		else if (anim_data.anim == Marker_anim::LEFT_TO_RIGHT)
		{
			if (distance_moved <= (anim_data.max_movement_distance * int(anim_data.anim)))
			{
				distance_moved = anim_data.max_movement_distance * int(anim_data.anim);
				movement_timer.Start();
			}
			distance_moved = int(movement_timer.ReadSec() * anim_data.speed * int(anim_data.anim));
			/*int horizontal_anim = distance_moved;*/
		}

		//This Hardcode really hurts my heart...
		if ((*(*target))->parent != nullptr &&  
			(*(*target))->parent->element_type == SLIDER)
		{
			globalPos.x = (*(*target))->parent->localPosition.x + ((*(*target))->parent->section.w + margin.x + distance_moved) ;
			globalPos.y = (*(*target))->parent->localPosition.y - ((*(*target))->parent->section.h / 6) + margin.y - 20;
		}
		else
		{
			globalPos.x = (*(*target))->localPosition.x + (*(*target))->section.w + margin.x + distance_moved;
			globalPos.y = (*(*target))->localPosition.y + ((*(*target))->section.h / 6) + margin.y;
		}
		
		if (IN_RANGE((*(*target))->element_type, element_type::CUSTOMIZING_BUTTON_BASIC, element_type::CUSTOMIZING_BUTTON_SHIELD) && this->callback != nullptr)
		{
			this->callback->OnUIEvent(this, MARKER_OVER_CUSTOMIZER);

			if ((*(*target))->is_locked == true)
			{
				section = rects.custom_locked;
			}
			else
			{
				section = rects.custom_unlocked;
			}
		}
		else
			section = original_section;

		if ((*(*target))->is_locked == true)
		{
			if (this->callback != nullptr)
			this->callback->OnUIEvent(this, MARKER_OVER_LOCKED_ELEMENT);
		}
	}

	if (is_support_marker == true)
	{
		if(IN_RANGE((*(*target))->element_type, element_type::CUSTOMIZING_BUTTON_BASIC, element_type::CUSTOMIZING_BUTTON_SHIELD))
			App->render->Blit(texture, globalPos.x, globalPos.y, &section);
	}
	else if (is_support_marker == false)
	{
		App->render->Blit(texture, globalPos.x, globalPos.y, &section);
	}
}

void Marker::SetTarget(std::list<UI_element*>::iterator* tar)
{
	target = tar;
	is_valid_iterator = true;
}
