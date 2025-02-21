/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "actionbuttonsmanagertest.h"
#include "actionbuttons/actionbuttonsmanager.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(ActionButtonsManagerTest)
ActionButtonsManagerTest::ActionButtonsManagerTest(QObject *parent)
    : QObject{parent}
{
}

void ActionButtonsManagerTest::shouldHaveDefaultValues()
{
    ActionButtonsManager m;
    QVERIFY(m.actionButtons().isEmpty());
}

void ActionButtonsManagerTest::shouldLoadActionButtons_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QList<ActionButton>>("actionButtons");

    {
        QList<ActionButton> buttons;
        QTest::addRow("actionbuttons-empty") << QStringLiteral("actionbuttons-empty") << buttons;
    }
    {
        QList<ActionButton> buttons;
        ActionButton b;
        b.setAppId("821cd5c6-1fb5-4d9e-8e88-e6176463efb6");
        b.setActionId("auto-reply-room-action-id");
        b.setLabelI18n("auto-reply-room-action-name"_L1);
        b.setContext(ActionButton::ButtonContext::RoomAction);
        b.setRoomTypeFilters(ActionButton::RoomTypeFilter::Direct);
        buttons.append(b);
        QTest::addRow("actionbuttons-test1") << QStringLiteral("actionbuttons-test1") << buttons;
    }
    // TODO
}

void ActionButtonsManagerTest::shouldLoadActionButtons()
{
    QFETCH(QString, name);
    QFETCH(QList<ActionButton>, actionButtons);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/actionbuttons/"_L1 + name + ".json"_L1;
    const QJsonArray array = AutoTestHelper::loadJsonArrayObject(originalJsonFile);
    ActionButtonsManager m;
    m.parseActionButtons(array);
    QCOMPARE(m.actionButtons(), actionButtons);
}

#include "moc_actionbuttonsmanagertest.cpp"
