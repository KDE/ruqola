/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "systemmessagesmodeltest.h"
#include "model/systemmessagesmodel.h"
#include <QTest>
QTEST_MAIN(SystemMessagesModelTest)
SystemMessagesModelTest::SystemMessagesModelTest(QObject *parent)
    : QObject(parent)
{
}

void SystemMessagesModelTest::shouldHaveDefaultValues()
{
    SystemMessagesModel w;
    QCOMPARE(w.rowCount(), 20);
    QVERIFY(w.systemMessagesSelected().isEmpty());
    QStringList lst;
    const int rowCountNb = w.rowCount();
    // First one is not a message type
    for (int i = 1; i < rowCountNb; i++) {
        QStandardItem *itemModel = w.item(i);
        if (itemModel) {
            lst.append(itemModel->data(SystemMessagesModel::SystemMessages).toString());
        }
    }
    QStringList lstRoles{
        QStringLiteral("au"),
        QStringLiteral("wm"),
        QStringLiteral("ut"),
        QStringLiteral("ul"),
        QStringLiteral("uj"),
        QStringLiteral("subscription-role-removed"),
        QStringLiteral("subscription-role-added"),
        QStringLiteral("ru"),
        QStringLiteral("room_unarchived"),
        QStringLiteral("room_disabled_encryption"),
        QStringLiteral("enabled_encryption"),
        QStringLiteral("room_changed_privacy"),
        QStringLiteral("room_changed_avatar"),
        QStringLiteral("mute_unmute"),
        QStringLiteral("r"),
        QStringLiteral("rm"),
        QStringLiteral("room_archived"),
        QStringLiteral("room_e2e_enabled"),
        QStringLiteral("room_e2e_disabled"),
    };

    for (const QString &r : std::as_const(lst)) {
        QVERIFY(lstRoles.contains(r));
        lstRoles.removeAll(r);
    }
    QVERIFY(lstRoles.isEmpty());
}
