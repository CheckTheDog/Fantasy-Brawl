#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>
#include "j1Window.h"
#include "j1Collision.h"
#include "j1Viewport.h"
#include "j1Scene.h"
#include "j1UIScene.h"
#include "j1Gui.h"
#include "j1Transition.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.assign("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.assign(config.child("folder").child_value());
	redCollider = config.child("colliders").attribute("red").as_int();
	greenCollider = config.child("colliders").attribute("green").as_int();
	blueCollider = config.child("colliders").attribute("blue").as_int();

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	std::list<MapLayer*>::const_iterator item = data.layers.begin();

	for(; item != data.layers.end(); ++item)
	{
		layerr = *item;

		if(layerr->properties.Get("Nodraw") != 0)
			continue;

		iPoint playerpos1map = WorldToMap(App->scene->player1->Entityinfo.entitycoll->rect.x, App->scene->player1->Entityinfo.entitycoll->rect.y);
		int playerpos1 = layerr->Get(playerpos1map.x, playerpos1map.y);
		int playerpos1_left = layerr->Get(playerpos1map.x - 1, playerpos1map.y);
		int playerpos1_right = layerr->Get(playerpos1map.x + 1, playerpos1map.y);
		int playerpos1_top = layerr->Get(playerpos1map.x, playerpos1map.y - 1);

		iPoint playerpos2map = WorldToMap(App->scene->player2->Entityinfo.entitycoll->rect.x, App->scene->player2->Entityinfo.entitycoll->rect.y);
		int playerpos2 = layerr->Get(playerpos2map.x, playerpos2map.y);
		int playerpos2_left = layerr->Get(playerpos2map.x - 1, playerpos2map.y);
		int playerpos2_right = layerr->Get(playerpos2map.x + 1, playerpos2map.y);
		int playerpos2_top = layerr->Get(playerpos2map.x, playerpos2map.y - 1);

		iPoint playerpos3map = WorldToMap(App->scene->player3->Entityinfo.entitycoll->rect.x, App->scene->player3->Entityinfo.entitycoll->rect.y);
		int playerpos3 = layerr->Get(playerpos3map.x, playerpos3map.y);
		int playerpos3_left = layerr->Get(playerpos3map.x - 1, playerpos3map.y);
		int playerpos3_right = layerr->Get(playerpos3map.x + 1, playerpos3map.y);
		int playerpos3_top = layerr->Get(playerpos3map.x, playerpos3map.y - 1);

		iPoint playerpos4map = WorldToMap(App->scene->player4->Entityinfo.entitycoll->rect.x, App->scene->player4->Entityinfo.entitycoll->rect.y);
		int playerpos4 = layerr->Get(playerpos4map.x, playerpos4map.y);
		int playerpos4_left = layerr->Get(playerpos4map.x - 1, playerpos4map.y);
		int playerpos4_right = layerr->Get(playerpos4map.x + 1, playerpos4map.y);
		int playerpos4_top = layerr->Get(playerpos4map.x, playerpos4map.y - 1);

		for(int y = 0; y < data.height; ++y)
		{
			for(int x = 0; x < data.width; ++x)
			{
			
				int tile_id = layerr->Get(x, y);
				if(tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);

					if (App->ui_scene->actual_menu == SELECTION_MENU && App->transition->doingMenuTransition)
					{
						//SDL_SetTextureAlphaMod(tileset->texture, App->gui->alpha_value);
						continue;
					}

					SDL_Rect r = tileset->GetTileRect(tile_id);
					iPoint pos = MapToWorld(x, y);

					if (layerr->name == "ground")
					{
						App->view->PushQueue(1,tileset->texture, pos.x, pos.y, r);
					}
					else if (layerr->name == "beauty_1")
					{
						App->view->PushQueue(2, tileset->texture, pos.x, pos.y, r);
					}
					else if (layerr->name == "beauty_2")
					{
						App->view->PushQueue(3, tileset->texture, pos.x, pos.y, r);
					}
					else if (layerr->name == "shadows_1")
					{
						App->view->PushQueue(4, tileset->texture, pos.x, pos.y, r);
					}
					else if (layerr->name == "walls")
					{
						if (playerpos1 > 0 || playerpos1_left > 0 || playerpos1_right > 0)
						{
							if(playerpos1_top > 0)
							App->scene->player1->PlayerPrintOnTop = true;
							else
							App->scene->player1->PlayerPrintOnTop = false;
 						}
						else
						{
							App->scene->player1->PlayerPrintOnTop = true;
						}
						
						if (playerpos2 > 0 || playerpos2_left > 0 || playerpos2_right > 0)
						{
							if (playerpos2_top > 0)
								App->scene->player2->PlayerPrintOnTop = true;
							else
								App->scene->player2->PlayerPrintOnTop = false;
						}
						else
						{
							App->scene->player2->PlayerPrintOnTop = true;
						}

						if (playerpos3 > 0 || playerpos3_left > 0 || playerpos3_right > 0)
						{
							if (playerpos3_top > 0)
								App->scene->player3->PlayerPrintOnTop = true;
							else
								App->scene->player3->PlayerPrintOnTop = false;
						}
						else
						{
							App->scene->player3->PlayerPrintOnTop = true;
						}

						if (playerpos4 > 0 || playerpos4_left > 0 || playerpos4_right > 0)
						{
							if (playerpos4_top > 0)
								App->scene->player4->PlayerPrintOnTop = true;
							else
								App->scene->player4->PlayerPrintOnTop = false;
						}
						else
						{
							App->scene->player4->PlayerPrintOnTop = true;
						}

						App->view->PushQueue(6, tileset->texture, pos.x, pos.y, r);

					}
					else
					{
						App->view->PushQueue(8, tileset->texture, pos.x, pos.y, r);
					}
						

				}
			}
		}
	}
}

