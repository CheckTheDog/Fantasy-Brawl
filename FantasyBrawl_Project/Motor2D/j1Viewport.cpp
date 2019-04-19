#include "j1App.h"
#include "j1Viewport.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "p2Log.h"
#include <vector>

j1Viewport::j1Viewport()
{
	name = "viewports";
}

j1Viewport::~j1Viewport()
{
}

bool j1Viewport::Awake()
{
	bool ret = true;

	return ret;
}

bool j1Viewport::Start()
{
	bool ret = true;

	number_of_views = 1;
	scale = 2.0f;

	App->win->GetWindowSize(win_w, win_h);

	// Screens ------
	four_views_1 = { 0, 0, (int)win_w / 2, (int)win_h / 2 };
	four_views_2 = { (int)win_w / 2, 0, (int)win_w / 2, (int)win_h / 2 };
	four_views_3 = { 0, (int)win_h / 2, (int)win_w / 2, (int)win_h / 2 };
	four_views_4 = { (int)win_w / 2, (int)win_h / 2, (int)win_w / 2, (int)win_h / 2 };

	return ret;
}

bool j1Viewport::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool j1Viewport::Update(float dt)
{
	bool ret = true;

	timer.Start();

	// Blit different layers
	BlitFromQueue();

	screen_size = GetScreenRect(1);

	return ret;
}

bool j1Viewport::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool j1Viewport::CleanUp()
{
	bool ret = true;

	return ret;
}

void j1Viewport::SetViews(uint views)
{
	if (views > 0 && views < 5 && views != 3)
	{
		number_of_views = views;

		switch (views)
		{
		case 1:
			scale = 1.0f;
			break;
		case 4:
			scale = 1.0f;
			break;
		default:
			break;
		}
	}
}

uint j1Viewport::GetViews()
{
	return number_of_views;
}

SDL_Rect j1Viewport::GetScreenRect(uint id)
{
	SDL_Rect ret = { 0,0,0,0 };

	switch (number_of_views)
	{
	case 1:
	{
		ret = { (int)-screen_1.x, (int)-screen_1.y, four_views_1.w, four_views_1.h };
		break;
	}
	case 4:
	{
		switch (id)
		{
		case 1:
			ret = { (int)-screen_1.x, (int)-screen_1.y, four_views_1.w, four_views_1.h };
			break;
		case 2:
			ret = { (int)-screen_2.x, (int)-screen_2.y, four_views_2.w, four_views_2.h };
			break;
		case 3:
			ret = { (int)-screen_3.x, (int)-screen_3.y, four_views_3.w, four_views_3.h };
			break;
		case 4:
			ret = { (int)-screen_4.x, (int)-screen_4.y, four_views_4.w, four_views_4.h };
			break;
		}
		break;
	}
	}
	return ret;
}

SDL_Rect j1Viewport::GetScreenSize()
{
	return screen_size;
}

void j1Viewport::ScreenMove(int id, float x, float y)
{
	switch (id)
	{
	case 1:
		screen_1.x += x;
		screen_1.y += y;
		break;
	case 2:
		screen_2.x += x;
		screen_2.y += y;
		break;
	case 3:
		screen_3.x += x;
		screen_3.y += y;
		break;
	case 4:
		screen_4.x += x;
		screen_4.y += y;
		break;
	default:
		break;
	}
}

void j1Viewport::ScreenPosition(int id, int x, int y)
{
	switch (id)
	{
	case 1:
		screen_1.x = x;
		screen_1.y = y;
		break;
	case 2:
		screen_2.x = x;
		screen_2.y = y;
		break;
	case 3:
		screen_3.x = x;
		screen_3.y = y;
		break;
	case 4:
		screen_4.x = x;
		screen_4.y = y;
		break;
	default:
		break;
	}
}

