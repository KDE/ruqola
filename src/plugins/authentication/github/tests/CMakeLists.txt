# SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(githubtestapps)

target_sources(githubtestapps
    PRIVATE
    main.cpp
    ../githubauthenticationjob.h
    ../githubauthenticationjob.cpp
    ../githubauthenticationsettings.h
    ../githubauthenticationsettings.cpp
)

ecm_qt_declare_logging_category(githubtestapps HEADER githubauthenticationplugin_debug.h
    IDENTIFIER RUQOLA_GITHUBAUTHENTICATION_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_githubauthenticationplugin
    DESCRIPTION "ruqola (github authentication plugin)" EXPORT RUQOLA)

target_link_libraries(githubtestapps
    Qt::Gui
    Qt::Network
    Qt::NetworkAuth
)
set_target_properties(githubtestapps PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
