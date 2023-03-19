#include "GuiSample.h"
#include <UnigineObjects.h>
#include <UnigineVisualizer.h>
#include <UniginePrimitives.h>
#include <plugins/UnigineVarjo.h>
#include "../../Framework/Components/VRPlayerSpawner.h"
#include "../../Framework/Components/Players/VRPlayer.h"

REGISTER_COMPONENT(GuiSample);

using namespace Unigine;
using namespace Math;

const float depth_test_range = 3.0f;

void GuiSample::init()
{
	// attach current GUI to the VR Player (on the left controller)
	VRPlayer *vr_player = VRPlayer::get();
	if (vr_player && vr_player->isInitialized())
		gui = vr_player->getGui();
	else
		return; // hmm... it's not initialized yet. May be next frame we get lucky?

	// create background (when VR is loaded)
	EngineWindowPtr engine_window = Unigine::WindowManager::getMainWindow();
	GuiPtr engine_window_gui = nullptr;
	if (engine_window)
		engine_window_gui = engine_window->getGui();

	if (gui == nullptr)
		gui = engine_window_gui;

	if (gui != engine_window_gui)
	{
		background = WidgetSprite::create(gui, "core/textures/common/black.texture");
		background->setColor(Math::vec4(1, 1, 1, 0.25f));
		gui->addChild(background, Gui::ALIGN_BACKGROUND | Gui::ALIGN_EXPAND);
	}


	grid_for_world_creation = Materials::findMaterialByPath("core/materials/for_world_creation/grid_for_world_creation.mat");
	if (grid_for_world_creation)
	{
		grid_for_world_creation_mr = grid_for_world_creation->inherit();
		grid_for_world_creation_mr->setTransparent(Material::TRANSPARENT_BLEND);
		grid_for_world_creation_mr->setBlendSrcFunc(RenderState::BLEND_NONE);
		grid_for_world_creation_mr->setBlendDestFunc(RenderState::BLEND_NONE);
		grid_for_world_creation_mr->setParameterFloat("transparent", 0.0f);
	}

	int x = 16;

	// toggle button
	add_button(toggle, x, 16, MakeCallback(this, &GuiSample::toggle_clicked), "Press me!", 2.0f);


	add_button(quit, x, 64, MakeCallback(this, &GuiSample::quit_clicked), "Quit", 2.0f);

	// Mixed Reality Settings
	app = Unigine::Plugins::Varjo::get();

	if (app && (Engine::get()->findPlugin("Varjo") != -1))
	{
		add_button(toggle_motion_prediction, x, 112, MakeCallback(this, &GuiSample::toggle_motion_prediction_clicked), "Toggle Motion Prediction", 2.0f);

		if (app->isMixedRealityAvailable())
		{
			app->setMarkerTrackingEnabled(true);

			app->setCameraExposureTimeAuto();
			app->setCameraWhiteBalanceAuto();
			app->setCameraISOAuto();
			app->setStreamEnvironmentCubemapMode(app->ENVIRONMENT_MODE_PRESET);

			add_button(toggle_mr, x, 160, MakeCallback(this, &GuiSample::toggle_mr_clicked), "Toggle Mixed Reality", 2.0f);
			add_combobox(label_blend_masking_mode, select_blend_masking_mode, x, 224, MakeCallback(this, &GuiSample::select_blend_masking_mode_changed), "Blend Masking Mode", 1.0f);
			select_blend_masking_mode->addItem("Disabled");
			select_blend_masking_mode->addItem("Restrict Video to Mask");
			select_blend_masking_mode->addItem("Restrict VR to Mask");
			select_blend_masking_mode->addItem("Restrict VR to Chromakey Reduced by Mask");
			add_button(toggle_blend_masking_debug, x, 248, MakeCallback(this, &GuiSample::toggle_blend_masking_debug_clicked), "Toggle Blend Masking Debug", 1.0f);

			{
				ObjectMeshDynamicPtr mask_mesh = Unigine::Primitives::createBox(vec3(1.0f, 1.0f, 1.0f));
				mask_mesh->setWorldPosition(Math::Vec3(0.0f, 3.0f, 0.0f));

				MaterialPtr mat = Materials::findManualMaterial("Unigine::Varjo::varjo_mesh_mask");
				if (mat)
				{
					mat = mat->inherit();
					mat->setParameterFloat4("mask_color", vec4(0.7f, 0.9f, 0.0f, 1.0f));
					mask_mesh->setMaterial(mat, 0);
				}
			}

			int x_offset = 384;
			int y = 16;
			{
				add_button(toggle_chromakey, x, 288, MakeCallback(this, &GuiSample::toggle_chromakey_clicked), "Toggle Chromakey", 2.0f);

				{
					// Enable HSV chromakey and set default values
					app->setChromaKeyConfigType(0, app->CHROMAKEY_TYPE_HSV);
					app->setChromaKeyConfigTargetColor(0, vec3(0.48f, 0.3333f, 0.2862f));
					app->setChromaKeyConfigFalloff(0, vec3(0.03f, 0.03f, 0.03f));
					app->setDepthTestRange(dvec2(0.1f, 0.5f));

					int hue = 48;
					int hue_tolerance = 5;
					int saturation_tolerance = 10;
					int value_tolerance = 70;
					
					add_slider(label_hue, slider_hue, x + x_offset, y + 32, MakeCallback(this, &GuiSample::slider_hue_changed), "Hue", hue);
					add_slider(label_hue_tolerance, slider_hue_tolerance, x + x_offset, y + 96, MakeCallback(this, &GuiSample::slider_hue_tolerance_changed), "Hue tolerance", hue_tolerance);
					add_slider(label_saturation_tolerance, slider_saturation_tolerance, x + x_offset, y + 160, MakeCallback(this, &GuiSample::slider_saturation_tolerance_changed), "Saturation tolerance", saturation_tolerance);
					add_slider(label_value_tolerance, slider_value_tolerance, x + x_offset, y + 224, MakeCallback(this, &GuiSample::slider_value_tolerance_changed), "Value tolerance", value_tolerance);
				}

				add_button(toggle_depth_test, x, 352, MakeCallback(this, &GuiSample::toggle_depth_test_clicked), "Toggle Depth Test", 2.0f);

				{
					add_button(toggle_depth_test_range, x + x_offset, y + 256, MakeCallback(this, &GuiSample::toggle_depth_test_range_clicked), "Toggle Depth Test Range", 1.0f);

					int temp = toInt(app->getDepthTestRange().x / depth_test_range * 100.0f);
					add_slider(label_near_range, slider_near_range, x + x_offset, y + 320, MakeCallback(this, &GuiSample::slider_near_range_changed), "Near range (in meters)", temp);

					temp = toInt(app->getDepthTestRange().y / depth_test_range * 100.0f);
					add_slider(label_far_range, slider_far_range, x + x_offset, y + 384, MakeCallback(this, &GuiSample::slider_far_range_changed), "Far range (in meters)", temp);
				}

				add_button(toggle_alpha_blend, x, 416, MakeCallback(this, &GuiSample::toggle_alpha_blend_clicked), "Toggle Alpha Blend", 2.0f);
				add_button(toggle_alpha_invert, x + x_offset, 464, MakeCallback(this, &GuiSample::toggle_alpha_invert_clicked), "Toggle Alpha Invert", 1.0f);

				// Hide settings
				toggle_chromakey_clicked();
				toggle_depth_test_clicked();
				toggle_alpha_blend_clicked();
				toggle_mr_clicked();
			}
		} else
		{
			// Tell End-User that Mixed Reality is not avaliable
			mr_not_avaliable = WidgetLabel::create(gui, "Mixed Reality is unavaliable!\nThis helmet does not support mixed reality");
			mr_not_avaliable->setFontSize(32);
			mr_not_avaliable->setPosition(x, 192);
			gui->addChild(mr_not_avaliable, Gui::ALIGN_OVERLAP);
		}
	} else
	{
		// Tell End-User that Mixed Reality is not avaliable
		mr_not_avaliable = WidgetLabel::create(gui, "Mixed Reality is unavaliable!\nVarjo plugin not loaded");
		mr_not_avaliable->setFontSize(32);
		mr_not_avaliable->setPosition(x, 192);
		gui->addChild(mr_not_avaliable, Gui::ALIGN_OVERLAP);
	}

	// window
	window = WidgetWindow::create(gui, "Congratulations! You pressed the button!");
	window->setFontSize(32);
	window->addCallback(Gui::CHANGED, MakeCallback(this, &GuiSample::window_changed));

	// close button
	button = WidgetButton::create(gui, "OK");
	button->setFontSize(32);
	window->addChild(button, Gui::ALIGN_CENTER);
	button->addCallback(Gui::CLICKED, MakeCallback(this, &GuiSample::button_clicked));

	window->arrange();
	window->setSizeable(1);
}

