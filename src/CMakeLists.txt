
# 设置代码文件的路径
set(SRC_FILE 
	./src/main.cpp
)

set(CORE_FILE
	./src/Core/KeyCodes.h
	./src/Core/MouseCodes.h
	./src/Core/Input.h
	./src/Core/Input.cpp
	./src/Core/Base.h
	./src/Core/Assert.h
	./src/Core/TimeStep.h
	./src/Core/Application.cpp
	./src/Core/Application.h
	./src/Core/Common.cpp
	./src/Core/Common.h
	./src/Core/Log.cpp
	./src/Core/Log.h
	./src/Core/Window.h
	./src/Core/Window.cpp
	./src/Core/FrameStack.h
	./src/Core/FrameStack.cpp
)

set(DATA_FILE 
	./src/Data/ProjectData.cpp
	./src/Data/ProjectData.h
)

set(DEBUG_FILE 
#	./src/Debug/
#	./src/Debug/
)

set(EVENTS_FILE 
	./src/Events/ApplicationEvent.h
	./src/Events/Event.h
	./src/Events/KeyEvent.h
	./src/Events/MouseEvent.h
)

set(FRAMES_FILE
	./src/Frames/FMainMenuBar.h
	./src/Frames/FMainMenuBar.cpp
	./src/Frames/FMainToolBar.h
	./src/Frames/FMainToolBar.cpp
	./src/Frames/FMainStatusBar.h
	./src/Frames/FMainStatusBar.cpp

	./src/Frames/FAccountLogin.h
	./src/Frames/FAccountLogin.cpp
	./src/Frames/FSecurityList.h
	./src/Frames/FSecurityList.cpp
	./src/Frames/FTrendChart.h
	./src/Frames/FTrendChart.cpp
	./src/Frames/FBacktestRun.h
	./src/Frames/FBacktestRun.cpp
	./src/Frames/FStrategyBlueprintRun.h
	./src/Frames/FStrategyBlueprintRun.cpp	
	
	./src/Frames/FBacktestRecord.h
	./src/Frames/FBacktestRecord.cpp
	./src/Frames/FPositionDetails.h
	./src/Frames/FPositionDetails.cpp
	
	./src/Frames/FLogOutput.h
	./src/Frames/FLogOutput.cpp

	./src/Frames/FStrategyBlueprintEdit.h
	./src/Frames/FStrategyBlueprintEdit.cpp

	./src/Frames/FTradeRecord.h
	./src/Frames/FTradeRecord.cpp

	./src/Frames/FAbout.h
	./src/Frames/FAbout.cpp
	./src/Frames/FHelp.h
	./src/Frames/FHelp.cpp
	./src/Frames/FSetting.h
	./src/Frames/FSetting.cpp
	
	./src/Frames/FTest.h
	./src/Frames/FTest.cpp
		
)

set(IMGUIEXT_FILE 
	./src/ImGuiEXT/ImGuiLayer.h
	./src/ImGuiEXT/ImGuiLayer.cpp
	
	./src/ImGuiEXT/ImGuiFrame.h
	./src/ImGuiEXT/ImGuiFrame.cpp
)

set(IMGUIEXT_IMPLOT_FILE 
	./src/ImGuiEXT/Implot/implot.h
	./src/ImGuiEXT/Implot/implot.cpp
	./src/ImGuiEXT/Implot/implot_internal.h
	./src/ImGuiEXT/Implot/implot_items.cpp
	./src/ImGuiEXT/Implot/implot_demo.cpp
)

set(IO_FILE 
#	./src/IO/
#	./src/IO/
)

set(LANGUAGE_FILE 
	./src/Language/MultiLanguage.h
	./src/Language/MultiLanguage.cpp
)

set(NET_FILE 
#	./src/Net/
#	./src/Net/
)

set(PLATFORM_CTP_FILE 
	./src/Platform/Ctp/App.cpp
	./src/Platform/Ctp/MdSpi.h
	./src/Platform/Ctp/MdSpi.cpp	
	./src/Platform/Ctp/TdSpi.h
	./src/Platform/Ctp/TdSpi.cpp

	./src/Platform/Ctp/KData.h	
	./src/Platform/Ctp/KData.cpp
	./src/Platform/Ctp/mystruct.h
	./src/Platform/Ctp/mystruct.cpp
	./src/Platform/Ctp/timer.h
	./src/Platform/Ctp/Strategy.h
	./src/Platform/Ctp/Strategy.cpp
)

set(PLATFORM_XTP_FILE 
#	./src/Platform/Xtp/
#	./src/Platform/Xtp/
)

set(STRATEGY_FILE 
#	./src/Strategy/
#	./src/Strategy/
)

set(UTILS_FILE 
	./src/Utils/FileHelper.h
	./src/Utils/FileHelper.cpp
	./src/Utils/ImGuiHelper.h
	./src/Utils/ImGuiHelper.cpp
	./src/Utils/RapidJsonHelper.h
	./src/Utils/RapidJsonHelper.cpp
	./src/Utils/StringHelper.h
	./src/Utils/StringHelper.cpp
	./src/Utils/FileDialogs.h
	./src/Utils/FileDialogs.cpp		
)

# 对ImGui的源码分组到ImGui组里
source_group(src FILES ${SRC_FILE})
source_group(src/Core FILES ${CORE_FILE})
source_group(src/Data FILES ${DATA_FILE})
source_group(src/Debug FILES ${DEBUG_FILE})
source_group(src/Events FILES ${EVENTS_FILE})
source_group(src/Frames FILES ${FRAMES_FILE})
source_group(src/ImGuiEXT FILES ${IMGUIEXT_FILE})
source_group(src/ImGuiEXT/Implot FILES ${IMGUIEXT_IMPLOT_FILE})
source_group(src/IO FILES ${IO_FILE})
source_group(src/Language FILES ${LANGUAGE_FILE})
source_group(src/Net FILES ${NET_FILE})
source_group(src/Platform/Ctp FILES ${PLATFORM_CTP_FILE})
source_group(src/Platform/Xtp FILES ${PLATFORM_XTP_FILE})
source_group(src/Strategy FILES ${STRATEGY_FILE})
source_group(src/Utils FILES ${UTILS_FILE})	

