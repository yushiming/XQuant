//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  ImGuiLayer.h
//
//   Description:  ImGui��װ��,
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

#include "Core/Layer.h"
#include "Events/Event.h"

namespace XQuant {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onEvent(Event& e) override;

		void begin();
		void end();

		void blockEvents(bool block) { _blockEvents = block; }

		void setDarkThemeColors();
	private:
		bool _blockEvents = true;
		float _time = 0.0f;
	};

}
