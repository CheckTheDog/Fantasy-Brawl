#include "UI_Image.h"
#include "j1App.h"
#include "j1Render.h"
#include "Brofiler\Brofiler.h"

void Image::BlitElement()
{
	BROFILER_CATEGORY("Image Blit", Profiler::Color::Chocolate);

	if (texture != App->gui->GetAtlas())
		SDL_SetTextureAlphaMod(texture, App->gui->alpha_value);
	iPoint globalPos = calculateAbsolutePosition();
	App->render->Blit(texture, globalPos.x, globalPos.y, &section);
}