void GuiSample::update()
{
	if (!gui)
		init();

	if (app && app->isMixedRealityAvailable())
	{
		if (!Visualizer::isEnabled())
			Visualizer::setEnabled(true);

		bool enabled = toggle_mr->isToggled();

		enabled = toggle_alpha_blend->isToggled() || toggle_depth_test->isToggled() || toggle_chromakey->isToggled();
		app->setStreamColorCorrectionEnabled(enabled && toggle_mr->isToggled());
		app->setStreamColorCorrectionWhiteBalanceEnabled(enabled && toggle_mr->isToggled());

		int num = app->getMarkerObjectNum();
		if (num > 0)
		{
			Math::vec4 colors[3] = { Math::vec4_green, Math::vec4_blue, Math::vec4_red };

			for (int i = 0; i < num; i++)
			{
				const auto &obj = app->getMarkerObject(i);

				Mat4 transform = obj.transform;
				Vec3 size = obj.size;
				size.y = (size.x + size.z) * 0.1f;

				// There are up to 3 supported marker sizes
				// - Small (up to 0.5m)
				// - Medium (up to 1m)
				// - Large (up to 3m)
				int index = Math::clamp((obj.id / 100) - 1, 0, 2);
				Scalar scale = Scalar(0.01 * (index + 1));

				Visualizer::renderBox(vec3(size), transform, colors[index], 0.0f, false);
				Visualizer::renderDirection(transform.getTranslate(), vec3(transform.getAxisX() * scale), Math::vec4_red, 0.1f, false);
				Visualizer::renderDirection(transform.getTranslate(), vec3(transform.getAxisY() * scale), Math::vec4_green, 0.1f, false);
				Visualizer::renderDirection(transform.getTranslate(), vec3(transform.getAxisZ() * scale), Math::vec4_blue, 0.1f, false);
			}
		}
	}
}

