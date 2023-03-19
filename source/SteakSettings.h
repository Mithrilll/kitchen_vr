#pragma once
#include "ProductBaseSettings.h"

class SteakSettings : public ProductBaseSettings
{
public:
	COMPONENT_DEFINE(SteakSettings, ProductBaseSettings);
	COMPONENT_UPDATE(update);

	PROP_PARAM(Float, cooking_time);

	void product_init() override;

	void update();
};