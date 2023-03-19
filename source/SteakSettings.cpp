#include "SteakSettings.h"
#include <UnigineGame.h>

REGISTER_COMPONENT(SteakSettings);

using namespace Unigine;

void SteakSettings::product_init()
{
	time_to_cook_left = cooking_time.get();
	is_cooking = false;
}

void SteakSettings::update()
{
	if (is_cooking && time_to_cook_left > 0.0f)
	{
		time_to_cook_left -= Game::getIFps();
		Log::message("Left %f seconds\n", time_to_cook_left);
		if (time_to_cook_left <= 0.0f)
		{
			switch (current_state)
			{
			case RAW:
				time_to_cook_left = cooking_time.get();
				object->setMaterial(prepared_material.get(), "*");
				current_state = PREPARED;
				break;
			case PREPARED:
				object->setMaterial(burnt_material.get(), "*");
				current_state = BURNT;
				break;
			case BURNT:
				break;
			}
		}
	}
}