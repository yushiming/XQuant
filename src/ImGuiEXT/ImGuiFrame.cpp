#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "ImGuiEXT/ImGuiFrame.h"
#include "Core/Application.h"
#include "Events/Event.h"


namespace XQuant {

	ImGuiFrame::ImGuiFrame(std::string name)
		: _debugName(name) {

	}

	ImGuiFrame::~ImGuiFrame() {

	}

	void ImGuiFrame::onAttach()
	{

	}

	void ImGuiFrame::onDetach()
	{

	}

	void ImGuiFrame::onEvent(Event& e)
	{
		//if (_blockEvents)
		//{
		//	ImGuiIO& io = ImGui::GetIO();
		//	e.handled |= e.isInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		//	e.handled |= e.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		//}
	}

}
