# SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
# SPDX-License-Identifier: BSD-3-Clause

kcoreaddons_add_plugin(ruqola_ollamatextplugin INSTALL_NAMESPACE ruqolaplugins/textplugins)

target_sources(ruqola_ollamatextplugin PRIVATE
    ollamatextinterface.cpp
    ollamatextplugin.cpp
    ollamatextinterface.h
    ollamatextplugin.h
)
target_link_libraries(ruqola_ollamatextplugin
    libruqolawidgets
)
if (BUILD_TESTING)
	#add_subdirectory(autotests)
endif()

