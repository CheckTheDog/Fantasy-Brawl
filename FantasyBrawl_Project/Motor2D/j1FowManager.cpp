#include "j1FowManager.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "SDL/include/SDL_scancode.h"

j1FowManager::j1FowManager()
{
}

j1FowManager::~j1FowManager()
{
	//Delete all 2D Fog data containers
	if (visibility_map != nullptr && visibility_map != debug_map)
	{
		delete[] visibility_map;
		visibility_debug_holder = nullptr;
	}
	else if (visibility_debug_holder != nullptr)
		delete[] visibility_debug_holder;
	
	
	if (debug_map != nullptr)
		delete[] debug_map;
}

bool j1FowManager::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1FowManager::Start()
{
	meta_FOW = App->tex->Load("maps/FOW_meta_sheet.png");

	return true;
}

bool j1FowManager::Update(float dt)
{
	if (debug == false)
	{
		// We manage fow_entities that manipulate the visibility map 
		ManageEntitiesFOWManipulation();
	}
	//Manage the entities visibibility, check if they are visible or not
	//depending on their position
	ManageEntitiesVisibility();

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug = !debug;

		// If we enter debug mode, our visibility map should be clear.
		// We will point to a clear visibility map (debug_map) so all calls depending on visibility_map don't
		// need further management. But before we store our current visibility map!
		if (debug == true)
		{
			//To keep the pointer to the visibility map we use our debug_holder;
			visibility_debug_holder = visibility_map;
			visibility_map = debug_map;
		}
		else // Debug == false
		{
			visibility_map = visibility_debug_holder;
		}
	}

	return true;
}

bool j1FowManager::CleanUp()
{
	App->tex->UnLoad(meta_FOW);
	meta_FOW = nullptr;

	for (std::list<FOW_Data*>::iterator item = fow_entities.begin(); item != fow_entities.end(); ++item)
	{
		if((*item)!=nullptr)
		delete (*item);
	}

	fow_entities.clear();
	return true;
}

//Create and add a FOW_Data to the list of fow_entities
FOW_Data * j1FowManager::CreateFOWEntity(iPoint position, bool provides_visibility)
{
	FOW_Data* ret = nullptr;

	ret = new FOW_Data(position, provides_visibility);

	if(ret != nullptr)
	fow_entities.push_back(ret);

	return ret;
}

//Delete and remove a FOW_Data to the list of fow_entities
bool j1FowManager::DestroyFOWEntity(FOW_Data* to_destroy)
{
	bool ret = false;

	for (std::list<FOW_Data*>::iterator item = fow_entities.begin(); item != fow_entities.end(); ++item)
	{
		if (to_destroy == (*item))
		{
			delete (*item);
			fow_entities.remove(*item);
			ret = true;
			break;
		}
	}

	return ret;
}

//Set the visibility map
void j1FowManager::SetVisibilityMap(uint w, uint h)
{
	if (visibility_map != nullptr)
	{
		if (visibility_debug_holder == visibility_map)
		{
			visibility_debug_holder = nullptr;
		}
		else
		{
			delete[] visibility_debug_holder;
			visibility_debug_holder = nullptr;
		}

		if (visibility_map != debug_map)
		{
			delete[] visibility_map;
			visibility_map = nullptr;
		}
	}

	if (debug_map != nullptr)
	{
		delete[] debug_map;
		debug_map = nullptr;
	}

	width = w;
	height = h;

	//Visibility map will contain the 3 basic states of logic in FOW
	visibility_map = new int8_t [width*height];
	memset(visibility_map, 0,width*height);

	// Keep a totally clear map for debug purposes
	debug_map = new int8_t[width*height];
	memset(debug_map, 255, width*height);
}

int8_t j1FowManager::GetVisibilityTileAt(const iPoint& pos) const
{
	// Utility: return the visibility value of a tile
	if (CheckBoundaries(pos)) // Since both maps will have the same size we can check with the main one
			return visibility_map[(pos.y * width) + pos.x];
	else
		return 0;
}

