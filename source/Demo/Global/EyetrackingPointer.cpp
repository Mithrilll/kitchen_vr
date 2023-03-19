#include "EyetrackingPointer.h"
#include "../../Framework/Utils.h"
#include "../Global/ObjectLabeling.h"

#include "../../Framework/Components/Players/VRPlayer.h"

REGISTER_COMPONENT(EyetrackingPointer);

using namespace Unigine;
using namespace Math;

void EyetrackingPointer::update()
{
	VRPlayer *player = VRPlayer::get();
	if (player == nullptr || player->isEyetrackingAvailable() == false || player->isEyetrackingValid() == false)
		return;

	// calculate gaze hit position
	Vec3 dir = (player->getFocusWorldPosition() - player->getHead()->getWorldPosition());
	dir.normalize();

	Vec3 p0 = player->getHead()->getWorldPosition();
	Vec3 p1 = p0 + dir * 10000.0;
	ObjectPtr hit_obj = World::getIntersection(p0, p1, 1, intersection);

	// show text
	ObjectLabeling *labeling = ObjectLabeling::get();
	if (labeling)
	{
		if (hit_obj && hit_obj->getProperty())
			last_hit_object = hit_obj;

		labeling->setTarget(last_hit_object, last_hit_object ? last_hit_object->getWorldPosition() : Vec3_zero, false);
	}
}
