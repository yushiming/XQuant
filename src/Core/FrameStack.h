//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FrameStack.h
//
//   Description:  窗口管理栈
//
//       Version:  1.0
//       Created:  2023年1月2日 17时15分23秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================


#pragma once
#include <vector>

#include "Core/Base.h"
#include "ImGuiEXT/ImGuiFrame.h"


namespace XQuant {

	class FrameStack
	{
	public:
		FrameStack() = default;
		~FrameStack();

		void pushLayer(ImGuiFrame* frame);
		void pushOverlay(ImGuiFrame* overlay);
		void popLayer(ImGuiFrame* frame);
		void popOverlay(ImGuiFrame* overlay);

		std::vector<ImGuiFrame*>::iterator begin() { return _frames.begin(); }
		std::vector<ImGuiFrame*>::iterator end() { return _frames.end(); }
		std::vector<ImGuiFrame*>::reverse_iterator rbegin() { return _frames.rbegin(); }
		std::vector<ImGuiFrame*>::reverse_iterator rend() { return _frames.rend(); }

		std::vector<ImGuiFrame*>::const_iterator begin() const { return _frames.begin(); }
		std::vector<ImGuiFrame*>::const_iterator end()	const { return _frames.end(); }
		std::vector<ImGuiFrame*>::const_reverse_iterator rbegin() const { return _frames.rbegin(); }
		std::vector<ImGuiFrame*>::const_reverse_iterator rend() const { return _frames.rend(); }

	private:
		std::vector<ImGuiFrame*> _frames;
		unsigned int _frameInsertIndex = 0;
	};


}












