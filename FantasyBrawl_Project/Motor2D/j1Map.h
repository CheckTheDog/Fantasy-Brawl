#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include <list>
#include "p2Point.h"
#include "j1Module.h"

// ----------------------------------------------------
struct Properties
{
	struct Property
	{
		std::string name;
		int value;
	};

	~Properties()
	{
		std::list<Property*>::const_iterator item;
		item = list.begin();

		while(item != list.end())
		{
			delete *item;
			item++;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	std::list <Property*>	list;
};

// ----------------------------------------------------
struct MapLayer
{
	std::string	name;
	int			width;
	int			height;
	uint*		data;
	Properties	properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE_ARRAY(data);
	}

	inline uint Get(int x, int y) const
	{
		if (((y*width) + x) < 0 || width*height < ((y*width) + x))
			return 0;
		else
		return data[(y*width) + x];
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	std::string			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width = 0;
	int					height = 0;
	int					tile_width = 0;
	int					tile_height = 0;
	SDL_Color			background_color;
	MapTypes			type;
	std::list <TileSet*>	tilesets;
	std::list <MapLayer*>	layers;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	bool ColliderDrawer();

public:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;
	MapLayer* layerr;

private:

	pugi::xml_document	map_file;
	std::string			folder;
	bool				map_loaded;

	// --- Collider id's ---
	int redCollider = 0;
	int greenCollider = 0;
	int blueCollider = 0;
};

#endif // __j1MAP_H__