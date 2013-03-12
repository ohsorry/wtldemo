project(wwaher)

set(src 
	stdafx.h
	CusWin.h
	main.cpp
	GdiPlusHelper.h
	GdiPlusHelper.cpp
)

add_executable(we WIN32 ${src})