void j1Viewport::CenterScreen(int id, float x, float y)
{
	if (number_of_views == 1)
	{
		if (id == 1)
		{
			screen_1.x = (int)(-x + win_w / 4);
			screen_1.y = (int)(-y + win_h / 4);
		}
	}
	else if (number_of_views == 4)
	{
		switch (id)
		{
		case 1:
			screen_1.x = (int)(-x + win_w / 4);
			screen_1.y = (int)(-y + win_h / 4);
			break;
		case 2:
			screen_2.x = (int)(-x + win_w / 4);
			screen_2.y = (int)(-y + win_h / 4);
			break;
		case 3:
			screen_3.x = (int)(-x + win_w / 4);
			screen_3.y = (int)(-y + win_h / 4);
			break;
		case 4:
			screen_4.x = (int)(-x + win_w / 4);
			screen_4.y = (int)(-y + win_h / 4);
			break;
		default:
			break;
		}
	}
}

void j1Viewport::ResetScreens()
{
	screen_1.x = 0;
	screen_1.y = 0;

	screen_2.x = 0;
	screen_2.y = 0;

	screen_3.x = 0;
	screen_3.y = 0;

	screen_4.x = 0;
	screen_4.y = 0;
}

iPoint j1Viewport::GetScreenPosition(uint id)
{
	iPoint ret = { 0,0 };

	switch (id)
	{
	case 1:
		ret.x = (int)screen_1.x;
		ret.y = (int)screen_1.y;
		break;
	case 2:
		ret.x = (int)screen_2.x;
		ret.y = (int)screen_2.y;
		break;
	case 3:
		ret.x = (int)screen_3.x;
		ret.y = (int)screen_3.y;
		break;
	case 4:
		ret.x = (int)screen_4.x;
		ret.y = (int)screen_4.y;
		break;
	}

	return ret;
}

void j1Viewport::PushQueue(int layer, SDL_Texture * texture, int x, int y, const SDL_Rect section, int viewport, float speed, double angle, int pivot_x, int pivot_y)
{
	element_to_render lblit(layer, texture, x, y, section, viewport, speed, angle, pivot_x, pivot_y);

	SDL_Rect view = GetScreenSize();

	if (viewport == 0 || viewport == 1)
	{
		if(number_of_views == 1)
		{
			if (lblit.x > - 60 && lblit.y > - 60 && lblit.x < win_w + (screen_1.x)*(-1) && lblit.y < win_h + (screen_1.y)*(-1))
			{
				layer_list1.push(lblit);
			}
		}
		else if (number_of_views == 4)
		{
			SDL_Rect rect = GetScreenRect(1);
			if (lblit.x > rect.x - 60 && lblit.y > rect.y - 60 && lblit.x < rect.w + (screen_1.x)*(-1) && lblit.y < rect.h + (screen_1.y)*(-1))
			{
				layer_list1.push(lblit);
			}
		}
		
	}
	if ((viewport == 0 || viewport == 2) && number_of_views >= 2)
	{
		SDL_Rect rect = GetScreenRect(2);
		if (lblit.x > rect.x - 60 && lblit.y > rect.y - 60 && lblit.x < rect.w + (screen_2.x)*(-1) && lblit.y < rect.h + (screen_2.y)*(-1))
		{
			layer_list2.push(lblit);
		}
	}
	if ((viewport == 0 || viewport == 3) && number_of_views >= 4)
	{
		SDL_Rect rect = GetScreenRect(3);
		if (lblit.x > rect.x - 60 && lblit.y > rect.y - 60 && lblit.x < rect.w + (screen_3.x)*(-1) && lblit.y < rect.h + (screen_3.y)*(-1))
		{
			layer_list3.push(lblit);
		}
		
	}
	if ((viewport == 0 || viewport == 4) && number_of_views >= 4)
	{
		SDL_Rect rect = GetScreenRect(4);
		if (lblit.x > rect.x - 60 && lblit.y > rect.y - 60 && lblit.x < rect.w + (screen_4.x)*(-1) && lblit.y < rect.h + (screen_4.y)*(-1))
		{
		layer_list4.push(lblit);
		}
	}
}

