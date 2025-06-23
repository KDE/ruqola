/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "systemmessagesmodeltest.h"
using namespace Qt::Literals::StringLiterals;

#include "model/systemmessagesmodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(SystemMessagesModelTest)
SystemMessagesModelTest::SystemMessagesModelTest(QObject *parent)
    : QObject(parent)
{
}

void SystemMessagesModelTest::shouldHaveDefaultValues()
{
    SystemMessagesModel w;
    QCOMPARE(w.rowCount(), 36);
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
        u"au"_s,
        u"wm"_s,
        u"ut"_s,
        u"ul"_s,
        u"ult"_s,
        u"uj"_s,
        u"subscription-role-removed"_s,
        u"subscription-role-added"_s,
        u"ru"_s,
        u"room-unarchived"_s,
        u"room_disabled_encryption"_s,
        u"enabled_encryption"_s,
        u"room_changed_privacy"_s,
        u"room_changed_avatar"_s,
        u"mute_unmute"_s,
        u"r"_s,
        u"rm"_s,
        u"room-archived"_s,
        u"room_e2e_enabled"_s,
        u"room_e2e_disabled"_s,
        u"room-removed-read-only"_s,
        u"room-set-read-only"_s,
        u"room-disallowed-reacting"_s,
        u"room-allowed-reacting"_s,
        u"user-added-room-to-team"_s,
        u"user-converted-to-channel"_s,
        u"user-converted-to-team"_s,
        u"user-removed-room-from-team"_s,
        u"user-deleted-room-from-team"_s,
        u"ujt"_s,
        u"removed-user-from-team"_s,
        u"added-user-to-team"_s,
        u"room_changed_topic"_s,
        u"room_changed_announcement"_s,
        u"room_changed_description"_s,
    };

    for (const QString &r : std::as_const(lst)) {
        const bool containsRole = lstRoles.contains(r);
        if (!containsRole) {
            qDebug() << " r " << r;
        }
        QVERIFY(containsRole);
        lstRoles.removeAll(r);
    }
    const bool lstIsEmpty = lstRoles.isEmpty();
    if (!lstIsEmpty) {
        qDebug() << " missing " << lstRoles;
    }
    QVERIFY(lstIsEmpty);
}

#include "moc_systemmessagesmodeltest.cpp"
