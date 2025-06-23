/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewcomboboxwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "whatsnew/whatsnewcomboboxwidget.h"
#include <KSeparator>
#include <QComboBox>
#include <QLabel>
#include <QSignalSpy>
#include <QTest>
#include <QVBoxLayout>

QTEST_MAIN(WhatsNewComboBoxWidgetTest)

WhatsNewComboBoxWidgetTest::WhatsNewComboBoxWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void WhatsNewComboBoxWidgetTest::shouldHaveDefaultValues()
{
    WhatsNewComboBoxWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto hboxLayout = w.findChild<QHBoxLayout *>(u"hboxLayout"_s);
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), QMargins{});

    auto mVersionComboBox = w.findChild<QComboBox *>(u"mVersionComboBox"_s);
    QVERIFY(mVersionComboBox);

    auto label = w.findChild<QLabel *>(u"label"_s);
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto separator = w.findChild<KSeparator *>(u"separator"_s);
    QVERIFY(separator);
}

void WhatsNewComboBoxWidgetTest::shouldInitializeComboBox()
{
    /*
    WhatsNewComboBoxWidget w;
    w.initializeVersion(WhatsNewUtils::Version2_0);
    auto mVersionComboBox = w.findChild<QComboBox *>(u"mVersionComboBox"_s);
    QCOMPARE(mVersionComboBox->currentData(), WhatsNewUtils::Version2_0);
    */
}

void WhatsNewComboBoxWidgetTest::shouldEmitVersionChanged()
{
    /*
    WhatsNewComboBoxWidget w;
    QSignalSpy versionChanged(&w, &WhatsNewComboBoxWidget::versionChanged);

    auto mVersionComboBox = w.findChild<QComboBox *>(u"mVersionComboBox"_s);
    mVersionComboBox->setCurrentIndex(mVersionComboBox->findData(WhatsNewUtils::AllVersion));
    QCOMPARE(versionChanged.count(), 0);

    // Same => not emitted.
    mVersionComboBox->setCurrentIndex(mVersionComboBox->findData(WhatsNewUtils::AllVersion));
    QCOMPARE(versionChanged.count(), 0);

    // Different => emitted.
    mVersionComboBox->setCurrentIndex(mVersionComboBox->findData(WhatsNewUtils::Version2_0));
    QCOMPARE(versionChanged.count(), 1);
    */
}

#include "moc_whatsnewcomboboxwidgettest.cpp"
