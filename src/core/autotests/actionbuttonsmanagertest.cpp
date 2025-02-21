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
    // TODO
}

void ActionButtonsManagerTest::shouldLoadActionButtons()
{
    QFETCH(QString, name);
    QFETCH(QList<ActionButton>, actionButtons);
    const QString originalJsonFile = QLatin1StringView(RUQOLA_DATA_DIR) + "/actionbuttons/"_L1 + name + ".json"_L1;
    const QJsonObject obj = AutoTestHelper::loadJsonObject(originalJsonFile);
    ActionButtonsManager m;
    // m.parseActionButton(obj);
    QCOMPARE(m.actionButtons(), actionButtons);
}

#include "moc_actionbuttonsmanagertest.cpp"
