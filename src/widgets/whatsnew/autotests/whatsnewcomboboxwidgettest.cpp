/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "whatsnewcomboboxwidgettest.h"
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
    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto hboxLayout = w.findChild<QHBoxLayout *>(QStringLiteral("hboxLayout"));
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), QMargins{});

    auto mVersionComboBox = w.findChild<QComboBox *>(QStringLiteral("mVersionComboBox"));
    QVERIFY(mVersionComboBox);

    auto label = w.findChild<QLabel *>(QStringLiteral("label"));
    QVERIFY(label);
    QVERIFY(!label->text().isEmpty());

    auto separator = w.findChild<KSeparator *>(QStringLiteral("separator"));
    QVERIFY(separator);
}

void WhatsNewComboBoxWidgetTest::shouldInitializeComboBox()
{
    WhatsNewComboBoxWidget w;
    w.initializeVersion();
    auto mVersionComboBox = w.findChild<QComboBox *>(QStringLiteral("mVersionComboBox"));
    QCOMPARE(mVersionComboBox->currentData(), WhatsNewComboBoxWidget::Version2_0);
}

void WhatsNewComboBoxWidgetTest::shouldEmitVersionChanged()
{
    WhatsNewComboBoxWidget w;
    QSignalSpy versionChanged(&w, &WhatsNewComboBoxWidget::versionChanged);

    auto mVersionComboBox = w.findChild<QComboBox *>(QStringLiteral("mVersionComboBox"));
    mVersionComboBox->setCurrentIndex(mVersionComboBox->findData(WhatsNewComboBoxWidget::AllVersion));
    QCOMPARE(versionChanged.count(), 0);

    // Same => not emitted.
    mVersionComboBox->setCurrentIndex(mVersionComboBox->findData(WhatsNewComboBoxWidget::AllVersion));
    QCOMPARE(versionChanged.count(), 0);

    // Different => emitted.
    mVersionComboBox->setCurrentIndex(mVersionComboBox->findData(WhatsNewComboBoxWidget::Version2_0));
    QCOMPARE(versionChanged.count(), 1);
}

#include "moc_whatsnewcomboboxwidgettest.cpp"
