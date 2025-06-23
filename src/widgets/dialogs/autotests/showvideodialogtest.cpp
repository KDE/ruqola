/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showvideodialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mShowVideoWidget = w.findChild<ShowVideoWidget *>(u"mShowVideoWidget"_s);
    QVERIFY(mShowVideoWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_showvideodialogtest.cpp"
