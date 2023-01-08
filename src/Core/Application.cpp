#include <GLFW/glfw3.h>

#include "Core/Application.h"
#include "Core/Log.h"
#include "Core/Input.h"
#include "Core/Assert.h"

namespace XQuant {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		XQ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		_window = Window::create(WindowProps(name));
		_window->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));

		_imGuiLayer = new ImGuiFrame("test");
		pushOverlay(_imGuiLayer);
	}

	Application::~Application()
	{

	}

	void Application::pushLayer(Layer* layer)
	{
		_layerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* layer)
	{
		_layerStack.pushOverlay(layer);
		layer->onAttach();
	}

	void Application::close()
	{
		_running = false;
	}

	void Application::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(XQ_BIND_EVENT_FN(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(XQ_BIND_EVENT_FN(Application::onWindowResize));

		for (auto it = _layerStack.rbegin(); it != _layerStack.rend(); ++it)
		{
			if (e.handled)
				break;
			(*it)->onEvent(e);
		}
	}

	void Application::run()
	{
		while (_running)
		{
			float time = (float)glfwGetTime();
			TimeStep timestep = time - _lastFrameTime;
			_lastFrameTime = time;

			if (!_minimized)
			{
				{
					for (Layer* layer : _layerStack)
						layer->onUpdate(timestep);
				}

				_imGuiLayer->begin();
				{
					for (Layer* layer : _layerStack)
						layer->onImGuiRender();
				}
				_imGuiLayer->end();
			}

			_window->onUpdate();
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		_running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		if (e.getWidth() == 0 || e.getHeight() == 0)
		{
			_minimized = true;
			return false;
		}

		_minimized = false;
		return false;
	}

	XQuant::Application* XQuant::CreateApplication()
	{
		auto app = new Application();
		app->pushLayer(new ImGuiFrame("test"));  // TODO 创建多个窗体实例 并加入

		return app;
	}

}
