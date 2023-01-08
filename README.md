## 运行环境

1. Win10
2. Linux
3. Mac

## CMake生成工程

#### Win10

双击CreateVS2019Project-Win64.bat文件，生成VS2019工程。

#### Linux



## 目录结构	
	3rd   : 第三方库
	bin   : 生成的运行文件
	docs  : 文档 
	res   : 工程资源
	src   : 工程代码
	temp  : 临时文件存放目录


## 第三方库



* glm 数学库

  地址: https://github.com/g-truc/glm

* rapidjson: json解析库。             

  地址: https://github.com/Tencent/rapidjson

  文档: http://rapidjson.org/zh-cn/

* imgui: 界面库。

  地址: https://github.com/ocornut/imgui     

  docking分支：主要引入16个文件

  imconfig.h    

  imgui.h    

  imgui_impl_glfw.h     

  imgui_impl_opengl3.h   

  imstb_textedit.h    

  imstb_textedit.h

  imgui_impl_opengl3_loader.h     

  imgui_internal.h   

  imstb_rectpack.h   

  imgui.cpp   

  imgui_demo.cpp

  imgui_draw.cpp  

  imgui_impl_glfw.cpp   

  imgui_impl_opengl3.cpp   

  imgui_tables.cpp   

  imgui_widgets.cpp

* imgui节点: 节点编辑器库。

  地址: https://github.com/thedmd/imgui-node-editor

* ctp: 用于期货行情及交易的接口。

​          地址: http://www.sfit.com.cn/5_2_DocumentDown_2.htm



## 开发工作:	

- [x] 添加json解析库
- [x] 添加imgui库
- [x] 添加ctp库
- [ ] 读账号配置信息
- [ ] 



