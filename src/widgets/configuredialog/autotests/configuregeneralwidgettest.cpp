/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configuregeneralwidgettest.h"
#include "configuredialog/configuregeneralwidget.h"
#include <QCheckBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfigureGeneralWidgetTest)

ConfigureGeneralWidgetTest::ConfigureGeneralWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ConfigureGeneralWidgetTest::shouldHaveDefaultValues()
{
    ConfigureGeneralWidget w;
    // Load otherwise we can't load default settings
    w.load();

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mSetOnlineForAllAccount = w.findChild<QCheckBox *>(QStringLiteral("mSetOnlineForAllAccount"));
    QVERIFY(mSetOnlineForAllAccount);
    QVERIFY(mSetOnlineForAllAccount->isChecked());
    QVERIFY(!mSetOnlineForAllAccount->text().isEmpty());

    auto mShowImageByDefault = w.findChild<QCheckBox *>(QStringLiteral("mShowImageByDefault"));
    QVERIFY(mShowImageByDefault);
    QVERIFY(mShowImageByDefault->isChecked());
    QVERIFY(!mShowImageByDefault->text().isEmpty());

    auto mMarkAsReadOnTextClicked = w.findChild<QCheckBox *>(QStringLiteral("mMarkAsReadOnTextClicked"));
    QVERIFY(mMarkAsReadOnTextClicked);
    QVERIFY(!mMarkAsReadOnTextClicked->isChecked());
    QVERIFY(!mMarkAsReadOnTextClicked->text().isEmpty());

    auto mEnableSystemTray = w.findChild<QCheckBox *>(QStringLiteral("mEnableSystemTray"));
    QVERIFY(mEnableSystemTray);
    QVERIFY(mEnableSystemTray->isChecked());
    QVERIFY(!mEnableSystemTray->text().isEmpty());

    auto mEnableLogging = w.findChild<QCheckBox *>(QStringLiteral("mEnableLogging"));
    QVERIFY(mEnableLogging);
    QVERIFY(mEnableLogging->isChecked());
    QVERIFY(!mEnableLogging->text().isEmpty());

    auto mShowHoverHightLights = w.findChild<QCheckBox *>(QStringLiteral("mShowHoverHightLights"));
    QVERIFY(mShowHoverHightLights);
    QVERIFY(mShowHoverHightLights->isChecked());
    QVERIFY(!mShowHoverHightLights->text().isEmpty());

    auto mAnimateGif = w.findChild<QCheckBox *>(QStringLiteral("mAnimateGif"));
    QVERIFY(mAnimateGif);
    QVERIFY(mAnimateGif->isChecked());
    QVERIFY(!mAnimateGif->text().isEmpty());

    auto mStoreMessageInDataBase = w.findChild<QCheckBox *>(QStringLiteral("mStoreMessageInDataBase"));
    QVERIFY(mStoreMessageInDataBase);
    QVERIFY(!mStoreMessageInDataBase->isChecked());
    QVERIFY(!mStoreMessageInDataBase->text().isEmpty());
}

#include "moc_configuregeneralwidgettest.cpp"
