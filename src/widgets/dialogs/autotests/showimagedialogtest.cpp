/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "showimagedialogtest.h"
#include "dialogs/showimagedialog.h"
#include "dialogs/showimagewidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ShowImageDialogTest)

ShowImageDialogTest::ShowImageDialogTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ShowImageDialogTest::shouldHaveDefaultValues()
{
    ShowImageDialog w(nullptr);
    QVERIFY(!w.windowTitle().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mShowImageWidget = w.findChild<ShowImageWidget *>(QStringLiteral("mShowImageWidget"));
    QVERIFY(mShowImageWidget);

    auto button = w.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
    QCOMPARE(button->standardButtons(), {QDialogButtonBox::Close | QDialogButtonBox::Save});
}
