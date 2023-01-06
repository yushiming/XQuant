
# 设置代码文件的路径
set(SRC_FILE 
	./src/main.cpp
)

set(CORE_FILE 
	./src/Core/Application.cpp
	./src/Core/Application.h
	./src/Core/Common.cpp
	./src/Core/Common.h
	./src/Core/Layer.cpp
	./src/Core/Layer.h
	./src/Core/Log.cpp
	./src/Core/Log.h
	./src/Core/Window.cpp
	./src/Core/Window.cpp
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
)

set(FRAMES_FILE 
#	./src/Frames/
#	./src/Frames/
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
#	./src/Platform/Ctp/MdSpi.h
#	./src/Platform/Ctp/TdSpi.h
#	./src/Platform/Ctp/MdSpi.cpp
#	./src/Platform/Ctp/TdSpi.cpp
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
	./src/Utils/ImGuiHelper.h
	./src/Utils/ImGuiHelper.cpp
	./src/Utils/RapidJsonHelper.h
	./src/Utils/RapidJsonHelper.cpp
	./src/Utils/StringHelper.h
	./src/Utils/StringHelper.cpp	
)

# 对ImGui的源码分组到ImGui组里
source_group(src FILES ${SRC_FILE})
source_group(src/Core FILES ${CORE_FILE})
source_group(src/Data FILES ${DATA_FILE})
source_group(src/Debug FILES ${DEBUG_FILE})
source_group(src/Events FILES ${EVENTS_FILE})
source_group(src/Frames FILES ${FRAMES_FILE})
source_group(src/ImGuiEXT/Implot FILES ${IMGUIEXT_IMPLOT_FILE})
source_group(src/IO FILES ${IO_FILE})
source_group(src/Language FILES ${LANGUAGE_FILE})
source_group(src/Net FILES ${NET_FILE})
source_group(src/Platform/Ctp FILES ${PLATFORM_CTP_FILE})
source_group(src/Platform/Xtp FILES ${PLATFORM_XTP_FILE})
source_group(src/Strategy FILES ${STRATEGY_FILE})
source_group(src/Utils FILES ${UTILS_FILE})	

