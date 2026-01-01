/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewdiscussiondialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mCreateNewDiscussionWidget = w.findChild<CreateNewDiscussionWidget *>(u"mCreateNewDiscussionWidget"_s);
    QVERIFY(mCreateNewDiscussionWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_createnewdiscussiondialogtest.cpp"
