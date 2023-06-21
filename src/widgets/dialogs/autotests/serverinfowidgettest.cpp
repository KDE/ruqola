/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverinfowidgettest.h"
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
    auto layout = w.findChild<QFormLayout *>(QStringLiteral("layout"));
    QVERIFY(layout);
    QCOMPARE(layout->contentsMargins(), QMargins{});

    auto mAccountName = w.findChild<QLabel *>(QStringLiteral("mAccountName"));
    QVERIFY(mAccountName);
    QVERIFY(mAccountName->text().isEmpty());
    QCOMPARE(mAccountName->textFormat(), Qt::PlainText);
    QCOMPARE(mAccountName->textInteractionFlags(), Qt::TextSelectableByMouse);

    auto mUserName = w.findChild<QLabel *>(QStringLiteral("mUserName"));
    QVERIFY(mUserName);
    QVERIFY(mUserName->text().isEmpty());
    QCOMPARE(mUserName->textFormat(), Qt::PlainText);
    QCOMPARE(mUserName->textInteractionFlags(), Qt::TextSelectableByMouse);

    auto mServerVersion = w.findChild<QLabel *>(QStringLiteral("mServerVersion"));
    QVERIFY(mServerVersion);
    QVERIFY(mServerVersion->text().isEmpty());
    QCOMPARE(mServerVersion->textFormat(), Qt::PlainText);
    QCOMPARE(mServerVersion->textInteractionFlags(), Qt::TextSelectableByMouse);

    auto mServerUrl = w.findChild<QLabel *>(QStringLiteral("mServerUrl"));
    QVERIFY(mServerUrl);
    QVERIFY(mServerUrl->text().isEmpty());
    QCOMPARE(mServerUrl->textFormat(), Qt::RichText);
    QCOMPARE(mServerUrl->textInteractionFlags(), Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
}

#include "moc_serverinfowidgettest.cpp"
