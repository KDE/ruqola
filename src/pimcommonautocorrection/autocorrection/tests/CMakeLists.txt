# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: none
add_executable(autocorrectiontextedit_gui autocorrectiontextedit_gui.cpp)
target_link_libraries(autocorrectiontextedit_gui
  KF5::KIOCore
  KF5::PimCommonAutoCorrection
  KF5::I18n
)


######
add_executable(autocorrectionwidget_gui autocorrectionwidget_gui.h autocorrectionwidget_gui.cpp)
target_link_libraries(autocorrectionwidget_gui
  KF5::KIOCore
  KF5::PimCommonAutoCorrection
  KF5::I18n
)
