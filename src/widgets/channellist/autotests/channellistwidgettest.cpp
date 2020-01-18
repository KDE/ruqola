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

#include "channellistwidgettest.h"
#include "channellist/channellistwidget.h"
#include "channellist/statuscombobox.h"
#include "channellist/channellistview.h"

#include <QLabel>
#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(ChannelListWidgetTest)
ChannelListWidgetTest::ChannelListWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ChannelListWidgetTest::shouldHaveDefaultValues()
{
    ChannelListWidget w;

    QVBoxLayout *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainlayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    QLineEdit *mSearchRoom = w.findChild<QLineEdit *>(QStringLiteral("mSearchRoom"));
    QVERIFY(mSearchRoom);

    StatusCombobox *mStatusComboBox = w.findChild<StatusCombobox *>(QStringLiteral("mStatusComboBox"));
    QVERIFY(mStatusComboBox);

    ChannelListView *mChannelView = w.findChild<ChannelListView *>(QStringLiteral("mChannelView"));
    QVERIFY(mChannelView);

    QLabel *label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());
}
