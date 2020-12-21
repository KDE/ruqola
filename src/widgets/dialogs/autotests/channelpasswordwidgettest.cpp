/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "channelpasswordwidgettest.h"
#include "dialogs/channelpasswordwidget.h"
#include <KPasswordLineEdit>
#include <QHBoxLayout>
#include <QSignalSpy>
#include <QLabel>
#include <QTest>
#include <QList>
QTEST_MAIN(ChannelPasswordWidgetTest)
ChannelPasswordWidgetTest::ChannelPasswordWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelPasswordWidgetTest::shouldHaveDefaultValues()
{
    ChannelPasswordWidget w;
    auto mainLayout = w.findChild<QHBoxLayout * >(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QVERIFY(mPasswordLineEdit);

    QVERIFY(w.password().isEmpty());
}

void ChannelPasswordWidgetTest::shouldEmitSignals()
{
    ChannelPasswordWidget w;
    auto mPasswordLineEdit = w.findChild<KPasswordLineEdit *>(QStringLiteral("mPasswordLineEdit"));
    QSignalSpy spy(&w, &ChannelPasswordWidget::updateOkButton);
    mPasswordLineEdit->setPassword(QStringLiteral("bla"));
    QVERIFY(spy.count() > 0);
    QVERIFY(spy.at(0).at(0).toBool());
    spy.clear();
    mPasswordLineEdit->setPassword({});
    QVERIFY(spy.count() > 0);
    QVERIFY(!spy.at(0).at(0).toBool());
}
