/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showvideodialogtest.h"
#include "dialogs/showvideodialog.h"
#include "dialogs/showvideowidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ShowVideoDialogTest)
ShowVideoDialogTest::ShowVideoDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ShowVideoDialogTest::shouldHaveDefaultValues()
{
    ShowVideoDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mShowVideoWidget = w.findChild<ShowVideoWidget *>(QStringLiteral("mShowVideoWidget"));
    QVERIFY(mShowVideoWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_showvideodialogtest.cpp"
