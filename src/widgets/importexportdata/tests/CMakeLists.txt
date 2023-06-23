######
add_executable(testimportdatawizard_gui testimportdatawizard_gui.cpp)
target_link_libraries(testimportdatawizard_gui libruqolawidgets KF${KF_MAJOR_VERSION}::I18n)


######
add_executable(testexportdatawizard_gui testexportdatawizard_gui.cpp)
target_link_libraries(testexportdatawizard_gui libruqolawidgets KF${KF_MAJOR_VERSION}::I18n)
