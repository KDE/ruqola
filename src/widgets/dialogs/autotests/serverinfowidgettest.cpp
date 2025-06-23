/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverinfowidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "dialogs/serverinfo/serverinfowidget.h"
#include <QFormLayout>
#include <QLabel>
#include <QTest>
QTEST_MAIN(ServerInfoWidgetTest)
ServerInfoWidgetTest::ServerInfoWidgetTest(QWidget *parent)
    : QWidget(parent)
{
}

void ServerInfoWidgetTest::shouldHaveDefaultValues()
{
    ServerInfoWidget w(nullptr);
    auto layout = w.findChild<QFormLayout *>(u"layout"_s);
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins{});

    auto mAccountName = w.findChild<QLabel *>(u"mAccountName"_s);
    QVERIFY(mAccountName);
    QVERIFY(mAccountName->text().isEmpty());
    QCOMPARE(mAccountName->textFormat(), Qt::PlainText);
    QCOMPARE(mAccountName->textInteractionFlags(), Qt::TextSelectableByMouse);

    auto mUserName = w.findChild<QLabel *>(u"mUserName"_s);
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());
    QCOMPARE(mUserName->textFormat(), Qt::PlainText);
    QCOMPARE(mUserName->textInteractionFlags(), Qt::TextSelectableByMouse);

    auto mServerVersion = w.findChild<QLabel *>(u"mServerVersion"_s);
    QVERIFY(mServerVersion);
    QVERIFY(mServerVersion->text().isEmpty());
    QCOMPARE(mServerVersion->textFormat(), Qt::PlainText);
    QCOMPARE(mServerVersion->textInteractionFlags(), Qt::TextSelectableByMouse);

    auto mServerUrl = w.findChild<QLabel *>(u"mServerUrl"_s);
    QVERIFY(mServerUrl);
    QVERIFY(mServerUrl->text().isEmpty());
    QCOMPARE(mServerUrl->textFormat(), Qt::RichText);
    QCOMPARE(mServerUrl->textInteractionFlags(), Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
}

#include "moc_serverinfowidgettest.cpp"
