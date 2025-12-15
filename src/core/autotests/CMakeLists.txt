# SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>,
# SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

if(ENABLE_PCH)
    if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/empty_pch.cpp)
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/empty_pch.cpp "/*empty file*/")
    endif()
    file(
        WRITE ${CMAKE_CURRENT_BINARY_DIR}/ruqolacore_pch_tmp.h
        "#pragma once\n"
        "#include <QObject>\n"
        "#include <QTest>\n"
        "#include <QJsonDocument>\n"
        "#include <QJsonObject>\n"
        "#include <QSignalSpy>\n"
    )
    # avoid rebuilding if there was no change
    execute_process(
        COMMAND
            ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/ruqolacore_pch_tmp.h"
            "${CMAKE_CURRENT_BINARY_DIR}/ruqolacore_pch.h"
    )

    add_library(libruqolacore_test_pch STATIC ${CMAKE_CURRENT_BINARY_DIR}/empty_pch.cpp)
    target_precompile_headers(libruqolacore_test_pch PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/ruqolacore_pch.h)
    target_link_libraries(
        libruqolacore_test_pch
        libruqolacore
        ruqolacorehelper
        Qt::Test
    )
endif()

add_library(ruqolacorehelper STATIC)
target_sources(
    ruqolacorehelper
    PRIVATE
        ruqola_autotest_helper.cpp
        ruqola_autotest_helper.h
)
target_link_libraries(
    ruqolacorehelper
    Qt::Test
    Qt::Core
)

