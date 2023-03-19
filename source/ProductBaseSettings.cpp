#include "ProductBaseSettings.h"

using namespace Unigine;

void ProductBaseSettings::init()
{
	current_state = RAW;
	
	object = checked_ptr_cast<Object>(node);
	object->setMaterial(raw_material.get(), "*");

	product_init();
}

void ProductBaseSettings::statr_cooking()
{
	is_cooking = true;
}

void ProductBaseSettings::stop_cooking()
{
	is_cooking = false;
}