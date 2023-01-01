/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewdiscussiondialogtest.h"
#include "dialogs/createnewdiscussiondialog.h"
#include "dialogs/createnewdiscussionwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(CreateNewDiscussionDialogTest)
CreateNewDiscussionDialogTest::CreateNewDiscussionDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void CreateNewDiscussionDialogTest::shouldHaveDefaultValues()
{
    CreateNewDiscussionDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mCreateNewDiscussionWidget = w.findChild<CreateNewDiscussionWidget *>(QStringLiteral("mCreateNewDiscussionWidget"));
    QVERIFY(mCreateNewDiscussionWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}
