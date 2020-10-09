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

#include "configuregeneralwidgettest.h"
#include "configuredialog/configuregeneralwidget.h"
#include <QCheckBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfigureGeneralWidgetTest)

ConfigureGeneralWidgetTest::ConfigureGeneralWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void ConfigureGeneralWidgetTest::shouldHaveDefaultValues()
{
    ConfigureGeneralWidget w;

    QVBoxLayout *mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins(0, 0, 0, 0));

    QCheckBox *mSetOnlineForAllAccount = w.findChild<QCheckBox *>(QStringLiteral("mSetOnlineForAllAccount"));
    QVERIFY(mSetOnlineForAllAccount);
    QVERIFY(!mSetOnlineForAllAccount->isChecked());
    QVERIFY(!mSetOnlineForAllAccount->text().isEmpty());

    QCheckBox *mShowImageByDefault = w.findChild<QCheckBox *>(QStringLiteral("mShowImageByDefault"));
    QVERIFY(mShowImageByDefault);
    QVERIFY(!mShowImageByDefault->isChecked());
    QVERIFY(!mShowImageByDefault->text().isEmpty());


    QCheckBox *mShowRoomAvatar = w.findChild<QCheckBox *>(QStringLiteral("mShowRoomAvatar"));
    QVERIFY(mShowRoomAvatar);
    QVERIFY(!mShowRoomAvatar->isChecked());
    QVERIFY(!mShowRoomAvatar->text().isEmpty());
}
