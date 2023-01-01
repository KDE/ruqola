/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelpassworddialogtest.h"
#include "dialogs/channelpassworddialog.h"
#include "dialogs/channelpasswordwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ChannelPasswordDialogTest)
ChannelPasswordDialogTest::ChannelPasswordDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ChannelPasswordDialogTest::shouldHaveDefaultValues()
{
    ChannelPasswordDialog w;
    QVERIFY(!w.windowTitle().isEmpty());
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mChannelPasswordWidget = w.findChild<ChannelPasswordWidget *>(QStringLiteral("mChannelPasswordWidget"));
    QVERIFY(mChannelPasswordWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(buttonBox);
}
