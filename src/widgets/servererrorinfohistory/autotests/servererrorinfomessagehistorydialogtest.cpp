/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagehistorydialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "servererrorinfohistory/servererrorinfomessagehistorydialog.h"
#include "servererrorinfohistory/servererrorinfomessagehistorywidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ServerErrorInfoMessageHistoryDialogTest)
ServerErrorInfoMessageHistoryDialogTest::ServerErrorInfoMessageHistoryDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ServerErrorInfoMessageHistoryDialogTest::shouldHaveDefaultValues()
{
    ServerErrorInfoMessageHistoryDialog d;
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    auto mServerErrorInfoMessageHistoryWidget = d.findChild<ServerErrorInfoMessageHistoryWidget *>(u"mServerErrorInfoMessageHistoryWidget"_s);
    QVERIFY(mServerErrorInfoMessageHistoryWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_servererrorinfomessagehistorydialogtest.cpp"
