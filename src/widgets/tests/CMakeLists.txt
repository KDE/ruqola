set(testshowimage_gui_SRCS testshowimage_gui.cpp)
add_executable(testshowimage_gui ${testshowimage_gui_SRCS})
target_link_libraries(testshowimage_gui libruqolawidgets KF5::I18n)

######
set(testshowvideo_gui_SRCS testshowvideo_gui.cpp)
add_executable(testshowvideo_gui ${testshowvideo_gui_SRCS})
target_link_libraries(testshowvideo_gui libruqolawidgets KF5::I18n)

####
set(testaudio_gui_SRCS testaudio_gui.cpp)
add_executable(testaudio_gui ${testaudio_gui_SRCS})
target_link_libraries(testaudio_gui libruqolawidgets KF5::I18n)

####
set(testnotification_gui_SRCS testnotification_gui.cpp notificationwidget.cpp)
add_executable(testnotification_gui ${testnotification_gui_SRCS})
target_link_libraries(testnotification_gui libruqolawidgets KF5::I18n)

####
set(testcreatevideomessage_gui_SRCS testcreatevideomessage_gui.cpp)
add_executable(testcreatevideomessage_gui ${testcreatevideomessage_gui_SRCS})
target_link_libraries(testcreatevideomessage_gui libruqolawidgets KF5::I18n)
