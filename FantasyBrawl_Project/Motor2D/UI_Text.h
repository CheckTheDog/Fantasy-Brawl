#ifndef __UI_TEXT__
#define __UI_TEXT__

#include "UI_element.h"
#include "SDL/include/SDL_pixels.h"

struct _TTF_Font;

class Text : public UI_element
{
public:

	Text()
	{}

	Text(char* text, int x, int y, _TTF_Font* font, SDL_Color color, j1Module* callback) : UI_element(x, y, element_type::TEXT, { 0, 0, 0, 0 }, callback, nullptr),
		text(text),
		font(font),
		color(color)
	{
		createTexture();
	}

	~Text();

	void createTexture();
	void setColor(SDL_Color newColor);
	void setOutlineColor(SDL_Color newColor);
	void setOutlined(bool isOutlined);

	std::string getText() const;
	void setText(const char* string);
	void setText(const std::string string);
	int getLength() const;

	void BlitElement();

public:

	SDL_Color color;
	uint tex_width;
	uint tex_height;

private:

	std::string text;
	_TTF_Font* font = nullptr;
	bool outlined = false;
	SDL_Texture* outline = nullptr;
	SDL_Color outline_color = { 0, 0, 0, 255 };
	iPoint outline_offset;
};

#endif // !__UI_TEXT__

