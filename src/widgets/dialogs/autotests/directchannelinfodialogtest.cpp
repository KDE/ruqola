/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directchannelinfodialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/directchannelinfodialog.h"
#include "dialogs/directchannelinfowidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(DirectChannelInfoDialogTest)

DirectChannelInfoDialogTest::DirectChannelInfoDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void DirectChannelInfoDialogTest::shouldHaveDefaultValue()
{
    DirectChannelInfoDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mDirectChannelInfoWidget = w.findChild<DirectChannelInfoWidget *>(u"mDirectChannelInfoWidget"_s);
    QVERIFY(mDirectChannelInfoWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"buttonBox"_s);
    QVERIFY(button);
}

#include "moc_directchannelinfodialogtest.cpp"
