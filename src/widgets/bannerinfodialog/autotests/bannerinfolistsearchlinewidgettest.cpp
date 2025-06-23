/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfolistsearchlinewidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "bannerinfodialog/bannerinfolistsearchlinewidget.h"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSignalSpy>
#include <QTest>

QTEST_MAIN(BannerInfoListSearchLineWidgetTest)
BannerInfoListSearchLineWidgetTest::BannerInfoListSearchLineWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void BannerInfoListSearchLineWidgetTest::shouldHaveDefaultValues()
{
    BannerInfoListSearchLineWidget w;

    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mSearchLineEdit = w.findChild<QLineEdit *>(u"mSearchLineEdit"_s);
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->text().isEmpty());
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());

    auto mOnlyUnReadCheckBox = w.findChild<QCheckBox *>(u"mOnlyUnReadCheckBox"_s);
    QVERIFY(mOnlyUnReadCheckBox);
    QVERIFY(!mOnlyUnReadCheckBox->isChecked());

    QVERIFY(w.searchText().isEmpty());
    QVERIFY(!w.showOnlyUnread());
}

void BannerInfoListSearchLineWidgetTest::shouldEmitFilterChanged()
{
    BannerInfoListSearchLineWidget w;
    QSignalSpy filterChanged(&w, &BannerInfoListSearchLineWidget::filterChanged);

    auto mSearchLineEdit = w.findChild<QLineEdit *>(u"mSearchLineEdit"_s);
    mSearchLineEdit->setText(u"bla"_s);
    QCOMPARE(filterChanged.count(), 1);

    filterChanged.clear();
    mSearchLineEdit->setText(QString());
    QCOMPARE(filterChanged.count(), 1);

    filterChanged.clear();
    auto mOnlyUnReadCheckBox = w.findChild<QCheckBox *>(u"mOnlyUnReadCheckBox"_s);
    QTest::mouseClick(mOnlyUnReadCheckBox, Qt::LeftButton);
    QCOMPARE(filterChanged.count(), 1);
}

#include "moc_bannerinfolistsearchlinewidgettest.cpp"
