#ifndef __j1VIEWPORT_H__
#define __j1VIEWPORT_H__

#include "j1Module.h"
#include "j1Scene.h"
#include <vector>
#include "j1PerfTimer.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"
#include <queue>


class element_to_render
{
public:

	element_to_render() {};

	element_to_render(int _layer, SDL_Texture* _texture, int _x, int _y, const SDL_Rect _section, int _viewport, float _speed, double _angle, int _pivot_x, int _pivot_y, float _scale)
	{
		layer = _layer; texture = _texture; x = _x; y = _y; section = { _section }; speed = _speed;
		angle = _angle; pivot_x = _pivot_x; pivot_y = _pivot_y; viewport = _viewport; scale = _scale;
	};

	bool operator ()(const element_to_render& l1, const element_to_render& l2)
	{
		return l1.layer > l2.layer;
	}

	SDL_Texture*	 texture = nullptr;
	int				 x = 0;
	int				 y = 0;
	SDL_Rect		 section = { 0,0,0,0 };
	float			 speed = 0.0f;
	double           angle = 0;
	int              pivot_x = 0;
	int              pivot_y = 0;
	int				 viewport = 0;
	int				 layer = 0;
	float			 scale = 1.0f;
};


class view_quad
{
public:
	view_quad() {};
	view_quad(int _layer, const SDL_Rect _rect, uint _r, uint _g, uint _b, uint _a, bool _filled, bool _use_camera)
	{
		layer = _layer; rect = { _rect }; r = _r; g = _g; b = _b; a = _a; filled = _filled; use_camera = _use_camera;
	}

	bool operator ()(const view_quad& l1, const view_quad& l2)
	{
		return l1.layer > l2.layer;
	}

	SDL_Rect rect = { 0,0 };
	uint    r = 0;
	uint    g = 0;
	uint    b = 0;
	uint    a = 0;
	bool     filled = false;
	bool     use_camera = true;
	int		 layer = 0;
};

struct view_line
{
public:
	view_line() {};
	view_line(int _layer, int _x1, int _y1, int _x2, int _y2, uint _r, uint _g, uint _b, uint _a, bool _use_camera)
	{
		layer = _layer; x1 = _x1; y1 = _y1; x2 = _x2; y2 = _y2; r = _r; g = _g; b = _b; a = _a; use_camera = _use_camera;
	}

	bool operator ()(const view_line& l1, const view_line& l2)
	{
		return l1.layer > l2.layer;
	}

	int			x1 = 0;
	int			y1 = 0;
	int			x2 = 0;
	int			y2 = 0;
	uint		r = 0;
	uint		g = 0;
	uint		b = 0;
	uint		a = 255;
	bool		use_camera = true;
	int			layer = 0;
};

struct view_circle
{
public:
	view_circle() {};
	view_circle(int _layer, int _x1, int _y1, int _redius, uint _r, uint _g, uint _b, uint _a, bool _filled, bool _use_camera)
	{
		layer = _layer; x1 = _x1; y1 = _y1; redius = _redius; r = _r; g = _g; b = _b; a = _a; filled = _filled, use_camera = _use_camera;
	}

	bool operator ()(const view_circle& l1, const view_circle& l2)
	{
		return l1.layer > l2.layer;
	}

	int			x1 = 0;
	int			y1 = 0;
	int			redius = 0;
	uint		r = 0;
	uint		g = 0;
	uint		b = 0;
	uint		a = 0;
	bool		filled = false;
	bool		use_camera = true;
	int			layer = 0;
};

class j1Viewport : public j1Module
{
public:

	j1Viewport();

	// Destructor
	virtual ~j1Viewport();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Screen Control and movement
	void ScreenMove(int id, float x, float y);
	void ScreenPosition(int id, int x, int y);
	void CenterScreen(int id, float x, float y);
	void ResetScreens();
	void KeepCameraOnBounds(int id);

	// Blit Functions and control
	void PushQueue(int layer, SDL_Texture* texture, int x, int y, const SDL_Rect section = { 0,0 }, int viewport = 0, float speed = 0.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX, float scale = 1.0f);
	void LayerDrawQuad(const SDL_Rect rect, uint r, uint g, uint b, uint a, bool filled, int layer = 0, int viewport = 0, bool use_camera = true);
	void LayerDrawLine(int x1, int y1, int x2, int y2, uint r, uint g, uint b, uint a = 255, int layer = 0, int viewport = 0, bool use_camera = true);
	void LayerDrawCircle(int x1, int y1, int redius, uint r, uint g, uint b, uint a = 255, int layer = 0, int viewport = 0, bool filled = false, bool use_camera = true);

	// Setters and Getters
	void SetViews(uint number);
	uint GetViews();
	SDL_Rect GetScreenRect(uint id);
	SDL_Rect GetScreenSize();
	iPoint GetScreenPosition(uint id);


private:

	// Blit in the layer order
	void BlitFromQueue();


public:
	//Screen Positions
	fPoint				 screen_1 = { 0,0 };
	fPoint				 screen_2 = { 0,0 };
	fPoint				 screen_3 = { 0,0 };
	fPoint				 screen_4 = { 0,0 };
	
	// Views
	uint				 number_of_views = 1;

	SDL_Rect			 four_views_1 = { 0,0,0,0 };
	SDL_Rect			 four_views_2 = { 0,0,0,0 };
	SDL_Rect			 four_views_3 = { 0,0,0,0 };
	SDL_Rect			 four_views_4 = { 0,0,0,0 };

private:

	// Layer Blit list
	std::priority_queue<view_line, std::vector<view_line>, view_line> line_list1;
	std::priority_queue<view_line, std::vector<view_line>, view_line> line_list2;
	std::priority_queue<view_line, std::vector<view_line>, view_line> line_list3;
	std::priority_queue<view_line, std::vector<view_line>, view_line> line_list4;

	std::priority_queue<element_to_render, std::vector<element_to_render>, element_to_render> layer_list1;
	std::priority_queue<element_to_render, std::vector<element_to_render>, element_to_render> layer_list2;
	std::priority_queue<element_to_render, std::vector<element_to_render>, element_to_render> layer_list3;
	std::priority_queue<element_to_render, std::vector<element_to_render>, element_to_render> layer_list4;

	std::priority_queue<view_quad, std::vector<view_quad>, view_quad> quad_list1;
	std::priority_queue<view_quad, std::vector<view_quad>, view_quad> quad_list2;
	std::priority_queue<view_quad, std::vector<view_quad>, view_quad> quad_list3;
	std::priority_queue<view_quad, std::vector<view_quad>, view_quad> quad_list4;

	std::priority_queue<view_circle, std::vector<view_circle>, view_circle> circle_list1;
	std::priority_queue<view_circle, std::vector<view_circle>, view_circle> circle_list2;
	std::priority_queue<view_circle, std::vector<view_circle>, view_circle> circle_list3;
	std::priority_queue<view_circle, std::vector<view_circle>, view_circle> circle_list4;

	// Win Size
	uint                 win_w = 0, win_h = 0;


	float				 scale = -1.0;

	j1PerfTimer			 timer;

	SDL_Rect			 screen_size = { 0,0,0,0 };

	int				   	 test = 0;
};

#endif // __j1VIEWPORT_H__