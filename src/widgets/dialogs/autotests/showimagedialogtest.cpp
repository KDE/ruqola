/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showimagedialogtest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mShowImageWidget = w.findChild<ShowImageWidget *>(u"mShowImageWidget"_s);
    QVERIFY(mShowImageWidget);

    auto button = w.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
    QCOMPARE(button->standardButtons(), QDialogButtonBox::Close | QDialogButtonBox::Save);

    auto clipboardButton = w.findChild<QToolButton *>(u"clipboardButton"_s);
    QVERIFY(clipboardButton);

    auto mClipboardMenu = w.findChild<QMenu *>(u"mClipboardMenu"_s);
    QVERIFY(mClipboardMenu);

    auto clipboardImageAction = w.findChild<QAction *>(u"clipboardLocationAction"_s);
    QVERIFY(clipboardImageAction);
    QVERIFY(!clipboardImageAction->text().isEmpty());

    auto clipboardLocationAction = w.findChild<QAction *>(u"clipboardLocationAction"_s);
    QVERIFY(clipboardLocationAction);
    QVERIFY(!clipboardLocationAction->text().isEmpty());
}

#include "moc_showimagedialogtest.cpp"
