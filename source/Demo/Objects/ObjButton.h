#pragma once
#include "../../Framework/Components/VRInteractable.h"
#include "../PlateZone.h"

class ObjButton : public VRInteractable
{
public:
	COMPONENT(ObjButton, VRInteractable);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);

	// property
	PROP_NAME("button");
	//PROP_AUTOSAVE(0);

	// parameters
	PROP_PARAM(Node, button_text);
	PROP_PARAM(Float, text_up_speed, 0.2f);
	
	PROP_PARAM(Node, plate);

	// interact methods
	void grabIt(VRPlayer* player, int hand_num) override;

protected:
	void init();
	void update();

private:
	Unigine::ObjectTextPtr button_reaction_text;
	Unigine::Math::Vec3 button_pos;
	float button_reaction_percent; // [0,1]

	PlateZone* plate_zone;
};