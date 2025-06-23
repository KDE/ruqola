/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewdialogtest.h"
using namespace Qt::Literals::StringLiterals;

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
    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    auto mWhatsNewWidget = d.findChild<WhatsNewWidget *>(u"mWhatsNewWidget"_s);
    QVERIFY(mWhatsNewWidget);

    auto button = d.findChild<QDialogButtonBox *>(u"button"_s);
    QVERIFY(button);
}

#include "moc_whatsnewdialogtest.cpp"
