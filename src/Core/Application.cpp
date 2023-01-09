#include <GLFW/glfw3.h>

#include "Core/Application.h"
#include "Core/Log.h"
#include "Core/Input.h"
#include "Core/Assert.h"

#include "Frames/FTest.h"

namespace XQuant {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name) {
		XQ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
	}

	Application::~Application() {

	}

	void Application::onInit() {
		// 创建窗口
		_window = Window::create(WindowProps());
		_window->onInit();
		_window->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));

		// 创建imgui层
		_imGuiLayer = new ImGuiLayer();
		_imGuiLayer->onInit();

		// TODO 创建多个窗体实例(imgui frame) 并加入_frameStack
		auto ftest = new FTest("FTest");
		pushLayer(ftest);


	}

	void Application::pushLayer(ImGuiFrame* frame) {
		_frameStack.pushLayer(frame);
		frame->onAttach();
	}

	void Application::pushOverlay(ImGuiFrame* frame) {
		_frameStack.pushOverlay(frame);
		frame->onAttach();
	}

	void Application::close() {
		_running = false;
	}

	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(XQ_BIND_EVENT_FN(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(XQ_BIND_EVENT_FN(Application::onWindowResize));

		_imGuiLayer->onEvent(e);

		for (auto it = _frameStack.rbegin(); it != _frameStack.rend(); ++it) {
			if (e.handled)
				break;
			(*it)->onEvent(e);
		}
	}

	void Application::run() {

		while (_running) {
			float time = (float)glfwGetTime();
			TimeStep timestep = time - _lastFrameTime;
			_lastFrameTime = time;

			if (!_minimized)
			{
				{
					for (ImGuiFrame* frame : _frameStack)
						frame->onUpdate(timestep);
				}

				_imGuiLayer->begin();
				{
					for (ImGuiFrame* frame : _frameStack)
						frame->onImGuiRender();
				}
				_imGuiLayer->end();
			}

			_window->onUpdate();
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& e) {
		_running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e) {
		if (e.getWidth() == 0 || e.getHeight() == 0) {
			_minimized = true;
			return false;
		}

		_minimized = false;
		return false;
	}

	XQuant::Application* XQuant::CreateApplication() {
		auto app = new Application();
		app->onInit();
		return app;
	}

}