void j1Viewport::LayerDrawQuad(const SDL_Rect rect, uint r, uint g, uint b, uint a, bool filled, int layer, int id, bool use_camera)
{
	view_quad q(layer, rect, r, g, b, a, filled, use_camera);

	SDL_Rect view = GetScreenSize();

	if (id == 0 || id == 1)
	{
		if (use_camera && !(-screen_1.x > rect.x + rect.w || -screen_1.x + view.w < rect.x || -screen_1.y > rect.y + rect.h || -screen_1.y + view.h < rect.y))
			quad_list1.push(q);
		else if (!use_camera)
			quad_list1.push(q);
	}
	if ((id == 0 || id == 2) && number_of_views >= 2)
	{
		if (use_camera && !(-screen_2.x > rect.x + rect.w || -screen_2.x + view.w < rect.x || -screen_2.y > rect.y + rect.h || -screen_2.y + view.h < rect.y))
			quad_list2.push(q);
		else if (!use_camera)
			quad_list2.push(q);
	}
	if ((id == 0 || id == 3) && number_of_views >= 4)
	{
		if (use_camera && !(-screen_3.x > rect.x + rect.w || -screen_3.x + view.w < rect.x || -screen_3.y > rect.y + rect.h || -screen_3.y + view.h < rect.y))
			quad_list3.push(q);
		else if (!use_camera)
			quad_list3.push(q);
	}
	if ((id == 0 || id == 4) && number_of_views >= 4)
	{
		if (use_camera && !(-screen_4.x > rect.x + rect.w || -screen_4.x + view.w < rect.x || -screen_4.y > rect.y + rect.h || -screen_4.y + view.h < rect.y))
			quad_list4.push(q);
		else if (!use_camera)
			quad_list4.push(q);
	}
}

void j1Viewport::LayerDrawLine(int x1, int y1, int x2, int y2, uint r, uint g, uint b, uint a, int layer, int id, bool use_camera)
{
	view_line l(layer, x1, y1, x2, y2, r, g, b, a, use_camera);

	SDL_Rect view = GetScreenSize();

	if (id == 0 || id == 1)
	{
		if (use_camera && !((-screen_1.x > x1 && -screen_1.x > x2) || (-screen_1.x + view.w < x1 && -screen_1.x + view.w < x2) || (-screen_1.y > y1 && -screen_1.y > y2) || (-screen_1.y + view.h < y1 && -screen_1.y + view.h < y2)))
			line_list1.push(l);
		else if (!use_camera)
			line_list1.push(l);

	}
	if ((id == 0 || id == 2) && number_of_views >= 2)
	{
		if (use_camera && !((-screen_2.x > x1 && -screen_2.x > x2) || (-screen_2.x + view.w < x1 && -screen_2.x + view.w < x2) || (-screen_2.y > y1 && -screen_2.y > y2) || (-screen_2.y + view.h < y1 && -screen_2.y + view.h < y2)))
			line_list2.push(l);
		else if (!use_camera)
			line_list3.push(l);
	}
	if ((id == 0 || id == 3) && number_of_views >= 4)
	{
		if (use_camera && !((-screen_3.x > x1 && -screen_3.x > x2) || (-screen_3.x + view.w < x1 && -screen_3.x + view.w < x2) || (-screen_3.y > y1 && -screen_3.y > y2) || (-screen_3.y + view.h < y1 && -screen_3.y + view.h < y2)))
			line_list3.push(l);
		else if (!use_camera)
			line_list3.push(l);
	}
	if ((id == 0 || id == 4) && number_of_views >= 4)
	{
		if (use_camera && !((-screen_4.x > x1 && -screen_4.x > x2) || (-screen_4.x + view.w < x1 && -screen_4.x + view.w < x2) || (-screen_4.y > y1 && -screen_4.y > y2) || (-screen_4.y + view.h < y1 && -screen_4.y + view.h < y2)))
			line_list4.push(l);
		else if (!use_camera)
			line_list4.push(l);
	}
}

