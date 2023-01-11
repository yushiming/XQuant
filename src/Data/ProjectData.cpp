
#include "Data/ProjectData.h"
#include "Utils/FileHelper.h"


namespace XQuant {
	ProjectData* ProjectData::_instance = nullptr;
	std::once_flag ProjectData::_flag;

	const std::string ProjectData::ConfigJsonFileName = "World.dpsProj";

	FuturesAccountInfo* ProjectData::futuresAccountInfo = new FuturesAccountInfo();
	StocksAccountInfo* ProjectData::stocksAccountInfo = new StocksAccountInfo();
	ForexAccountInfo* ProjectData::forexAccountInfo = new ForexAccountInfo();
	DigitalCashAccountInfo* ProjectData::digitalcashAccountInfo = new DigitalCashAccountInfo();

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

	void ProjectData::onInit() {
		std::string path = FileHelper::getFullPath("Futures\\");
		_configPath.emplace(EPlatform::eFutures, path);

		path = FileHelper::getFullPath("Stocks\\");
		_configPath.emplace(EPlatform::eStocks, path);

		path = FileHelper::getFullPath("Forex\\");
		_configPath.emplace(EPlatform::eForex, path);

		path = FileHelper::getFullPath("DigitalCash\\");
		_configPath.emplace(EPlatform::eDigitalCash, path);

	}

	void ProjectData::onEvent(Event& e) {
		//if (_blockEvents)
		//{
		//	ImGuiIO& io = ImGui::GetIO();
		//	e.handled |= e.isInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		//	e.handled |= e.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		//}
	}

	void ProjectData::login(EPlatform platform) {
		// auto projectData = ProjectData::instance();
	}

	void ProjectData::logout(EPlatform platform) {

	}

	std::string ProjectData::getConfigPath(EPlatform platform) {
		return _configPath[EPlatform::eFutures];
	}

	std::string ProjectData::getConfigFullPathFile(EPlatform platform) {
		return _configPath[EPlatform::eFutures] + ConfigJsonFileName;
	}

	bool ProjectData::readAccountConfig() {
		return true;
	}

	void ProjectData::writerAccountConfig() {

	}

	bool ProjectData::readFuturesAccountJsonFile(std::string filename) {
		return true;
	}

	void ProjectData::writerFuturesAccountJsonFile() {

	}

	bool ProjectData::readStocksAccountJsonFile(std::string filename) {
		return true;
	}

	void ProjectData::writerStocksAccountJsonFile() {

	}

	bool ProjectData::readForexAccountJsonFile(std::string filename) {
		return true;
	}

	void ProjectData::writerForexAccountJsonFile() {

	}

	bool ProjectData::readDigitalCashAccountJsonFile(std::string filename) {
		return true;
	}

	void ProjectData::writerDigitalCashAccountJsonFile() {

	}

}