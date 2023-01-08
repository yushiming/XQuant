
#include "Data/ProjectData.h"

namespace XQuant {
	ProjectData* ProjectData::_instance = nullptr;
	std::once_flag ProjectData::_flag;

	const std::string ProjectData::worldXmlFileName = "World.dpsProj";


	ProjectData::ProjectData() {

	}

	ProjectData::~ProjectData() {

	}

	ProjectData* ProjectData::instance() {

		std::call_once(_flag, [&]() {
			_instance = new ProjectData();
			});

		return _instance;
	}

	void ProjectData::destroy() {
		if (_instance) {
			delete _instance;
			_instance = nullptr;
		}
	}


}