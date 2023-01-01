# SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_subdirectory(password)

if (PLUGINS_AUTHENTICATION_BASED_ON_O2)
    add_subdirectory(google)
endif()


#Experimental
add_subdirectory(twitter)
add_subdirectory(facebook)
add_subdirectory(github)
add_subdirectory(gitlab)