void j1Viewport::LayerDrawCircle(int x1, int y1, int redius, uint r, uint g, uint b, uint a, int layer, int id, bool filled, bool use_camera)
{
	view_circle c(layer, x1, y1, redius, r, g, b, a, filled, use_camera);

	SDL_Rect view = GetScreenSize();

	if (id == 0 || id == 1)
	{
		if (use_camera && !(-screen_1.x > x1 + redius || -screen_1.x + view.w < x1 - redius || -screen_1.y > y1 + redius || -screen_1.y + view.h < y1 - redius))
			circle_list1.push(c);
		else if (!use_camera)
			circle_list1.push(c);
	}
	if ((id == 0 || id == 2) && number_of_views >= 2)
	{
		if (use_camera && !(-screen_2.x > x1 + redius || -screen_2.x + view.w < x1 - redius || -screen_2.y > y1 + redius || -screen_2.y + view.h < y1 - redius))
			circle_list2.push(c);
		else if (!use_camera)
			circle_list2.push(c);
	}
	if ((id == 0 || id == 3) && number_of_views >= 4)
	{
		if (use_camera && !(-screen_3.x > x1 + redius || -screen_3.x + view.w < x1 - redius || -screen_3.y > y1 + redius || -screen_3.y + view.h < y1 - redius))
			circle_list3.push(c);
		else if (!use_camera)
			circle_list3.push(c);
	}
	if ((id == 0 || id == 4) && number_of_views >= 4)
	{
		if (use_camera && !(-screen_4.x > x1 + redius || -screen_4.x + view.w < x1 - redius || -screen_4.y > y1 + redius || -screen_4.y + view.h < y1 - redius))
			circle_list4.push(c);
		else if (!use_camera)
			circle_list4.push(c);
	}
}

