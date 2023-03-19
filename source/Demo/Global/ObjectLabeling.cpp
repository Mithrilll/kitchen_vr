#include "ObjectLabeling.h"
#include "../../Framework/Utils.h"

#include "../../Framework/Components/Players/VRPlayer.h"

REGISTER_COMPONENT(ObjectLabeling);

using namespace Unigine;
using namespace Math;

ObjectLabeling* ObjectLabeling::instance;

ObjectLabeling* ObjectLabeling::get()
{
	return instance;
}

void ObjectLabeling::setTarget(const ObjectPtr &object, const Vec3 &hit_point, bool shift_text_up)
{
	if (object)
	{
		obj_text->setEnabled(1);
		obj_text->setText(object->getName());
		obj_text->setWorldDirection(vec3(hit_point - VRPlayer::get()->getHead()->getWorldPosition()), vec3_up);
		float radius = obj_text->getBoundSphere().radius;
		Vec3 shift = shift_text_up ? Vec3_up * radius : Vec3_zero;
		obj_text->setWorldPosition(hit_point - Vec3(getWorldDirectionX(obj_text) * radius) + shift);
	} else
		obj_text->setEnabled(0);
}

void ObjectLabeling::init()
{
	obj_text = checked_ptr_cast<ObjectText>(text.get());
	obj_text->setEnabled(0);
	instance = this;
}

void ObjectLabeling::update()
{
	obj_text->setEnabled(0);
}