SDL_Rect& j1FowManager::GetFOWMetaRect(FOW_TileState state)
{
	SDL_Rect ret;
	uint real_sprite_pos = uint(state) - (uint(FOW_TileState::UNVISITED)); // tile inside the FOWmeta spritesheet

	// 64 is the width and height between different tiles in the spritesheet, thats done this way because it's the
	// total rect we use to print tiles from the tileset. The Map module should provide you that data but for this
	//Implementation you must not have calls to 
	ret.w = 64; 
	ret.h = 64;
	ret.x = real_sprite_pos * ret.w; 
	ret.y = 0;

	return ret;
}

void j1FowManager::SetVisibilityTile(iPoint pos, FOW_TileState state)
{
	if (CheckBoundaries(pos))
			visibility_map[(pos.y * width) + pos.x] = (int8_t)state;
}

// We will manage the bool is_visible in the fow_entities, entities from the entity manager should check this value of
// their own FOW_Data to determine if they're visible
void j1FowManager::ManageEntitiesVisibility()
{
	for (std::list<FOW_Data*>::iterator item = fow_entities.begin(); item != fow_entities.end(); ++item)
	{
		int8_t st = GetVisibilityTileAt((*item)->position);
		// If the tile isn't visible or a smoothing tile from black to Fogged
		if (st != (int8_t)FOW_TileState::UNVISITED)
		{
			(*item)->is_visible = true;
		} else {
			(*item)->is_visible = false;
		}
	}
}

void j1FowManager::ManageEntitiesFOWManipulation()
{
	//Iterate all fow_entities
	for (std::list<FOW_Data*>::iterator item = fow_entities.begin(); item != fow_entities.end(); ++item)
	{
		// If an entity provides visibility and has moved
		if ((*item)->provides_visibility == true && (*item)->moved_in_map == true)
		{
			//We store the LOS of the current entity, since the LOS will change this will be our previous LOS
			std::list<iPoint> prev_LOS = (*item)->LOS;

			//Since the Entity moved, we update the LOS position and make the tiles contained inside it visible 
			for (std::list<iPoint>::iterator tile = (*item)->LOS.begin(); tile != (*item)->LOS.end(); tile++)
			{
				// TODO 3.5 Since we moved we needupdate the position of the tiles in the LOS (Line of Sight), 
				// effectively moving all the LOS area to the current position we are in.
				// Remember that each time an entity moves from one tile to another its variable motion is updated with
				// the total movement in tile distance
				(*tile) += (*item)->motion;

				// TODO 3.5 Set The Visibility of the tile we just moved. We need to go to the SAME
				// tile in the VISIBILITY MAP and make it VISIBLE. Look at the header and search which 
				// private function will allow you to do so
				SetVisibilityTile((*tile),FOW_TileState::VISIBLE);
			}

			(*item)->moved_in_map = false;

			//Now iterate the tiles that the entity left behind to determine its state 
			//(fogged or unvisited depending of in if we leave a scouting trail)
			
			//TODO 4 UNCOMMENT FROM HERE: --------------------------------------------------------
			for (std::list<iPoint>::const_iterator tile = prev_LOS.cbegin(); tile != prev_LOS.end(); tile++)
			{
				if (TileInsideList((*tile), (*item)->LOS) == false)
				{
						SetVisibilityTile((*tile), FOW_TileState::UNVISITED);
				}
			}
		}
	}
}

bool j1FowManager::TileInsideList(iPoint tile, const std::list<iPoint>& list_checked) const
{
	for (std::list<iPoint>::const_iterator item = list_checked.cbegin(); item != list_checked.end(); item++)
	{
		if ((*item).x == tile.x && (*item).y == tile.y)
		{
			return true;
		}
	}

	return false;
}

