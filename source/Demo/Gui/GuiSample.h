#pragma once
#include <UnigineNode.h>
#include <UnigineGui.h>
#include <UnigineWidgets.h>
#include <UnigineMaterial.h>
#include <UnigineComponentSystem.h>

namespace Unigine
{
	namespace Plugins
	{
		class Varjo;
	}
}

class GuiSample : public Unigine::ComponentBase
{
public:
	COMPONENT(GuiSample, Unigine::ComponentBase);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);
	COMPONENT_SHUTDOWN(shutdown);

	// property
	PROP_NAME("gui_sample");
	PROP_AUTOSAVE(0);

protected:
	void init();
	void update();
	void shutdown();

private:
	void prepare_environment(bool enabled);
	
	void toggle_clicked();
	void button_clicked();
	void quit_clicked();
	void window_changed();
	
	void toggle_mr_clicked();
	void toggle_chromakey_clicked();
	void toggle_motion_prediction_clicked();
	void toggle_depth_test_clicked();
	void toggle_alpha_blend_clicked();
	void toggle_alpha_invert_clicked();
	void toggle_depth_test_range_clicked();
	void toggle_blend_masking_debug_clicked();

	void slider_hue_changed();
	void slider_hue_tolerance_changed();
	void slider_saturation_tolerance_changed();
	void slider_value_tolerance_changed();
	void slider_near_range_changed();
	void slider_far_range_changed();

	void select_blend_masking_mode_changed();

	void add_slider(Unigine::WidgetLabelPtr &label, Unigine::WidgetSliderPtr &slider, int x, int y, Unigine::CallbackBase *callback, Unigine::String name, int value);
	void add_button(Unigine::WidgetButtonPtr &button, int x, int y, Unigine::CallbackBase *callback, Unigine::String name, float scale);
	void add_combobox(Unigine::WidgetLabelPtr &label, Unigine::WidgetComboBoxPtr &combobox, int x, int y, Unigine::CallbackBase *callback, Unigine::String name, float scale);

	Unigine::Plugins::Varjo *app;

	Unigine::GuiPtr				gui;
	Unigine::WidgetSpritePtr	background;
	Unigine::WidgetLabelPtr		mr_not_avaliable;
	Unigine::WidgetComboBoxPtr	select_blend_masking_mode;
	Unigine::WidgetButtonPtr	quit;
	Unigine::WidgetButtonPtr	toggle;
	Unigine::WidgetButtonPtr	toggle_motion_prediction;
	Unigine::WidgetButtonPtr	toggle_mr;
	Unigine::WidgetButtonPtr	toggle_blend_masking_debug;
	Unigine::WidgetButtonPtr	toggle_chromakey;
	Unigine::WidgetButtonPtr	toggle_depth_test;
	Unigine::WidgetButtonPtr	toggle_depth_test_range;
	Unigine::WidgetButtonPtr	toggle_alpha_blend;
	Unigine::WidgetButtonPtr	toggle_alpha_invert;
	Unigine::WidgetSliderPtr	slider_hue;
	Unigine::WidgetSliderPtr	slider_hue_tolerance;
	Unigine::WidgetSliderPtr	slider_saturation_tolerance;
	Unigine::WidgetSliderPtr	slider_value_tolerance;
	Unigine::WidgetSliderPtr	slider_near_range;
	Unigine::WidgetSliderPtr	slider_far_range;
	Unigine::WidgetLabelPtr		label_blend_masking_mode;
	Unigine::WidgetLabelPtr		label_hue;
	Unigine::WidgetLabelPtr		label_hue_tolerance;
	Unigine::WidgetLabelPtr		label_saturation_tolerance;
	Unigine::WidgetLabelPtr		label_value_tolerance;
	Unigine::WidgetLabelPtr		label_near_range;
	Unigine::WidgetLabelPtr		label_far_range;
	Unigine::WidgetWindowPtr	window;
	Unigine::WidgetButtonPtr	button;

	Unigine::MaterialPtr grid_for_world_creation;
	Unigine::MaterialPtr grid_for_world_creation_mr;
};