void j1Viewport::BlitFromQueue()
{
	// Screens

	if (number_of_views == 1)
	{
		while (!layer_list1.empty())
		{
			element_to_render curr = layer_list1.top();
			App->render->Blit(curr.texture, curr.x + (int)screen_1.x, curr.y + (int)screen_1.y, &curr.section, curr.speed, curr.angle, curr.pivot_x, curr.pivot_y);
			layer_list1.pop();
		}

		while (!quad_list1.empty())
		{
			view_quad curr = quad_list1.top();
			App->render->DrawQuad({ curr.rect.x + (int)screen_1.x, curr.rect.y + (int)screen_1.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, curr.a, curr.filled, curr.use_camera);
			quad_list1.pop();
		}

		while (!line_list1.empty())
		{
			view_line curr = line_list1.top();
			App->render->DrawLine(curr.x1 + (int)screen_1.x, curr.y1 + (int)screen_1.y, curr.x2 + screen_1.x, curr.y2 + screen_1.y, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
			line_list1.pop();
		}

		while (!circle_list1.empty())
		{
			view_circle curr = circle_list1.top();
			App->render->DrawCircle(curr.x1 + (int)screen_1.x, curr.y1 + (int)screen_1.y, curr.redius, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
			circle_list1.pop();
		}

	}
	else if (number_of_views == 4)
	{
		// Debug
		if (App->debug_mode)
		{
			App->render->DrawQuad(four_views_1, 255, 0, 0, 100);
			App->render->DrawQuad(four_views_2, 0, 255, 0, 100);
			App->render->DrawQuad(four_views_3, 0, 0, 255, 100);
			App->render->DrawQuad(four_views_4, 255, 0, 255, 100);
		}

		// View 1
		App->render->SetViewPort(four_views_1);

		while (!layer_list1.empty())
		{
			element_to_render curr = layer_list1.top();
			App->render->Blit(curr.texture, curr.x + (int)screen_1.x, curr.y + (int)screen_1.y, &curr.section, curr.speed, curr.angle, curr.pivot_x, curr.pivot_y);
			layer_list1.pop();
		}

		while (!quad_list1.empty())
		{
			view_quad curr = quad_list1.top();
			App->render->DrawQuad({ curr.rect.x + (int)screen_1.x, curr.rect.y + (int)screen_1.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, curr.a, curr.filled, curr.use_camera);
			quad_list1.pop();
		}

		while (!line_list1.empty())
		{
			view_line curr = line_list1.top();
			App->render->DrawLine(curr.x1 + (int)screen_1.x, curr.y1 + (int)screen_1.y, curr.x2 + (int)screen_1.x, curr.y2 + (int)screen_1.y, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
			line_list1.pop();
		}

		while (!circle_list1.empty())
		{
			view_circle curr = circle_list1.top();
			App->render->DrawCircle(curr.x1 + (int)screen_1.x, curr.y1 + (int)screen_1.y, curr.redius, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
			circle_list1.pop();
		}

		// View 2
		App->render->SetViewPort(four_views_2);

		while (!layer_list2.empty())
		{
			element_to_render curr = layer_list2.top();
			App->render->Blit(curr.texture, curr.x + (int)screen_2.x, curr.y + (int)screen_2.y, &curr.section, curr.speed, curr.angle, curr.pivot_x, curr.pivot_y);
			layer_list2.pop();
		}

		while (!quad_list2.empty())
		{
			view_quad curr = quad_list2.top();
			App->render->DrawQuad({ curr.rect.x + (int)screen_2.x, curr.rect.y + (int)screen_2.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, curr.a, curr.filled, curr.use_camera);
			quad_list2.pop();
		}

		while (!line_list2.empty())
		{
			view_line curr = line_list2.top();
			App->render->DrawLine(curr.x1 + (int)screen_2.x, curr.y1 + (int)screen_2.y, curr.x2 + (int)screen_2.x, curr.y2 + (int)screen_2.y, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
			line_list2.pop();
		}

		while (!circle_list2.empty())
		{
			view_circle curr = circle_list2.top();
			App->render->DrawCircle(curr.x1 + (int)screen_2.x, curr.y1 + (int)screen_2.y, curr.redius, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
			circle_list2.pop();
		}

		// View 3
		App->render->SetViewPort(four_views_3);

		while (!layer_list3.empty())
		{
			element_to_render curr = layer_list3.top();
			App->render->Blit(curr.texture, curr.x + (int)screen_3.x, curr.y + (int)screen_3.y, &curr.section, curr.speed, curr.angle, curr.pivot_x, curr.pivot_y);
			layer_list3.pop();
		}

		while (!quad_list3.empty())
		{
			view_quad curr = quad_list3.top();
			App->render->DrawQuad({ curr.rect.x + (int)screen_3.x, curr.rect.y + (int)screen_3.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, curr.a, curr.filled, curr.use_camera);
			quad_list3.pop();
		}

		while (!line_list3.empty())
		{
			view_line curr = line_list3.top();
			App->render->DrawLine(curr.x1 + (int)screen_3.x, curr.y1 + (int)screen_3.y, curr.x2 + (int)screen_3.x, curr.y2 + (int)screen_3.y, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
			line_list3.pop();
		}

		while (!circle_list3.empty())
		{
			view_circle curr = circle_list3.top();
			App->render->DrawCircle(curr.x1 + (int)screen_3.x, curr.y1 + (int)screen_3.y, curr.redius, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
			circle_list3.pop();
		}

		// View 4
		App->render->SetViewPort(four_views_4);

		while (!layer_list4.empty())
		{
			element_to_render curr = layer_list4.top();
			App->render->Blit(curr.texture, curr.x + (int)screen_4.x, curr.y + (int)screen_4.y, &curr.section, curr.speed, curr.angle, curr.pivot_x, curr.pivot_y);
			layer_list4.pop();
		}

		while (!quad_list4.empty())
		{
			view_quad curr = quad_list4.top();
			App->render->DrawQuad({ curr.rect.x + (int)screen_4.x, curr.rect.y + (int)screen_4.y, curr.rect.w, curr.rect.h }, curr.r, curr.g, curr.b, curr.a, curr.filled, curr.use_camera);
			quad_list4.pop();
		}

		while (!line_list4.empty())
		{
			view_line curr = line_list4.top();
			App->render->DrawLine(curr.x1 + (int)screen_4.x, curr.y1 + (int)screen_4.y, curr.x2 + (int)screen_4.x, curr.y2 + (int)screen_4.y, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
			line_list4.pop();
		}

		while (!circle_list4.empty())
		{
			view_circle curr = circle_list4.top();
			App->render->DrawCircle(curr.x1 + (int)screen_4.x, curr.y1 + (int)screen_4.y, curr.redius, curr.r, curr.g, curr.b, curr.a, curr.use_camera);
			circle_list4.pop();
		}

		App->render->ResetViewPort();
	}
}