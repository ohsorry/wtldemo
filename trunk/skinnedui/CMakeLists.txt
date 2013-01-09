project(SkineUI)
set(src
	DibCtrls.h
	RoundColorFrame.h
	RoundDibButton.h
	RoundDibFrame.h
	ShadedLabel.h
	SkinnedUI.cpp
	SkinnedUI.h
	SkinnedUI.rc
	atldib.h
	atldibfilters.h
	maindlg.h
	resource.h
	stdafx.cpp
	stdafx.h
	)
set(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:WINDOWS")
set(CMAKE_CXX_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
include_directories(${CMAKE_SOURCE_DIR}/wtl80/include)
add_executable(SkineUI ${src})