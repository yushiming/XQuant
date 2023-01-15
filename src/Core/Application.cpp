#include <GLFW/glfw3.h>

#include "Core/Application.h"
#include "Core/Log.h"
#include "Core/Input.h"
#include "Core/Assert.h"

#include "Frames/FTest.h"
#include "Frames/FMainMenuBar.h"
//#include "Frames/FMainToolBar.h"
//#include "Frames/FMainStatusBar.h"

#include "Frames/FAccountLogin.h"
#include "Frames/FSecurityList.h"
#include "Frames/FTrendChart.h"
#include "Frames/FStrategyBlueprintRun.h"
#include "Frames/FPositionDetails.h"
#include "Frames/FTradeLog.h"
#include "Frames/FTradeRecord.h"

#include "Frames/FStrategyClasses.h"
#include "Frames/FStrategyBlueprintEdit.h"
#include "Frames/FBacktestRecord.h"
#include "Frames/FBacktestLog.h"
#include "Frames/FStrategyProperty.h"
#include "Frames/FSecurityClasses.h"

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
		_imGuiLayer->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));

		// 初始化ProjectData
		_projectData = ProjectData::instance();
		_projectData->onInit();
		_projectData->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));

		// TODO 创建多个窗体实例(imgui frame) 并加入_frameStack
		auto ftest = new FTest("FTest");
		ftest->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		pushLayer(ftest);

		auto fmainmenubar = new FMainMenuBar(u8"菜单栏");
		fmainmenubar->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		pushLayer(fmainmenubar);

		//auto fmaintoolbar = new FMainToolBar(u8"工具栏");
		//fmaintoolbar->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		//pushLayer(fmaintoolbar);

		//auto fmainstatusbar = new FMainStatusBar(u8"状态栏");
		//fmainstatusbar->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		//pushLayer(fmainstatusbar);

		// 交易
		//auto fsecuritylist = new FSecurityList(u8"品种列表");
		//fsecuritylist->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		//pushLayer(fsecuritylist);
		//// 
		//auto ftrendchart = new FTrendChart(u8"趋势图");
		//ftrendchart->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		//pushLayer(ftrendchart);
		//// 
		//auto fpositiondetails = new FPositionDetails(u8"持仓明细");
		//fpositiondetails->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		//pushLayer(fpositiondetails);
		//// 日志输出
		//auto ftradelog = new FTradeLog(u8"交易日志");
		//ftradelog->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		//pushLayer(ftradelog);
		//// 策略蓝图运行
		//auto fstrategyblueprintrun = new FStrategyBlueprintRun(u8"策略蓝图运行");
		//fstrategyblueprintrun->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		//pushLayer(fstrategyblueprintrun);
		//// 交易记录 
		//auto ftraderecord = new FTradeRecord(u8"交易记录");
		//ftraderecord->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		//pushLayer(ftraderecord);

		// 策略
		auto fstrategyclasses = new FStrategyClasses(u8"策略类型");
		fstrategyclasses->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		pushLayer(fstrategyclasses);
		// 
		auto fstrategyblueprintedit = new FStrategyBlueprintEdit(u8"策略蓝图编辑");
		fstrategyblueprintedit->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		pushLayer(fstrategyblueprintedit);
		// 
		auto fbacktestrecord = new FBacktestRecord(u8"回测记录");
		fbacktestrecord->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		pushLayer(fbacktestrecord);
		// 回测日志
		auto fbacktestlog = new FBacktestLog(u8"回测日志");
		fbacktestlog->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		pushLayer(fbacktestlog);
		// 策略属性
		auto fstrategyproperty = new FStrategyProperty(u8"策略属性");
		fstrategyproperty->setEventCallback(XQ_BIND_EVENT_FN(Application::onEvent));
		pushLayer(fstrategyproperty);
	}

	void Application::pushLayer(ImGuiFrame* frame) {
		_frameStack.pushLayer(frame);
		frame->onAttach();
	}

	void Application::pushOverlay(ImGuiFrame* frame) {
		_frameStack.pushOverlay(frame);
		frame->onAttach();
	}

	void Application::setDeleteImGuiFrame(ImGuiFrame* frame) {
		_updateDeleteVector.push_back(frame);
	}

	void Application::setAddVectorImGuiFrame(ImGuiFrame* frame) {
		_updateAddVector.push_back(frame);
	}

	void Application::close() {
		_running = false;
	}

	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(XQ_BIND_EVENT_FN(Application::onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(XQ_BIND_EVENT_FN(Application::onWindowResize));
		if (e.handled)
			return;

		_projectData->onEvent(e);
		if (e.handled)
			return;

		_imGuiLayer->onEvent(e);
		if (e.handled)
			return;

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
			
			// add frame 
			if (!_updateAddVector.empty()) {
				for (auto& e : _updateAddVector)
					pushLayer(e);

				_updateAddVector.clear();
			}

			// delete all updates that are removed in update
			if (!_updateDeleteVector.empty()) {
				for (auto& e : _updateDeleteVector)
					_frameStack.popLayer(e);

				_updateDeleteVector.clear();
			}

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
