#include <GLFW/glfw3.h>

#include "Core/Application.h"
#include "Core/Log.h"
#include "Core/Input.h"
#include "Core/Assert.h"

#include "Frames/FTest.h"
#include "Frames/FMainMenuBar.h"
#include "Frames/FMainToolBar.h"
#include "Frames/FMainStatusBar.h"

#include "Frames/FAccountLogin.h"
#include "Frames/FSecurityList.h"
#include "Frames/FTrendChart.h"
#include "Frames/FBacktestRun.h"
#include "Frames/FStrategyBlueprintRun.h"
#include "Frames/FBacktestRecord.h"
#include "Frames/FPositionDetails.h"
#include "Frames/FLogOutput.h"
#include "Frames/FStrategyBlueprintEdit.h"
#include "Frames/FTradeRecord.h"

namespace XQuant {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name) {
		XQ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
	}

	Application::~Application() {

	}

	void Application::onInit() {
		// ��������
		_window = Window::create(WindowProps());
		_window->onInit();
		_window->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));

		// ����imgui��
		_imGuiLayer = new ImGuiLayer();
		_imGuiLayer->onInit();

		// TODO �����������ʵ��(imgui frame) ������_frameStack
		auto ftest = new FTest("FTest");
		pushLayer(ftest);

		auto fmainmenubar = new FMainMenuBar(u8"�˵���");
		pushLayer(fmainmenubar);

		auto fmaintoolbar = new FMainToolBar(u8"������");
		pushLayer(fmaintoolbar);

		auto fmainstatusbar = new FMainStatusBar(u8"״̬��");
		pushLayer(fmainstatusbar);

		// 
		auto fsecuritylist = new FSecurityList(u8"Ʒ���б�");
		pushLayer(fsecuritylist);
		// 
		auto ftrendchart = new FSecurityList(u8"����ͼ");
		pushLayer(ftrendchart);
		// 
		auto fpositiondetails = new FPositionDetails(u8"�ֲ���ϸ");
		pushLayer(fpositiondetails);
		// ��־���
		auto flogoutput = new FLogOutput(u8"��־");
		pushLayer(flogoutput);
		// ������ͼ����
		auto fstrategyblueprintrun = new FStrategyBlueprintRun(u8"������ͼ����");
		pushLayer(fstrategyblueprintrun);
		// ���׼�¼ 
		auto ftraderecord = new FTradeRecord(u8"���׼�¼");
		pushLayer(ftraderecord);		
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
