/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewchanneldialogtest.h"
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

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mCreateNewChannelWidget = w.findChild<CreateNewChannelWidget *>(QStringLiteral("mCreateNewChannelWidget"));
    QVERIFY(mCreateNewChannelWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);

    QPushButton *mOkButton = button->button(QDialogButtonBox::Ok);
    QVERIFY(!mOkButton->isEnabled());
}

#include "moc_createnewchanneldialogtest.cpp"
