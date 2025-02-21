/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbuttontest.h"
#include "actionbuttons/actionbutton.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(ActionButtonTest)
ActionButtonTest::ActionButtonTest(QObject *parent)
    : QObject{parent}
{
}

void ActionButtonTest::shouldHaveDefaultValues()
{
    ActionButton b;
    QVERIFY(b.appId().isEmpty());
    QVERIFY(b.actionId().isEmpty());
    QVERIFY(b.labelI18n().isEmpty());
    QVERIFY(b.hasOneRole().isEmpty());
    QVERIFY(b.hasAllRoles().isEmpty());

    QCOMPARE(b.roomTypeFilters(), ActionButton::RoomTypeFilter::Unknown);
    QCOMPARE(b.buttonContext(), ActionButton::ButtonContext::Unknown);
}

void ActionButtonTest::shouldLoadActionButton_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<ActionButton>("actionButton");

    {
        ActionButton b;
        QTest::addRow("actionbutton-empty") << QStringLiteral("actionbutton-empty") << b;
    }

    {
        ActionButton b;
        b.setAppId("821cd5c6-1fb5-4d9e-8e88-e6176463efb6");
        b.setActionId("auto-reply-room-action-id");
        b.setLabelI18n("auto-reply-room-action-name"_L1);
        b.setButtonContext(ActionButton::ButtonContext::RoomAction);
        b.setRoomTypeFilters(ActionButton::RoomTypeFilter::Direct);
        QTest::addRow("actionbutton-test1") << QStringLiteral("actionbutton-test1") << b;
    }

    {
        ActionButton b;
        b.setAppId("7a8cd36b-5f7b-4177-bd7f-bfc9be908bf8");
        b.setActionId("manage_all_reminders_room_action");
        b.setLabelI18n("manage_all_reminders_room_action"_L1);
        b.setButtonContext(ActionButton::ButtonContext::RoomAction);
        b.setHasOneRole({"admin"_L1, "owner"_L1});
        ActionButton::RoomTypeFilters f;
        f |= ActionButton::RoomTypeFilter::PublicChannel;
        f |= ActionButton::RoomTypeFilter::PublicTeam;
        f |= ActionButton::RoomTypeFilter::PublicDiscussion;
        f |= ActionButton::RoomTypeFilter::PrivateChannel;
        f |= ActionButton::RoomTypeFilter::PrivateTeam;
        f |= ActionButton::RoomTypeFilter::PrivateDiscussion;

        b.setRoomTypeFilters(f);
        QTest::addRow("actionbutton-test2") << QStringLiteral("actionbutton-test2") << b;
    }
}

void ActionButtonTest::shouldLoadActionButton()
{
    QFETCH(QString, name);
    QFETCH(ActionButton, actionButton);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/actionbutton/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    ActionButton m;
    m.parseActionButton(obj);
    QCOMPARE(m, actionButton);
}

#include "moc_actionbuttontest.cpp"
