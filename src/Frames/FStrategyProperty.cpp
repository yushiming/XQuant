#include "Frames/FStrategyProperty.h"
#include "Utils/ImGuiHelper.h"

namespace XQuant {

	FStrategyProperty::FStrategyProperty(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FStrategyProperty::onAttach() {
		_isShow = true;
	}

	void FStrategyProperty::onDetach() {

	}

	void FStrategyProperty::onEvent(Event& e) {

	}

	void FStrategyProperty::onImGuiRender() {
		if (!_isShow) {
			// TOFIX 这地方可以改为事件
			// Application::instance().setDeleteImGuiFrame(this);
			return;
		}

		if (_initWinPos) {
			// ImGui::SetNextWindowPos({ float(Config::ScreenWidth - _winWidth) / 2, float(Config::ScreenHeigth - _winHeigth) / 2 });
			_initWinPos = false;
		}

		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_None);

		// ImGui::Separator();
		ImGui::LabelText("label", "Value");

		{
			// Using the _simplified_ one-liner Combo() api here
			// See "Combo" section for examples of how to use the more flexible BeginCombo()/EndCombo() api.
			const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIIIIII", "JJJJ", "KKKKKKK" };
			static int item_current = 0;
			ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
			ImGui::SameLine(); ImGuiHelper::helpMarker(
				"Using the simplified one-liner Combo API here.\nRefer to the \"Combo\" section below for an explanation of how to use the more flexible and general BeginCombo/EndCombo API.");
		}

		{
			// To wire InputText() with std::string or any other custom string type,
			// see the "Text Input > Resize Callback" section of this demo, and the misc/cpp/imgui_stdlib.h file.
			static char str0[128] = "Hello, world!";
			ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));
			ImGui::SameLine(); ImGuiHelper::helpMarker(
				"USER:\n"
				"Hold SHIFT or use mouse to select text.\n"
				"CTRL+Left/Right to word jump.\n"
				"CTRL+A or Double-Click to select all.\n"
				"CTRL+X,CTRL+C,CTRL+V clipboard.\n"
				"CTRL+Z,CTRL+Y undo/redo.\n"
				"ESCAPE to revert.\n\n"
				"PROGRAMMER:\n"
				"You can use the ImGuiInputTextFlags_CallbackResize facility if you need to wire InputText() "
				"to a dynamic string type. See misc/cpp/imgui_stdlib.h for an example (this is not demonstrated "
				"in imgui_demo.cpp).");

			static char str1[128] = "";
			ImGui::InputTextWithHint("input text (w/ hint)", "enter text here", str1, IM_ARRAYSIZE(str1));

			static int i0 = 123;
			ImGui::InputInt("input int", &i0);

			static float f0 = 0.001f;
			ImGui::InputFloat("input float", &f0, 0.01f, 1.0f, "%.3f");

			static double d0 = 999999.00000001;
			ImGui::InputDouble("input double", &d0, 0.01f, 1.0f, "%.8f");

			static float f1 = 1.e10f;
			ImGui::InputFloat("input scientific", &f1, 0.0f, 0.0f, "%e");
			ImGui::SameLine(); ImGuiHelper::helpMarker(
				"You can input value using the scientific notation,\n"
				"  e.g. \"1e+8\" becomes \"100000000\".");

			static float vec4a[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
			ImGui::InputFloat3("input float3", vec4a);
		}

		{
			static int i1 = 50, i2 = 42;
			ImGui::DragInt("drag int", &i1, 1);
			ImGui::SameLine(); ImGuiHelper::helpMarker(
				"Click and drag to edit value.\n"
				"Hold SHIFT/ALT for faster/slower edit.\n"
				"Double-click or CTRL+click to input value.");

			ImGui::DragInt("drag int 0..100", &i2, 1, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);

			static float f1 = 1.00f, f2 = 0.0067f;
			ImGui::DragFloat("drag float", &f1, 0.005f);
			ImGui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");
		}

		{
			static int i1 = 0;
			ImGui::SliderInt("slider int", &i1, -1, 3);
			ImGui::SameLine(); ImGuiHelper::helpMarker("CTRL+click to input value.");

			static float f1 = 0.123f, f2 = 0.0f;
			ImGui::SliderFloat("slider float", &f1, 0.0f, 1.0f, "ratio = %.3f");
			ImGui::SliderFloat("slider float (log)", &f2, -10.0f, 10.0f, "%.4f", ImGuiSliderFlags_Logarithmic);

			static float angle = 0.0f;
			ImGui::SliderAngle("slider angle", &angle);

			// Using the format string to display a name instead of an integer.
			// Here we completely omit '%d' from the format string, so it'll only display a name.
			// This technique can also be used with DragInt().
			enum Element { Element_Fire, Element_Earth, Element_Air, Element_Water, Element_COUNT };
			static int elem = Element_Fire;
			const char* elems_names[Element_COUNT] = { "Fire", "Earth", "Air", "Water" };
			const char* elem_name = (elem >= 0 && elem < Element_COUNT) ? elems_names[elem] : "Unknown";
			ImGui::SliderInt("slider enum", &elem, 0, Element_COUNT - 1, elem_name);
			ImGui::SameLine(); ImGuiHelper::helpMarker("Using the format string parameter to display a name instead of the underlying integer.");
		}

		ImGui::End();
	}
}

