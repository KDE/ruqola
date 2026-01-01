/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbuttonsmanagertest.h"
#include "actionbuttons/actionbuttonsmanager.h"
#include "ruqola_autotest_helper.h"
#include <QJsonArray>
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(ActionButtonsManagerTest)
ActionButtonsManagerTest::ActionButtonsManagerTest(QObject *parent)
    : QObject{parent}
{
}

void ActionButtonsManagerTest::shouldHaveDefaultValues()
{
    ActionButtonsManager m(nullptr);
    QVERIFY(m.actionButtons().isEmpty());
}

void ActionButtonsManagerTest::shouldLoadActionButtons_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QList<ActionButton>>("actionButtons");

    {
        QList<ActionButton> buttons;
        QTest::addRow("actionbuttons-empty") << u"actionbuttons-empty"_s << buttons;
    }
    {
        QList<ActionButton> buttons;
        ActionButton b;
        b.setAppId("821cd5c6-1fb5-4d9e-8e88-e6176463efb6");
        b.setActionId("auto-reply-room-action-id");
        b.setLabelI18n("auto-reply-room-action-name"_L1);
        b.setButtonContext(ActionButton::ButtonContext::RoomAction);
        b.setRoomTypeFilters(ActionButton::RoomTypeFilter::Direct);
        buttons.append(b);
        QTest::addRow("actionbuttons-test1") << u"actionbuttons-test1"_s << buttons;
    }
    {
        QList<ActionButton> buttons;
        {
            ActionButton b;
            b.setAppId("821cd5c6-1fb5-4d9e-8e88-e6176463efb6");
            b.setActionId("auto-reply-room-action-id");
            b.setLabelI18n("auto-reply-room-action-name"_L1);
            b.setButtonContext(ActionButton::ButtonContext::RoomAction);
            b.setRoomTypeFilters(ActionButton::RoomTypeFilter::Direct);
            buttons.append(b);
        }
        {
            ActionButton b;
            b.setAppId("7a8cd36b-5f7b-4177-bd7f-bfc9be908bf8");
            b.setActionId("remind_me_about_this_msg_action");
            b.setLabelI18n("remind_me_about_this_msg_action"_L1);
            b.setButtonContext(ActionButton::ButtonContext::MessageAction);
            buttons.append(b);
        }
        {
            ActionButton b;
            b.setAppId("7a8cd36b-5f7b-4177-bd7f-bfc9be908bf8");
            b.setActionId("add_reminder_message_box_action");
            b.setLabelI18n("add_reminder_message_box_action"_L1);
            b.setButtonContext(ActionButton::ButtonContext::MessageBoxAction);
            buttons.append(b);
        }
        {
            ActionButton b;
            b.setAppId("7a8cd36b-5f7b-4177-bd7f-bfc9be908bf8");
            b.setActionId("show_my_reminders_room_action");
            b.setLabelI18n("show_my_reminders_room_action"_L1);
            b.setButtonContext(ActionButton::ButtonContext::RoomAction);
            buttons.append(b);
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
            buttons.append(b);
        }
        QTest::addRow("actionbuttons-test2") << u"actionbuttons-test2"_s << buttons;
    }
    // TODO
}

void ActionButtonsManagerTest::shouldLoadActionButtons()
{
    QFETCH(QString, name);
    QFETCH(QList<ActionButton>, actionButtons);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/actionbuttons/"_L1 + name + ".json"_L1;
    const QJsonArray array = AutoTestHelper::loadJsonArrayObject(originalJsonFile);
    ActionButtonsManager m(nullptr);
    m.parseActionButtons(array);
    QCOMPARE(m.actionButtons(), actionButtons);
}

#include "moc_actionbuttonsmanagertest.cpp"