void GuiSample::shutdown()
{
	if (!gui)
		return;

	if (background)
		gui->removeChild(background);
	gui->removeChild(toggle);
	window->removeChild(button);
}

void GuiSample::prepare_environment(bool enabled)
{
	Render::getEnvironmentPreset(0)->setHazeColor(enabled ? vec4_black : vec4_white);

	// Swap ground's material
	ObjectPtr ground = checked_ptr_cast<Object>(World::getNodeByName("ground"));
	if (ground)
	{
		if (enabled)
			ground->setMaterial(grid_for_world_creation_mr, "*");
		else
			ground->setMaterial(grid_for_world_creation, "*");
	}

	RenderEnvironmentPresetPtr preset = Render::getEnvironmentPreset(0);
	preset->setSkyIntensity(enabled ? 0.0f : 1.0f);
}

void GuiSample::toggle_clicked()
{
	if (toggle->isToggled())
		gui->addChild(window, Gui::ALIGN_OVERLAP | Gui::ALIGN_CENTER);
	else
		gui->removeChild(window);
}

void GuiSample::button_clicked()
{
	toggle->setToggled(0);
}

void GuiSample::quit_clicked()
{
	exit(0);
}

void GuiSample::window_changed()
{
	// do not let the window go beyond the gui plane
	int x = Math::clamp(window->getPositionX(), 0, window->getGui()->getWidth() - window->getWidth());
	int y = Math::clamp(window->getPositionY(), 0, window->getGui()->getHeight() - window->getHeight());
	window->setPosition(x, y);
}

