#include "Frames/FLogOutput.h"

namespace XQuant {

	FLogOutput::FLogOutput(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FLogOutput::onAttach() {

	}

	void FLogOutput::onDetach() {

	}

	void FLogOutput::onEvent(Event& e) {

	}

	void FLogOutput::onImGuiRender() {
		ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoResize);
		if (ImGui::SmallButton("[Debug] FLogOutput"))
		{
			//static int counter = 0;
			//const char* categories[3] = { "info", "warn", "error" };
			//const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
			//for (int n = 0; n < 5; n++)
			//{
			//	const char* category = categories[counter % IM_ARRAYSIZE(categories)];
			//	const char* word = words[counter % IM_ARRAYSIZE(words)];
			//	log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
			//		ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
			//	counter++;
			//}
		}
		ImGui::End();
	}
}

