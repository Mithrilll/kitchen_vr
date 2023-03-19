#pragma once
#include <UnigineWorld.h>
#include <UnigineComponentSystem.h>

class ObjectLabeling : public Unigine::ComponentBase
{
public:
	COMPONENT(ObjectLabeling, Unigine::ComponentBase);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update, -1);

	// property
	PROP_NAME("object_labeling");
	PROP_AUTOSAVE(0);

	// parameters
	PROP_PARAM(Node, text);

public:
	static ObjectLabeling * get();
	void setTarget(const Unigine::ObjectPtr &object, const Unigine::Math::Vec3& hit_point, bool shift_text_up);

protected:
	void init();
	void update();

private:
	Unigine::ObjectTextPtr obj_text;
	static ObjectLabeling *instance;
};