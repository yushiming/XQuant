//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  Layer.h
//
//   Description:  层基类
//
//       Version:  1.0
//       Created:  2023年1月2日 9时12分34秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================

#pragma once

#include <string>

#include "Core/Base.h"
#include "Core/Timestep.h"
#include "Events/Event.h"


namespace XQuant {

	class Layer {

	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void onAttach() { }
		virtual void onDetach() { }
		virtual void onUpdate(TimeStep ts) { }
		virtual void onImGuiRender() { }
		virtual void onEvent(Event& event) { }

		const std::string& getName()const { return _debugName; }

	protected:
		std::string _debugName;
	};

}