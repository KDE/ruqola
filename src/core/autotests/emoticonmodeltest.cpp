/*
   Copyright (c) 2018-2021 Laurent Montel <montel@kde.org>

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
    EmoticonFilterModel filter;
    QVERIFY(filter.currentCategory().isEmpty());
    QVERIFY(filter.emoticonCategoriesModel());
}

void EmoticonModelTest::shouldListEmojis()
{
    // GIVEN
    EmoticonModel model;
    QCOMPARE(model.rowCount(), 0);
    UnicodeEmoticon icon1;
    icon1.setUnicode(QStringLiteral("D83D-DE42"));
    icon1.setIdentifier(QStringLiteral("id1"));
    icon1.setCategory(QStringLiteral("cat1"));
    icon1.setAliases({QStringLiteral("id2"), QStringLiteral("id3")});
    QVector<UnicodeEmoticon> list{icon1};

    // WHEN
    model.setUnicodeEmoticons(list);

    // THEN
    QCOMPARE(model.rowCount(), 3);

    const auto idx1 = model.index(0, 0);
    QCOMPARE(idx1.data(EmoticonModel::Identifier).toString(), icon1.identifier());
    QCOMPARE(idx1.data(EmoticonModel::Category).toString(), icon1.category());
    QCOMPARE(idx1.data(EmoticonModel::UnicodeEmoji).toString(), icon1.unicode());

    const auto idx2 = model.index(1, 0);
    QCOMPARE(idx2.data(EmoticonModel::Identifier).toString(), QStringLiteral("id2"));
    QCOMPARE(idx2.data(EmoticonModel::Category).toString(), icon1.category());
    QCOMPARE(idx1.data(EmoticonModel::UnicodeEmoji).toString(), icon1.unicode());

    const auto idx3 = model.index(2, 0);
    QCOMPARE(idx3.data(EmoticonModel::Identifier).toString(), QStringLiteral("id3"));
    QCOMPARE(idx3.data(EmoticonModel::Category).toString(), icon1.category());
    QCOMPARE(idx1.data(EmoticonModel::UnicodeEmoji).toString(), icon1.unicode());
}

void EmoticonModelTest::shouldFilterCategory()
{
    // GIVEN
    EmoticonModel model;
    EmoticonFilterModel filterModel;
    filterModel.setSourceModel(&model);
    UnicodeEmoticon icon1;
    icon1.setIdentifier(QStringLiteral("id1"));
    icon1.setCategory(QStringLiteral("cat1"));
    QVector<UnicodeEmoticon> list{icon1};
    model.setUnicodeEmoticons(list);

    // WHEN
    const QString category = QStringLiteral("cat1");
    filterModel.setCurrentCategory(category);

    // THEN
    QCOMPARE(filterModel.currentCategory(), category);
    QCOMPARE(filterModel.rowCount(), 1);

    // WHEN
    const QString unknown = QStringLiteral("unknown");
    filterModel.setCurrentCategory(unknown);

    // THEN
    QCOMPARE(filterModel.currentCategory(), unknown);
    QCOMPARE(filterModel.rowCount(), 0);
}

// TODO add more check
