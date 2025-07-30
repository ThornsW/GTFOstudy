![效果图](./GTFOstudy2.PNG)
# 如何使用

clone本程序，修改debug为release，然后生成，在\x64\Release中找到version.dll文件，放入GTFO根目录下即可


# 进度说明

·现在遗留的问题
1.鼠标在与菜单交互时无法截断输入，导致鼠标动作同时传入imgui窗口和游戏窗口

commit说明
v0.1.0 初始化版本，已完成菜单注入和菜单交互

v0.2.0 完成全套敌人数据获取相关，准备工作已经完成，但是目前没有链接渲染函数

v0.3.0 完成透视功能的显示，不过可能调用逻辑出问题了，控制台存在报错

v1.0.0 正式实现功能，修复了上述报错。算是个完整的版本。


# 特别鸣谢
djkaty的Il2CppInspector项目

Jadis0x的Il2CppInspectorPro项目

本项目学习自mankool0的GTFOHax
