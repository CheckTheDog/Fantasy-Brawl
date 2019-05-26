#include "UI_Slider.h"
#include "j1App.h"
#include "j1Render.h"
#include "UI_Button.h"
#include "Brofiler\Brofiler.h"

void Slider::appendChild(int x, int y, UI_element * child)
{
	child->localPosition = { x, y };
	child->setOriginalPos(x, y);
	child->parent = this;
	if (child->element_type == TEXT)
		progress_num = (Text*)child;
	else if (child->element_type == BUTTON)
		button = (Button*)child;

	children.push_back(child);
}
float Slider::getProgress() const
{
	return progress;
}

void Slider::setProgress(float newProgress)
{
	progress = newProgress;
}

void Slider::BlitElement()
{
	BROFILER_CATEGORY("Slider Blit", Profiler::Color::GreenYellow);

	iPoint globalPos = calculateAbsolutePosition();
	App->render->Blit(texture, globalPos.x, globalPos.y, &section);
	button->localPosition.y = -2;

	//p2SString newText("%.0f", (progress * 100));
	std::string newText = std::to_string(progress * 100);
	progress_num->setText(newText);
	progress_num->localPosition.x = full.w * App->gui->UI_scale - progress_num->section.w / (2 / App->gui->UI_scale);
	progress_num->localPosition.y = -progress_num->section.h * App->gui->UI_scale;

	if (progress < 0.01)
		progress = 0;
	else if (progress > 1)
		progress = 1;
	full.w = ((bar_length)* progress);
	App->render->Blit(texture, globalPos.x, globalPos.y, &full);
	button->BlitElement();
	progress_num->BlitElement();
}