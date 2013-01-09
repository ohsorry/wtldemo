project(DialogColor)
set(src
	CCtlColor.h
	aboutdlg.h
	maindlg.h
	required.h
	resource.h
	stdafx.cpp
	stdafx.h
	test_ctlcolor.h
	test_ctlcolor.rc
	test_ctlcolor.cpp
	)
set(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:WINDOWS")
set(CMAKE_CXX_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
include_directories(${CMAKE_SOURCE_DIR}/wtl80/include)
add_executable(DialogColor ${src})