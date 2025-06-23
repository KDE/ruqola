/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "channelpassworddialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mChannelPasswordWidget = w.findChild<ChannelPasswordWidget *>(u"mChannelPasswordWidget"_s);
    QVERIFY(mChannelPasswordWidget);

    auto buttonBox = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(buttonBox);
}

#include "moc_channelpassworddialogtest.cpp"
