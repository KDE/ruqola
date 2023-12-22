# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(gitlabtestapps)

target_sources(gitlabtestapps
    PRIVATE
    main.cpp
    ../gitlabauthenticationjob.h
    ../gitlabauthenticationjob.cpp
)

ecm_qt_declare_logging_category(gitlabtestapps HEADER gitlabauthenticationplugin_debug.h
    IDENTIFIER RUQOLA_GITLABAUTHENTICATION_PLUGIN_LOG CATEGORY_NAME org.kde.ruqola_gitlabauthenticationplugin
    DESCRIPTION "ruqola (gitlab authentication plugin)" EXPORT RUQOLA)

target_link_libraries(gitlabtestapps
    Qt::Gui
    Qt::Network
    Qt::NetworkAuth
)
set_target_properties(gitlabtestapps PROPERTIES DISABLE_PRECOMPILE_HEADERS ON)
