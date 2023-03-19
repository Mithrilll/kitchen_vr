#include "ObjLaserPointer.h"
#include "../../Framework/Utils.h"
#include "../Global/ObjectLabeling.h"

// we don't want to get intersection with himself, so
// we use offset
#define RAY_OFFSET 0.05f

REGISTER_COMPONENT(ObjLaserPointer);

using namespace Unigine;
using namespace Math;

void ObjLaserPointer::init()
{
	laser_ray_mat = laser_ray->getTransform();
	grabbed = 0;
	laser->setEnabled(false);
}

void ObjLaserPointer::update()
{
	if (laser->isEnabled())
	{
		// calculate laser hit position

		laser_ray->setTransform(laser_ray_mat);

		Vec3 dir = Vec3(laser_ray->getWorldDirection(Math::AXIS_Y));
		Vec3 p0 = laser_ray->getWorldPosition() + dir * RAY_OFFSET;
		Vec3 p1 = p0 + dir * 1000;
		ObjectPtr hit_obj = World::getIntersection(p0, p1, 1, intersection);
		if (hit_obj)
		{
			laser_ray->setScale(vec3(laser_ray->getScale().x, (float)length(intersection->getPoint() - p0) + RAY_OFFSET, laser_ray->getScale().z));
			laser_hit->setWorldPosition(intersection->getPoint());
			laser_hit->setEnabled(1);
		}
		else
			laser_hit->setEnabled(0);

		// rotate ray plane to player's head (Y fixed, Z floats)
		setWorldDirectionY(laser_ray, laser_ray->getWorldDirection(Math::AXIS_Y), vec3(VRPlayer::get()->getHead()->getWorldPosition() - laser_ray->getWorldPosition()));

		// show text
		ObjectLabeling *labeling = ObjectLabeling::get();
		if (labeling)
		{
			if (hit_obj && hit_obj->getProperty() && grabbed)
				labeling->setTarget(hit_obj, laser_hit->getWorldPosition(), true);
		}
	}
}

void ObjLaserPointer::useIt(VRPlayer* player, int hand_num)
{
	// laser pointer switch on/off (any laser pointer object)
	laser->setEnabled(!laser->isEnabled());
}

void ObjLaserPointer::grabIt(VRPlayer* player, int hand_num)
{
	grabbed = 1;
}

void ObjLaserPointer::throwIt(VRPlayer* player, int hand_num)
{
	grabbed = 0;
}