void GuiSample::toggle_mr_clicked()
{
	bool enabled = toggle_mr->isToggled();

	bool hide = (enabled == false);
	toggle_chromakey->setHidden(hide);
	toggle_depth_test->setHidden(hide);
	toggle_alpha_blend->setHidden(hide);
	select_blend_masking_mode->setHidden(hide);
	label_blend_masking_mode->setHidden(hide);
	toggle_blend_masking_debug->setHidden(hide);

	app->setVideo(enabled);
	app->setStreamEnvironmentCubemapEnabled(enabled);

	toggle_chromakey_clicked();
	toggle_depth_test_clicked();
	toggle_alpha_blend_clicked();
	toggle_blend_masking_debug_clicked();

	prepare_environment(toggle_alpha_blend->isToggled() && enabled);
}

void GuiSample::toggle_chromakey_clicked()
{
	bool enabled = toggle_chromakey->isToggled();
	bool hide = (enabled == false) || (toggle_mr->isToggled() == false);
	app->setChromaKey((hide == false) && enabled);

	label_hue->setHidden(hide);
	label_hue_tolerance->setHidden(hide);
	label_saturation_tolerance->setHidden(hide);
	label_value_tolerance->setHidden(hide);

	slider_hue->setHidden(hide);
	slider_hue_tolerance->setHidden(hide);
	slider_saturation_tolerance->setHidden(hide);
	slider_value_tolerance->setHidden(hide);
}

void GuiSample::toggle_motion_prediction_clicked()
{
	app->setMotionPrediction(toggle_motion_prediction->isToggled());
}

void GuiSample::toggle_blend_masking_debug_clicked()
{
	app->setBlendMaskingDebugEnabled(toggle_blend_masking_debug->isToggled());
}

void GuiSample::toggle_depth_test_clicked()
{
	app->setDepthTest(toggle_depth_test->isToggled());
	
	toggle_depth_test_range_clicked();
}

void GuiSample::toggle_alpha_blend_clicked()
{
	bool enabled = toggle_alpha_blend->isToggled();
	app->setAlphaBlend(enabled);
	prepare_environment(enabled);

	toggle_alpha_invert_clicked();
}

void GuiSample::toggle_alpha_invert_clicked()
{
	bool enabled = toggle_alpha_invert->isToggled();
	bool hide = (toggle_alpha_blend->isToggled() == false) || (toggle_mr->isToggled() == false);

	toggle_alpha_invert->setHidden(hide);
	app->setAlphaInvert((hide == false) && enabled);
}

void GuiSample::toggle_depth_test_range_clicked()
{
	bool enabled = toggle_depth_test_range->isToggled();
	app->setDepthTestRangeEnabled(enabled);

	bool hide = (toggle_depth_test->isToggled() == false) || (toggle_mr->isToggled() == false);
	toggle_depth_test_range->setHidden(hide);
	label_near_range->setHidden(hide);
	label_far_range->setHidden(hide);
	slider_near_range->setHidden(hide);
	slider_far_range->setHidden(hide);
}

void GuiSample::slider_hue_changed()
{
	vec3 hue = app->getChromaKeyConfigTargetColor(0);
	hue.x = toFloat(slider_hue->getValue()) / toFloat(slider_hue->getMaxValue());
	app->setChromaKeyConfigTargetColor(0, hue);
	app->chromaKeyConfigSubmit(0);
}

