/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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
    EmoticonModel model(nullptr);
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
    EmoticonModel model(nullptr);
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
