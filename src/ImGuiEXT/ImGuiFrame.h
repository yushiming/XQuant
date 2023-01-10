//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  ImGuiFrame.h
//
//   Description:  ImGui��װ����,
//
//       Version:  1.0
//       Created:  2023��1��3�� 19ʱ32��45��
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================

#pragma once

#include <imgui.h>
#include <string>

#include "Core/TimeStep.h"
#include "Events/Event.h"

namespace XQuant {

	class ImGuiFrame
	{
	public:
		ImGuiFrame(std::string name);
		virtual ~ImGuiFrame();

		virtual void onAttach();
		virtual void onDetach();
		virtual void onUpdate(TimeStep ts) { }
		virtual void onImGuiRender() { }
		virtual void onEvent(Event& e);

		void blockEvents(bool block) { _blockEvents = block; }

		const std::string& getName()const { return _name; }

	protected:
		std::string _name;
	private:
		bool _blockEvents = true;
		float _time = 0.0f;
	};

}