int Properties::Get(const char* value, int default_value) const
{
	std::list<Property*>::const_iterator item = list.begin();

	while(item != list.end())
	{
		if((*item)->name == value)
			return (*item)->value;
		item++;
	}

	return default_value;
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	std::list<TileSet*>::const_iterator item = data.tilesets.begin();
	TileSet* set = *item;

	while(item != data.tilesets.end())
	{
		if(id < (*item)->firstgid)
		{
			set = *prev(item,1);
			break;
		}
		set = *item;
		item++;
	}

	return set;
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0,0);

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		
		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int( (x / half_width + y / half_height) / 2) - 1;
		ret.y = int( (y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	std::list<TileSet*>::const_iterator item = data.tilesets.begin();

	while(item != data.tilesets.end())
	{
		App->tex->UnLoad((*item)->texture);
		delete *item;
		item++;
	}
	data.tilesets.clear();

	// Remove all layers
	std::list<MapLayer*>::const_iterator item2 = data.layers.begin();

	while(item2 != data.layers.end())
	{
		delete *item2;
		item2++;
	}
	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	std::string tmp;
	tmp.assign(folder.data());
	tmp += file_name;
	
	pugi::xml_parse_result result = map_file.load_file(tmp.data());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.push_back(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if(ret == true)
			data.layers.push_back(lay);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		std::list<TileSet*>::const_iterator item = data.tilesets.begin();
	
		while(item != data.tilesets.end())
		{
			TileSet* s = *item;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.data(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item++;
		}

		std::list<MapLayer*>::const_iterator item_layer = data.layers.begin();

		while(item_layer != data.layers.end())
		{
			MapLayer* l = *item_layer;
			LOG("Layer ----");
			LOG("name: %s", l->name.data());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer++;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		std::string bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.size() > 0)
		{
			std::string red, green, blue;
			bg_color.substr(1, 2);
			bg_color.substr(3, 4);
			bg_color.substr(5, 6);

			int v = 0;

			sscanf_s(red.data(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.data(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.data(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		std::string orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.assign(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.data(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	LoadProperties(node, layer->properties);
	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for(pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

// Load a group of properties from a node and fill a list with it
bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if(data != NULL)
	{
		pugi::xml_node prop;

		for(prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();

			properties.list.push_back(p);
		}
	}

	return ret;
}


bool j1Map::ColliderDrawer()
{
	bool ret = true;

	std::list<MapLayer*>::const_iterator layer = data.layers.begin();

	for (; layer != data.layers.end(); ++layer)
	{

		if ((*layer)->properties.Get("Nodraw") == 1)
		{

			for (int y = 0; y < data.height; ++y)
			{
				for (int x = 0; x < data.width; ++x)
				{
					int tile_id = (*layer)->Get(x, y);

					if (tile_id > 0)
					{
						TileSet* tileset = GetTilesetFromTileId(tile_id);

						if (tile_id > tileset->firstgid)
						{

							iPoint pos = MapToWorld(x, y);

							if (data.width == 50 && data.height == 50)
							{
								if (tile_id == 3)
									App->coll->AddCollider({ pos.x,pos.y,data.tile_width,data.tile_height }, COLLIDER_TYPE::COLLIDER_FLOOR, this);
								//else if (tile_id == greenCollider)
								//	LOG("");
								else if (tile_id == 4)
									App->coll->AddCollider({ pos.x,pos.y,data.tile_width,data.tile_height }, COLLIDER_TYPE::COLLIDER_WATER, this);
							}
							else
							{
								if (tile_id == redCollider)
									App->coll->AddCollider({ pos.x,pos.y,data.tile_width,data.tile_height }, COLLIDER_TYPE::COLLIDER_FLOOR, this);
								//else if (tile_id == greenCollider)
								//	LOG("");
								else if (tile_id == blueCollider)
									App->coll->AddCollider({ pos.x,pos.y,data.tile_width,data.tile_height }, COLLIDER_TYPE::COLLIDER_WATER, this);
							}
						}
					}
				}
			}
		}
	}

	return ret;
}