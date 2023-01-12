#include <iostream>
#include <fstream>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include "Data/ProjectData.h"
#include "Utils/FileHelper.h"
#include "Utils/RapidJsonHelper.h"
#include "Core/Log.h"

namespace XQuant {
	ProjectData* ProjectData::_instance = nullptr;
	std::once_flag ProjectData::_flag;

	const std::string ProjectData::ConfigJsonFileName = "config.json";

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
		_appPath = FileHelper::getAppPath();
		_configPath = _appPath + "\\config";
		_resourcesPath = _appPath + "\\resources";
		_tempPath = _appPath + "\\temp";

		_platformConfigPath.emplace(EPlatform::eFutures, _configPath + "\\Futures");
		_platformConfigPath.emplace(EPlatform::eStocks, _configPath + "\\Stocks");
		_platformConfigPath.emplace(EPlatform::eForex, _configPath + "\\Forex");
		_platformConfigPath.emplace(EPlatform::eDigitalCash, _configPath + "\\DigitalCash");

		// ∂¡»°’À∫≈≈‰÷√
		readAccountConfig();

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

	bool ProjectData::readAccountConfig() {
		readFuturesAccountJsonFile();
		readStocksAccountJsonFile();
		readForexAccountJsonFile();
		readDigitalCashAccountJsonFile();
		return true;
	}

	void ProjectData::writerAccountConfig() {

	}

	bool ProjectData::readFuturesAccountJsonFile() {

		std::string filename = getConfigFullPathFile(EPlatform::eFutures);

		std::ifstream in(filename);
		if (!in.is_open()) {
			XQ_CORE_TRACE("fail to read json file: {0}", filename.c_str());
			return false;
		}

		std::string json_content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		in.close();

		rapidjson::Document dom;
		if (dom.Parse(json_content.c_str()).HasParseError()) {
			XQ_CORE_TRACE("fail to parse json file: {0}", filename.c_str());
			return false;
		}

		if (RapidJsonHelper::jsonHasMember(&dom, "appID")) {
			strcpy_s(futuresAccountInfo->appID, dom["appID"].GetString());
			XQ_CORE_INFO("appID: {0}", futuresAccountInfo->appID);
		}

		if (RapidJsonHelper::jsonHasMember(&dom, "authcode")) {
			strcpy_s(futuresAccountInfo->authcode, dom["authcode"].GetString());
			XQ_CORE_INFO("authcode: {0}", futuresAccountInfo->authcode);
		}

		if (RapidJsonHelper::jsonHasMember(&dom, "product")) {
			strcpy_s(futuresAccountInfo->product, dom["product"].GetString());
			XQ_CORE_INFO("product: {0}", futuresAccountInfo->product);
		}

		if (RapidJsonHelper::jsonHasMember(&dom, "brokerID")) {
			strcpy_s(futuresAccountInfo->brokerID, dom["brokerID"].GetString());
			XQ_CORE_INFO("brokerID: {0}", futuresAccountInfo->brokerID);
		}

		if (RapidJsonHelper::jsonHasMember(&dom, "userID")) {
			strcpy_s(futuresAccountInfo->userID, dom["userID"].GetString());
			XQ_CORE_INFO("userID: {0}", futuresAccountInfo->userID);
		}

		if (RapidJsonHelper::jsonHasMember(&dom, "password")) {
			strcpy_s(futuresAccountInfo->password, dom["password"].GetString());
			XQ_CORE_INFO("password: {0}", futuresAccountInfo->password);
		}

		if (RapidJsonHelper::jsonHasMember(&dom, "marketFront")) {
			strcpy_s(futuresAccountInfo->marketFront, dom["marketFront"].GetString());
			XQ_CORE_INFO("marketFront: {0}", futuresAccountInfo->marketFront);
		}

		if (RapidJsonHelper::jsonHasMember(&dom, "tradeFront")) {
			strcpy_s(futuresAccountInfo->tradeFront, dom["tradeFront"].GetString());
			XQ_CORE_INFO("tradeFront: {0}", futuresAccountInfo->tradeFront);
		}

		return true;
	}

	void ProjectData::writerFuturesAccountJsonFile() {

		rapidjson::StringBuffer buf;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buf); // it can word wrap

		writer.StartObject();

		writer.Key("appID"); writer.String(futuresAccountInfo->appID);
		writer.Key("authcode"); writer.String(futuresAccountInfo->authcode);
		writer.Key("product"); writer.String(futuresAccountInfo->product);
		writer.Key("brokerID"); writer.String(futuresAccountInfo->brokerID);
		writer.Key("userID"); writer.String(futuresAccountInfo->userID);
		writer.Key("password"); writer.String(futuresAccountInfo->password);
		writer.Key("marketFront"); writer.String(futuresAccountInfo->marketFront);
		writer.Key("tradeFront"); writer.String(futuresAccountInfo->tradeFront);
		writer.EndObject();

		const char* json_content = buf.GetString();
		std::string filename = getConfigFullPathFile(EPlatform::eFutures);
		std::ofstream outfile;
		outfile.open(filename);
		if (!outfile.is_open()) {
			XQ_CORE_ERROR("fail to open file to write: {0}", filename);
			return;
		}
		outfile << json_content << std::endl;
		outfile.close();
	}

	bool ProjectData::readStocksAccountJsonFile() {
		return true;
	}

	void ProjectData::writerStocksAccountJsonFile() {

	}

	bool ProjectData::readForexAccountJsonFile() {
		return true;
	}

	void ProjectData::writerForexAccountJsonFile() {

	}

	bool ProjectData::readDigitalCashAccountJsonFile() {
		return true;
	}

	void ProjectData::writerDigitalCashAccountJsonFile() {

	}

	std::string ProjectData::getConfigFullPathFile(EPlatform platform) {
		return _platformConfigPath[EPlatform::eFutures] + "\\" + ConfigJsonFileName;
	}

}