add_definitions(-DRUQOLA_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
add_definitions(-DRUQOLA_BINARY_DATA_DIR="${CMAKE_CURRENT_BINARY_DIR}/data")
macro(add_ruqola_test _source)
    set(_test ${_source})
    get_filename_component(_name ${_source} NAME_WE)
    add_executable(
        ${_name}
        ${_test}
        ${ARGN}
        ${_name}.h
    )
    add_test(NAME ${_name} COMMAND ${_name})
    ecm_mark_as_test(${_name})
    target_link_libraries(
        ${_name}
        Qt::Test
        libruqolacore
        ruqolacorehelper
    )
    if(ENABLE_PCH)
        target_precompile_headers(${_name} REUSE_FROM libruqolacore_test_pch)
    endif()
    set_target_properties(
        ${_name}
        PROPERTIES
            DISABLE_PRECOMPILE_HEADERS
                ON
    )
endmacro()

add_ruqola_test(accountschannelsmodeltest.cpp)
add_ruqola_test(rocketchatmessagetest.cpp)
add_ruqola_test(roommodeltest.cpp)
add_ruqola_test(messagesmodeltest.cpp)
add_ruqola_test(typingnotificationtest.cpp)
add_ruqola_test(utilstest.cpp)
add_ruqola_test(usertest.cpp)
add_ruqola_test(messageattachmenttest.cpp)
add_ruqola_test(rocketchataccountsettingstest.cpp)
add_ruqola_test(messagetest.cpp)
add_ruqola_test(messageurltest.cpp)
add_ruqola_test(roomtest.cpp)
add_ruqola_test(ruqolaserverconfigtest.cpp)
add_ruqola_test(statusmodeltest.cpp)
add_ruqola_test(rocketchatcachetest.cpp)
add_ruqola_test(loadrecenthistorymanagertest.cpp)
add_ruqola_test(notificationtest.cpp)
if(NOT TARGET KF6::TextEmoticonsWidgets)
    add_ruqola_test(customemojitest.cpp)
endif()
add_ruqola_test(emojimanagertest.cpp)
add_ruqola_test(otrtest.cpp)
add_ruqola_test(otrmanagertest.cpp)
add_ruqola_test(rocketchataccounttest.cpp)
add_ruqola_test(usersmodeltest.cpp)
add_ruqola_test(usersforroommodeltest.cpp)
add_ruqola_test(filetest.cpp)
add_ruqola_test(filesforroommodeltest.cpp)
add_ruqola_test(filesforroomfilterproxymodeltest.cpp)
add_ruqola_test(usersforroomfilterproxymodeltest.cpp)
add_ruqola_test(usercompletermodeltest.cpp)
add_ruqola_test(roomfilterproxymodeltest.cpp)
add_ruqola_test(roomlistheadingsproxymodeltest.cpp)
add_ruqola_test(usercompleterfilterproxymodeltest.cpp)
add_ruqola_test(inputcompletermodeltest.cpp)
add_ruqola_test(inputtextmanagertest.cpp)
add_ruqola_test(authenticationinfotest.cpp)
add_ruqola_test(commonmessagesmodeltest.cpp)
add_ruqola_test(commonmessagefilterproxymodeltest.cpp)
add_ruqola_test(accountmanagertest.cpp)
add_ruqola_test(rocketchataccountmodeltest.cpp)
add_ruqola_test(ruqolatest.cpp)
add_ruqola_test(managerdatapathstest.cpp)
add_ruqola_test(rocketchataccountfilterproxymodeltest.cpp)
add_ruqola_test(notificationoptionstest.cpp)
add_ruqola_test(reactionstest.cpp)
add_ruqola_test(reactiontest.cpp)
add_ruqola_test(receivetypingnotificationmanagertest.cpp)
add_ruqola_test(serverconfiginfotest.cpp)
add_ruqola_test(notificationpreferencemodeltest.cpp)
add_ruqola_test(notificationpreferencestest.cpp)
add_ruqola_test(roletest.cpp)
add_ruqola_test(rolestest.cpp)
add_ruqola_test(messagestarredtest.cpp)
add_ruqola_test(messagepinnedtest.cpp)
add_ruqola_test(notificationdesktopdurationpreferencemodeltest.cpp)
add_ruqola_test(notificationdesktopsoundpreferencemodeltest.cpp)
add_ruqola_test(discussionsmodeltest.cpp)
add_ruqola_test(discussionsfilterproxymodeltest.cpp)
add_ruqola_test(discussiontest.cpp)
add_ruqola_test(discussionstest.cpp)
add_ruqola_test(fileattachmentstest.cpp)
add_ruqola_test(threadmessagemodeltest.cpp)
add_ruqola_test(listmessagesmodeltest.cpp)
add_ruqola_test(listmessagesfilterproxymodeltest.cpp)
add_ruqola_test(autotranslatelanguagesmodeltest.cpp)
add_ruqola_test(autotranslatelanguagetest.cpp)
add_ruqola_test(autotranslatelanguagestest.cpp)
add_ruqola_test(messagetranslationstest.cpp)
add_ruqola_test(accountroomsettingstest.cpp)
add_ruqola_test(messagecachetest.cpp)
add_ruqola_test(commandtest.cpp)
add_ruqola_test(commandstest.cpp)
add_ruqola_test(lrucachetest.cpp)
add_ruqola_test(notifierjobtest.cpp)
add_ruqola_test(ddpauthenticationmanagertest.cpp)
add_ruqola_test(restauthenticationmanagertest.cpp)
add_ruqola_test(downloadappslanguagesparsertest.cpp)
add_ruqola_test(downloadappslanguagesinfotest.cpp)
add_ruqola_test(downloadappslanguagesmanagertest.cpp)
add_ruqola_test(ownusertest.cpp)
add_ruqola_test(servicepasswordtest.cpp)
add_ruqola_test(messageattachmentfieldtest.cpp)
add_ruqola_test(roominfotest.cpp)
add_ruqola_test(adminroomsmodeltest.cpp)
add_ruqola_test(channelcounterinfotest.cpp)
add_ruqola_test(customuserstatusestest.cpp)
add_ruqola_test(customuserstatustest.cpp)
add_ruqola_test(ownuserpreferencestest.cpp)
add_ruqola_test(permissiontest.cpp)
add_ruqola_test(permissionsmanagertest.cpp)
add_ruqola_test(adminusersallmodeltest.cpp)
add_ruqola_test(systemmessagesmodeltest.cpp)
add_ruqola_test(retentioninfotest.cpp)
add_ruqola_test(statusmodelfilterproxymodeltest.cpp)
add_ruqola_test(teaminfotest.cpp)
add_ruqola_test(teamroomtest.cpp)
add_ruqola_test(teamroomcompletertest.cpp)
add_ruqola_test(inviteinfotest.cpp)
add_ruqola_test(teamcompletertest.cpp)
add_ruqola_test(roleinfotest.cpp)
add_ruqola_test(customsoundinfotest.cpp)
add_ruqola_test(permissionstest.cpp)
add_ruqola_test(rolesmodeltest.cpp)
add_ruqola_test(otrnotificationjobtest.cpp)
add_ruqola_test(customsoundsmanagertest.cpp)
add_ruqola_test(rolesmanagertest.cpp)
add_ruqola_test(awaymanagertest.cpp)
add_ruqola_test(parserocketchaturlutilstest.cpp)
add_ruqola_test(oauthinfotest.cpp)
add_ruqola_test(notificationinfotest.cpp)
add_ruqola_test(notificationhistorymanagertest.cpp)
add_ruqola_test(bannerinfotest.cpp)
add_ruqola_test(bannerinfostest.cpp)
add_ruqola_test(deviceinfotest.cpp)
add_ruqola_test(deviceinfostest.cpp)
add_ruqola_test(licensesmanagertest.cpp)
add_ruqola_test(personalaccesstokeninfotest.cpp)
add_ruqola_test(personalaccesstokeninfostest.cpp)
add_ruqola_test(blocktest.cpp)
add_ruqola_test(videoconferencenotificationjobtest.cpp)
add_ruqola_test(videoconferencetest.cpp)
add_ruqola_test(videoconferenceinfotest.cpp)
add_ruqola_test(commandsmodeltest.cpp)
add_ruqola_test(servererrorinfotest.cpp)
add_ruqola_test(updatevideoconferencemessagejobtest.cpp)
add_ruqola_test(videoconferencemessageinfomanagertest.cpp)
add_ruqola_test(authenticationmanagerutilstest.cpp)
add_ruqola_test(managelocaldatabasetest.cpp)
add_ruqola_test(manageloadhistoryparsesyncmessagesutilstest.cpp)
add_ruqola_test(moderationreportedmessageinfotest.cpp)
add_ruqola_test(moderationreportinfotest.cpp)
add_ruqola_test(channelusercompletertest.cpp)
add_ruqola_test(ruqolaktexttohtmltest.cpp)
add_ruqola_test(previewurlcachemanagertest.cpp)
add_ruqola_test(e2ekeymanagertest.cpp)
add_ruqola_test(videoconferenceinfostest.cpp)
add_ruqola_test(blockactiontest.cpp)
add_ruqola_test(systemmessagetypeutiltest.cpp)
add_ruqola_test(messageextratest.cpp)
add_ruqola_test(moderationreporteduserinfotest.cpp)
add_ruqola_test(appscategoryinfotest.cpp)
if(HAVE_TEXT_TRANSLATOR)
    add_ruqola_test(translatetextjobtest.cpp)
endif()

add_ruqola_test(appsmarketplaceinfotest.cpp)
add_ruqola_test(textconvertertest.cpp)

if(USE_E2E_SUPPORT)
    add_ruqola_test(encryptionutilstest.cpp)
    add_ruqola_test(masterkeytest.cpp)
    add_ruqola_test(rsapairtest.cpp)
    add_ruqola_test(sessionkeytest.cpp)
    add_ruqola_test(messageencryptiondecryptiontest.cpp)
    #add_ruqola_test(uploaddownloadrsakeypairtest.cpp)
    add_ruqola_test(sessionkeydistributiontest.cpp)

    target_sources(
        sessionkeydistributiontest
        PRIVATE
            ${CMAKE_SOURCE_DIR}/tests/encryptiontestgui/loginmanager.cpp
            ${CMAKE_SOURCE_DIR}/tests/encryptiontestgui/envutils.cpp
            ${CMAKE_SOURCE_DIR}/tests/encryptiontestgui/uploaddownloadrsakeypair.cpp
    )
    target_link_libraries(sessionkeydistributiontest librocketchatrestapi-qt)

    target_include_directories(
        sessionkeydistributiontest
        PRIVATE
            ${CMAKE_SOURCE_DIR}/tests/encryptiontestgui
            ${CMAKE_SOURCE_DIR}/src/core/encryption
    )

    if(0)
        target_include_directories(
            uploaddownloadrsakeypairtest
            PRIVATE
                ${CMAKE_SOURCE_DIR}/tests/encryptiontest
                ${CMAKE_SOURCE_DIR}/src/core/encryption
                ${CMAKE_SOURCE_DIR}/src/rocketchatrestapi-qt
                ${CMAKE_SOURCE_DIR}/src/rocketchatrestapi-qt/e2e
                ${CMAKE_BINARY_DIR}/src/core
                ${CMAKE_BINARY_DIR}/src/rocketchatrestapi-qt
        )

        target_sources(
            uploaddownloadrsakeypairtest
            PRIVATE
                ${CMAKE_SOURCE_DIR}/tests/encryptiontestgui/loginmanager.cpp
                ${CMAKE_SOURCE_DIR}/tests/encryptiontestgui/uploaddownloadrsakeypair.cpp
                ${CMAKE_SOURCE_DIR}/tests/encryptiontestgui/envutils.cpp
        )
    endif()
endif()

add_ruqola_test(channelstest.cpp)
add_ruqola_test(blockstest.cpp)
add_ruqola_test(messageurlstest.cpp)
add_ruqola_test(repliestest.cpp)
add_ruqola_test(moderationreportuserinfotest.cpp)
add_ruqola_test(moderationreportuserinfostest.cpp)
add_ruqola_test(appscountinfotest.cpp)
add_ruqola_test(appsmarketplaceinstalledinfotest.cpp)
add_ruqola_test(applicationssettingssettingsinfotest.cpp)
add_ruqola_test(applicationssettingslogsinfotest.cpp)
add_ruqola_test(actionbuttontest.cpp)
add_ruqola_test(actionbuttonsmanagertest.cpp)
add_ruqola_test(previewcommandtest.cpp)
add_ruqola_test(previewcommandutilstest.cpp)
add_ruqola_test(ruqolacommandlineparsertest.cpp)
add_ruqola_test(ruqolaquicksearchmessagesettingstest.cpp)
