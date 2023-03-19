#pragma once
#include <UnigineComponentSystem.h>

class ProductBaseSettings : public Unigine::ComponentBase
{
public:
	enum ProductState
	{
		RAW = 0,
		PREPARED,
		BURNT
	};

	COMPONENT_DEFINE(ProductBaseSettings, Unigine::ComponentBase);
	COMPONENT_INIT(init);

	PROP_PARAM(Material, raw_material);
	PROP_PARAM(Material, prepared_material);
	PROP_PARAM(Material, burnt_material);

	void init();

	virtual void product_init() { };

	void statr_cooking();
	void stop_cooking();

protected:
	ProductState current_state;
	Unigine::ObjectPtr object;

	float time_to_cook_left;

	bool is_cooking;
};

