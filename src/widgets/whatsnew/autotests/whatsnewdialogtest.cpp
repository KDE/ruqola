/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewdialogtest.h"
#include "whatsnew/whatsnewdialog.h"
#include "whatsnew/whatsnewwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(WhatsNewDialogTest)
WhatsNewDialogTest::WhatsNewDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void WhatsNewDialogTest::shouldHaveDefaultValues()
{
    WhatsNewDialog d;
    QVERIFY(!d.windowTitle().isEmpty());
    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    auto mWhatsNewWidget = d.findChild<WhatsNewWidget *>(QStringLiteral("mWhatsNewWidget"));
    QVERIFY(mWhatsNewWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_whatsnewdialogtest.cpp"
