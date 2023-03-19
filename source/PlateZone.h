#pragma once
#include <UnigineComponentSystem.h>
#include <UnigineWorlds.h>

class PlateZone : public Unigine::ComponentBase
{
public:
	COMPONENT_DEFINE(PlateZone, Unigine::ComponentBase);
	COMPONENT_INIT(init);

	void switch_state();

protected:
	void init();

	void on_enter(Unigine::NodePtr target);
	void on_leave(Unigine::NodePtr target);

private:
	Unigine::Vector<Unigine::NodePtr> nodes_inside_trigger;
	bool is_working;

	Unigine::WorldTriggerPtr trigger;
};