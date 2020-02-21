/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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

#include "emoticonmodeltest.h"
#include "model/emoticonfiltermodel.h"
#include <QTest>
#include <model/emoticonmodel.h>
QTEST_GUILESS_MAIN(EmoticonModelTest)

EmoticonModelTest::EmoticonModelTest(QObject *parent)
    : QObject(parent)
{
}

void EmoticonModelTest::shouldHaveDefaultValue()
{
    EmoticonFilterModel w;
    QVERIFY(w.currentCategory().isEmpty());
    QVERIFY(w.emoticons().isEmpty());

    QHash<int, QByteArray> roles;
    roles[EmoticonFilterModel::Identifier] = QByteArrayLiteral("identifier");
    roles[EmoticonFilterModel::EmoticonFilterModel::Text] = QByteArrayLiteral("text");
    roles[EmoticonFilterModel::UnicodeEmoji] = QByteArrayLiteral("unicodeEmoji");
    roles[EmoticonFilterModel::Order] = QByteArrayLiteral("order");
    QCOMPARE(w.roleNames(), roles);
    QVERIFY(w.emoticonCategoriesModel());
}

void EmoticonModelTest::shouldAssignCategory()
{
    EmoticonFilterModel w;
    const QString category = QStringLiteral("bla");
    w.setCurrentCategory(category);
    QCOMPARE(w.currentCategory(), category);
}

void EmoticonModelTest::shouldListEmojis()
{
    // GIVEN
    EmoticonModel model;
    QCOMPARE(model.rowCount(), 0);
    UnicodeEmoticon icon1;
    icon1.setIdentifier(QStringLiteral("id1"));
    QVector<UnicodeEmoticon> list{ icon1 };

    // WHEN
    model.setEmoticons(list);

    // THEN
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.index(0, 0).data(EmoticonModel::Identifier).toString(), QStringLiteral("id1"));
}

//TODO add more check
