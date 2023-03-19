#pragma once
#include <UnigineWorld.h>
#include "../../Framework/Components/VRInteractable.h"

class ObjLaserPointer : public VRInteractable
{
public:
	COMPONENT(ObjLaserPointer, VRInteractable);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update, 1);

	// property
	PROP_NAME("laser_pointer");
	PROP_AUTOSAVE(0);

	// parameters
	PROP_PARAM(Node, laser);
	PROP_PARAM(Node, laser_ray);
	PROP_PARAM(Node, laser_hit);

	// interact methods
	void useIt(VRPlayer* player, int hand_num) override;
	void grabIt(VRPlayer* player, int hand_num) override;
	void throwIt(VRPlayer* player, int hand_num) override;

protected:
	void init();
	void update();

private:
	Unigine::ObjectTextPtr obj_text;
	Unigine::Math::Mat4 laser_ray_mat;
	int grabbed;
	Unigine::WorldIntersectionPtr intersection = Unigine::WorldIntersection::create();
};