void j1FowManager::ResetVisibilityMap()
{
	// We simply set the map again this way other modules can call and it will be
	// easier to understand rather than setting the the map again manually
	SetVisibilityMap(width,height);

	if (debug)
	{
		debug = false;
	}
}


bool j1FowManager::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x < (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

std::list<iPoint> j1FowManager::CreateFrontierRect(uint w, uint h, iPoint center)
{
	std::list<iPoint> frontier_to_fill;

	iPoint st_pos = { center.x - (int)(w/2), center.y - (int)(h / 2) };
	iPoint final_pos = { st_pos.x + (int)w, (int)st_pos.y + (int)h};

	// We iterate to find the iPoints that ARE part of the frontier
	for (int j = st_pos.y; j < final_pos.y; ++j)
	{
		int i = st_pos.x;
		for (; i < final_pos.x - 1; ++i) //check the explanations below to understand why i < w -1
		{
			// Since it's a rectangle some assumptions can be made: i== 0 will always be at the 
			// frontier as the left-most tile, i == w -1 will be for the right-most tile
			if (i == st_pos.x || j == st_pos.y || j == final_pos.y -1)
			{
				frontier_to_fill.push_back({ i, j });
			}
		}
		// We exit the for loop when i == w -1
		frontier_to_fill.push_back({ i, j });
	}
	
	return frontier_to_fill;
}

std::list<iPoint> j1FowManager::CreateFrontierSquare(uint radius, iPoint center)
{
	return CreateFrontierRect(radius*2, radius*2, center);
}

std::list<iPoint> j1FowManager::FillFrontier(const std::list<iPoint>& frontier)
{
	std::list<iPoint> shape_to_fill;

	//Iterate the frontier
	for (std::list<iPoint>::const_iterator curr = frontier.cbegin(); curr != frontier.cend(); ++curr)
	{
		if ((*std::next(curr)).y == (*curr).y)
		{
			int w = (*std::next(curr)).x - (*curr).x;
			for (int i = 0; i < w; ++i)
			{
				shape_to_fill.push_back({ (*curr).x + i ,(*curr).y });
			}
		}
		else
		{
			shape_to_fill.push_back(*curr);
		}
	}

	return shape_to_fill;
}

FOW_Data::FOW_Data(iPoint position, bool provides_visibility):
	position(App->map->WorldToMap(position.x, position.y)),
	provides_visibility(provides_visibility)
{}


void FOW_Data::SetPos(iPoint new_pos)
{
	new_pos = App->map->WorldToMap(new_pos.x, new_pos.y);
	// Our position is always in map reference
	if (position != new_pos)
	{
		moved_in_map = true;
		motion = { (new_pos.x - position.x), (new_pos.y - position.y) };
		position = new_pos;

	}
}

bool j1FowManager::CollectBushData()
{
	bool ret = true;

	MapData data = App->map->data;
	std::list<MapLayer*>::const_iterator layer = data.layers.begin();

	// Initiallize the bush meta data map
	if (bush_meta_map != nullptr)
	{
		delete bush_meta_map;
		bush_meta_map = nullptr;
	}
	uint w = (*layer)->width;
	uint h = (*layer)->height;
	
	bush_meta_map = new int8_t[w*h];
	memset(bush_meta_map, 0, w*h);
	//Iterate layers
	for (; layer != data.layers.end(); ++layer)
	{
		if ((*layer)->properties.Get("isBush") == 1)
		{
			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tile_id = (*layer)->Get(x, y);

					if (tile_id > 0)
					{
						TileSet* tileset = App->map->GetTilesetFromTileId(tile_id);

						if (tile_id > tileset->firstgid)
						{
							int i = (y*(*layer)->width) + x;

							TileSet* tileset = (tile_id > 0) ? App->map->GetTilesetFromTileId(tile_id) : NULL;

							if (tileset != NULL)
							{
								bush_meta_map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
							}

						}
					}
				}
			}
		}
	}

	return ret;
}
