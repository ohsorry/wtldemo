project(RibbonDailog)
set(src
	AboutDlg.h
	MainDlg.h
	RibbonDialog.cpp
	RibbonDialog.rc
	RibbonTabDialog.h
	resource.h
	stdafx.cpp
	stdafx.h
	)
set(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:WINDOWS")
set(CMAKE_CXX_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
include_directories(${CMAKE_SOURCE_DIR}/wtl80/include)
add_executable(RibbonDailog ${src})