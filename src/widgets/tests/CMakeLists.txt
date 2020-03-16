set(testshowimage_gui_SRCS testshowimage_gui.cpp)
add_executable(testshowimage_gui ${testshowimage_gui_SRCS})
target_link_libraries(testshowimage_gui libruqolawidgets KF5::I18n)

######
set(testshowvideo_gui_SRCS testshowvideo_gui.cpp)
add_executable(testshowvideo_gui ${testshowvideo_gui_SRCS})
target_link_libraries(testshowvideo_gui libruqolawidgets KF5::I18n)
