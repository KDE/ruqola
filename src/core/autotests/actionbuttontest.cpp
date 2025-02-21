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
    QVERIFY(b.oneRole().isEmpty());

    QCOMPARE(b.roomTypeFilter(), ActionButton::RoomTypeFilter::Unknown);
    QCOMPARE(b.context(), ActionButton::ButtonContext::Unknown);
}

void ActionButtonTest::shouldLoadActionButton_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<ActionButton>("actionButton");

    {
        ActionButton appsCountInfo;
        QTest::addRow("actionbutton-empty") << QStringLiteral("actionbutton-empty") << appsCountInfo;
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
