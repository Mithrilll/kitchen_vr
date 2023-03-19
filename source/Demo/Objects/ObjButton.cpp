#include "ObjButton.h"
#include <UnigineEditor.h>
#include <UnigineGame.h>

REGISTER_COMPONENT(ObjButton);

using namespace Unigine;
using namespace Math;

void ObjButton::init()
{
	// "Boom" animation init
	button_reaction_text = checked_ptr_cast<ObjectText>(button_text.get());
	button_pos = button_reaction_text->getWorldPosition();
	button_reaction_text->setTextColor(vec4(1, 1, 1, 0));
	button_reaction_percent = 1;

	plate_zone = getComponent<PlateZone>(plate);
}

void ObjButton::update()
{
	if (button_reaction_percent < 1)
	{
		button_reaction_percent = saturate(button_reaction_percent + Game::getIFps());
		//button_reaction_text->setWorldPosition(button_pos + Vec3_up * text_up_speed * button_reaction_percent);
		//button_reaction_text->setTextColor(vec4(1, 1, 1, 1 - button_reaction_percent));
	}
}

void ObjButton::grabIt(VRPlayer* player, int hand_num)
{
	// play animation from start
	button_reaction_percent = 0;

	if(plate_zone)
		plate_zone->switch_state();
}