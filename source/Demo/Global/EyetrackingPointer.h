#pragma once
#include <UnigineWorld.h>
#include <UnigineComponentSystem.h>

class EyetrackingPointer: public Unigine::ComponentBase
{
public:
	COMPONENT(EyetrackingPointer, Unigine::ComponentBase);
	COMPONENT_UPDATE(update);

	// property
	PROP_NAME("eyetracking_pointer");
	PROP_AUTOSAVE(0);

protected:
	void update();

private:
	Unigine::WorldIntersectionPtr intersection = Unigine::WorldIntersection::create();
	Unigine::ObjectPtr last_hit_object;
};