void GuiSample::slider_hue_tolerance_changed()
{
	vec3 hue_tolerance = app->getChromaKeyConfigTolerance(0);
	hue_tolerance.x = toFloat(slider_hue_tolerance->getValue()) / toFloat(slider_hue_tolerance->getMaxValue());
	app->setChromaKeyConfigTolerance(0, hue_tolerance);
	app->chromaKeyConfigSubmit(0);
}

void GuiSample::slider_saturation_tolerance_changed()
{
	vec3 hue_tolerance = app->getChromaKeyConfigTolerance(0);
	hue_tolerance.y = toFloat(slider_saturation_tolerance->getValue()) / toFloat(slider_saturation_tolerance->getMaxValue());
	app->setChromaKeyConfigTolerance(0, hue_tolerance);
	app->chromaKeyConfigSubmit(0);
}

void GuiSample::slider_value_tolerance_changed()
{
	vec3 hue_tolerance = app->getChromaKeyConfigTolerance(0);
	hue_tolerance.z = toFloat(slider_value_tolerance->getValue()) / toFloat(slider_value_tolerance->getMaxValue());
	app->setChromaKeyConfigTolerance(0, hue_tolerance);
	app->chromaKeyConfigSubmit(0);
}

void GuiSample::slider_near_range_changed()
{
	dvec2 range = app->getDepthTestRange();
	range.x = toDouble(toFloat(slider_near_range->getValue()) / toFloat(slider_near_range->getMaxValue()) * depth_test_range);
	app->setDepthTestRange(range);
}

void GuiSample::slider_far_range_changed()
{
	dvec2 range = app->getDepthTestRange();
	range.y = toDouble(toFloat(slider_far_range->getValue()) / toFloat(slider_far_range->getMaxValue()) * depth_test_range);
	app->setDepthTestRange(range);
}

void GuiSample::select_blend_masking_mode_changed()
{
	app->setBlendMaskingMode(Unigine::Plugins::Varjo::BLEND_MASKING_MODE(select_blend_masking_mode->getCurrentItem()));
}

void GuiSample::add_slider(Unigine::WidgetLabelPtr &label, Unigine::WidgetSliderPtr &slider, int x, int y, Unigine::CallbackBase *callback, Unigine::String name, int value)
{
	const int slider_width = 192;

	label = WidgetLabel::create(gui, name);
	label->setFontSize(16);
	label->setPosition(x, y - 16);
	gui->addChild(label, Gui::ALIGN_OVERLAP);

	slider = WidgetSlider::create(gui, 0, 100, value);
	slider->setPosition(x, y);
	slider->setWidth(slider_width);
	gui->addChild(slider, Gui::ALIGN_OVERLAP);
	slider->addCallback(Gui::CHANGED, callback);
}

void GuiSample::add_button(Unigine::WidgetButtonPtr &button, int x, int y, Unigine::CallbackBase *callback, Unigine::String name, float scale)
{
	button = WidgetButton::create(gui, name);
	button->setToggleable(1);
	button->setFontSize(toInt(16.0f * scale));
	button->setPosition(x, y);
	gui->addChild(button, Gui::ALIGN_OVERLAP);
	button->addCallback(Gui::CLICKED, callback);
}

void GuiSample::add_combobox(Unigine::WidgetLabelPtr &label, Unigine::WidgetComboBoxPtr &combobox, int x, int y, Unigine::CallbackBase *callback, Unigine::String name, float scale)
{
	label = WidgetLabel::create(gui, name);
	label->setFontSize(16);
	label->setPosition(x, y - 16);
	gui->addChild(label, Gui::ALIGN_OVERLAP);

	combobox = WidgetComboBox::create(gui);
	combobox->setFontSize(toInt(16.0f * scale));
	combobox->setPosition(x, y);
	gui->addChild(combobox, Gui::ALIGN_OVERLAP);
	combobox->addCallback(Gui::CLICKED, callback);
}
