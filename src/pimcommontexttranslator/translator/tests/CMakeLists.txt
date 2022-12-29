# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
add_executable(translator_gui translator_gui.cpp)
target_link_libraries(translator_gui
  KF5::PimCommonTextTranslator KF5::I18n
)

######
add_executable(translatorconfigurelanguagelistwidget_gui translatorconfigurelanguagelistwidget_gui.cpp translatorconfigurelanguagelistwidget_gui.h)
target_link_libraries(translatorconfigurelanguagelistwidget_gui
  KF5::PimCommonTextTranslator KF5::I18n
)
