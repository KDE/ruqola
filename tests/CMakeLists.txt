# SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
add_subdirectory(expandjson)
add_subdirectory(loadroomcache)
add_subdirectory(unicodeemoticongui)
add_subdirectory(uploadfilewidgetlist)
add_subdirectory(notificationhistorygui)
add_subdirectory(loaddatabasegui)
add_subdirectory(whatsnewdialoggui)
# Selenium GUI tests
if (SeleniumWebDriverATSPI_FOUND)
    add_subdirectory(selenium-tests)
endif()
