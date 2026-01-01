/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewchanneldialogtest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/createnewchanneldialog.h"
#include "dialogs/createnewchannelwidget.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(CreateNewChannelDialogTest)
CreateNewChannelDialogTest::CreateNewChannelDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void CreateNewChannelDialogTest::shouldHaveDefaultValues()
{
    CreateNewChannelDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mCreateNewChannelWidget = w.findChild<CreateNewChannelWidget *>(u"mCreateNewChannelWidget"_s);
    QVERIFY(mCreateNewChannelWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);

    QPushButton *mOkButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(!mOkButton->isEnabled());
}

#include "moc_createnewchanneldialogtest.cpp"
