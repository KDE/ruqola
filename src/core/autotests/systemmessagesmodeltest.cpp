/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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
