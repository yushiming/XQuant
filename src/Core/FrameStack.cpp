#include "Core/FrameStack.h"


namespace XQuant {

	FrameStack::~FrameStack() {
		for (ImGuiFrame* frame : _frames) {
			frame->onDetach();
			delete frame;
		}
	}

	void FrameStack::pushLayer(ImGuiFrame* frame) {
		_frames.emplace(_frames.begin() + _frameInsertIndex, frame);
		_frameInsertIndex++;
	}

	void FrameStack::pushOverlay(ImGuiFrame* overlay) {
		_frames.emplace_back(overlay);
	}

	void FrameStack::popLayer(ImGuiFrame* frame) {
		auto it = std::find(_frames.begin(), _frames.begin() + _frameInsertIndex, frame);
		if (it != _frames.begin() + _frameInsertIndex) {
			frame->onDetach();
			_frames.erase(it);
			_frameInsertIndex--;
		}
	}

	void FrameStack::popOverlay(ImGuiFrame* overlay) {
		auto it = std::find(_frames.begin() + _frameInsertIndex, _frames.end(), overlay);
		if (it != _frames.end()) {
			overlay->onDetach();
			_frames.erase(it);
		}
	}

}














