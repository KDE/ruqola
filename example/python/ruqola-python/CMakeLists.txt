# SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set(RUQOLA_PYTHON_VERSION "0.0.1")
cmake_minimum_required(VERSION 3.5)
project(ruqola-python VERSION ${RUQOLA_PYTHON_VERSION})

set(PYTHON_MIN_VERSION "3.5.0")
set(QT_MIN_VERSION "5.15")
set(KF_MIN_VERSION "5.73.0")


find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${ECM_MODULE_PATH})

find_package(PythonInterp ${PYTHON_MIN_VERSION} REQUIRED)
find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Core Gui Svg Widgets)
find_package(KF5 ${KF_MIN_VERSION} REQUIRED COMPONENTS Config DocTools)

include(FeatureSummary)
include(KDEInstallDirs)
include(ECMInstallIcons)
include(KDECMakeSettings)
include(pyslide2uigenerator)

include(FindPythonInterp)

include(KDEPython)

set(ruqola_python_SRCS
	lineedit.py  
	mainwindow.py  
)
#set(ui_files ui/accountwidget.ui)
#set(ruqola_python_generated_SRCS)
#PYSIDE_WRAP_UI(ruqola_python_generated_SRCS ${ui_files})

KDE_INSTALL_PYTHON_FILES(${ruqola_python_SRCS} ruqola.py ${ruqola_python_generated_SRCS})
KDE_ADD_PYTHON_EXECUTABLE(ruqola.py ruqola-python)
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
