/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverinfodialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mServerInfoWidget = w.findChild<ServerInfoWidget *>(u"mServerInfoWidget"_s);
    QVERIFY(mServerInfoWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_serverinfodialogtest.cpp"
