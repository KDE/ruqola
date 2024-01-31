######
add_executable(testimportdatawizard_gui testimportdatawizard_gui.cpp)
target_link_libraries(testimportdatawizard_gui libruqolawidgets KF6::I18n)


######
add_executable(testexportdatawizard_gui testexportdatawizard_gui.cpp)
target_link_libraries(testexportdatawizard_gui libruqolawidgets KF6::I18n)
