#include <math.h>
#include "ImGuiEXT/Implot/implot.h"
#include "ImGuiEXT/Implot/implot_internal.h"
#include "Core/Application.h"

#include "Frames/FTradeRecord.h"

namespace XQuant {

	template <typename T>
	inline T RandomRange(T min, T max) {
		T scale = rand() / (T)RAND_MAX;
		return min + scale * (max - min);
	}

	FTradeRecord::FTradeRecord(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FTradeRecord::onAttach() {
		_isShow = true;
	}

	void FTradeRecord::onDetach() {

	}

	void FTradeRecord::onEvent(Event& e) {

	}

	void FTradeRecord::onImGuiRender() {
		if (!_isShow) {
			// TOFIX 这地方可以改为事件
			//Application::instance().setDeleteImGuiFrame(this);
			return;
		}

		if (_initWinPos) {
			// ImGui::SetNextWindowPos({ float(Config::ScreenWidth - _winWidth) / 2, float(Config::ScreenHeigth - _winHeigth) / 2 });
			_initWinPos = false;
		}

		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_None);

		// 曲线图
		static double t_min = 1609459200; // 01/01/2021 @ 12:00:00am (UTC)
		static double t_max = 1640995200; // 01/01/2022 @ 12:00:00am (UTC)

		static const int length = 60 * 60 * 24 * 366;

		static double xs1[366], ys1[366];
		srand(0);
		for (int i = 0; i < 366; ++i) {
			xs1[i] = t_min + i * 60 * 60 * 24;
			ys1[i] = 1.0 + i + RandomRange(-5.0, 10.0);
		}
		static bool show_lines = true;
		static bool show_fills = true;
		static float fill_ref = 0;
		static ImPlotShadedFlags flags = 0;

		if (ImPlot::BeginPlot(u8"收益率")) {
			//ImPlot::SetupAxes("Days", "Price");
			//ImPlot::SetupAxesLimits(0, 100, 0, 500);
			ImPlot::SetupAxisScale(ImAxis_X1, ImPlotScale_Time);
			ImPlot::SetupAxesLimits(t_min, t_max, 1.0, 450.0);
			if (show_fills) {
				ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
				ImPlot::PlotShaded("##Line", xs1, ys1, 366, fill_ref, flags);
				ImPlot::PopStyleVar();
			}
			if (show_lines) {
				ImPlot::PlotLine("##Line", xs1, ys1, 366);
			}
			ImPlot::EndPlot();
		}		

		// 提示
		ImGui::BulletText(u8"交易次数: "); ImGui::SameLine(); ImGui::Text(u8"20");
		ImGui::BulletText(u8"盈利次数: "); ImGui::SameLine(); ImGui::Text(u8"10");
		ImGui::BulletText(u8"止损次数: "); ImGui::SameLine(); ImGui::Text(u8"3");

		ImGui::End();
	}
}

