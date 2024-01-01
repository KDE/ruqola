/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showimagedialogtest.h"
#include "dialogs/showimagedialog.h"
#include "dialogs/showimagewidget.h"
#include <QDialogButtonBox>
#include <QMenu>
#include <QStandardPaths>
#include <QTest>
#include <QToolButton>
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
    QCOMPARE(button->standardButtons(), QDialogButtonBox::Close | QDialogButtonBox::Save);

    auto clipboardButton = w.findChild<QToolButton *>(QStringLiteral("clipboardButton"));
    QVERIFY(clipboardButton);

    auto mClipboardMenu = w.findChild<QMenu *>(QStringLiteral("mClipboardMenu"));
    QVERIFY(mClipboardMenu);

    auto clipboardImageAction = w.findChild<QAction *>(QStringLiteral("clipboardLocationAction"));
    QVERIFY(clipboardImageAction);
    QVERIFY(!clipboardImageAction->text().isEmpty());

    auto clipboardLocationAction = w.findChild<QAction *>(QStringLiteral("clipboardLocationAction"));
    QVERIFY(clipboardLocationAction);
    QVERIFY(!clipboardLocationAction->text().isEmpty());
}

#include "moc_showimagedialogtest.cpp"
