/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverinfodialogtest.h"
#include "dialogs/serverinfo/serverinfodialog.h"
#include "dialogs/serverinfo/serverinfowidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ServerInfoDialogTest)
ServerInfoDialogTest::ServerInfoDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ServerInfoDialogTest::shouldHaveDefaultValues()
{
    ServerInfoDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mServerInfoWidget = w.findChild<ServerInfoWidget *>(QStringLiteral("mServerInfoWidget"));
    QVERIFY(mServerInfoWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_serverinfodialogtest.cpp"
