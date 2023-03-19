#include "PlateZone.h"
#include "ProductBaseSettings.h"
#include <UnigineWorlds.h>

REGISTER_COMPONENT(PlateZone);

using namespace Unigine;

void PlateZone::switch_state()
{
	is_working = !is_working;
	for (int i = 0; i < nodes_inside_trigger.size(); i++)
	{
		auto component = getComponent<ProductBaseSettings>(nodes_inside_trigger[i]);
		if (component)
		{
			if(is_working)
				component->statr_cooking();
			else
				component->stop_cooking();
		}
	}

	if (is_working)
		Log::message("Enabled");
	else
		Log::message("Disabled");
}

void PlateZone::init()
{
	is_working = false;

	trigger = checked_ptr_cast<WorldTrigger>(node);

	// set the callback to be executed when an object enters the area
	if (trigger)
	{
		trigger->addEnterCallback(MakeCallback(this, &PlateZone::on_enter));
		trigger->addLeaveCallback(MakeCallback(this, &PlateZone::on_leave));
	}
}

void PlateZone::on_enter(NodePtr target)
{
	auto component = getComponent<ProductBaseSettings>(target);
	if (component && is_working == true)
	{
			component->statr_cooking();
	}
	nodes_inside_trigger.push_back(target);
}

void PlateZone::on_leave(NodePtr target)
{
	auto component = getComponent<ProductBaseSettings>(target);
	if (component)
	{
		component->stop_cooking();
	}
	int i = 0;

	while (i < nodes_inside_trigger.size() && nodes_inside_trigger[i] != target)
		i++;

	nodes_inside_trigger.removeFast(i);

}
