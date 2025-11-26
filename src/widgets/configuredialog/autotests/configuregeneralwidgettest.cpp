/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configuregeneralwidgettest.h"
using namespace Qt::Literals::StringLiterals;

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

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mSetOnlineForAllAccount = w.findChild<QCheckBox *>(u"mSetOnlineForAllAccount"_s);
    QVERIFY(mSetOnlineForAllAccount);
    QVERIFY(mSetOnlineForAllAccount->isChecked());
    QVERIFY(!mSetOnlineForAllAccount->text().isEmpty());

    auto mShowImageByDefault = w.findChild<QCheckBox *>(u"mShowImageByDefault"_s);
    QVERIFY(mShowImageByDefault);
    QVERIFY(mShowImageByDefault->isChecked());
    QVERIFY(!mShowImageByDefault->text().isEmpty());

    auto mShowPreviewUrlByDefault = w.findChild<QCheckBox *>(u"mShowPreviewUrlByDefault"_s);
    QVERIFY(mShowPreviewUrlByDefault);
    QVERIFY(mShowPreviewUrlByDefault->isChecked());
    QVERIFY(!mShowPreviewUrlByDefault->text().isEmpty());

    auto mMarkAsReadOnTextClicked = w.findChild<QCheckBox *>(u"mMarkAsReadOnTextClicked"_s);
    QVERIFY(mMarkAsReadOnTextClicked);
    QVERIFY(!mMarkAsReadOnTextClicked->isChecked());
    QVERIFY(!mMarkAsReadOnTextClicked->text().isEmpty());

    auto mEnableSystemTray = w.findChild<QCheckBox *>(u"mEnableSystemTray"_s);
    QVERIFY(mEnableSystemTray);
    QVERIFY(mEnableSystemTray->isChecked());
    QVERIFY(!mEnableSystemTray->text().isEmpty());

    auto mEnableLogging = w.findChild<QCheckBox *>(u"mEnableLogging"_s);
    QVERIFY(mEnableLogging);
    QVERIFY(mEnableLogging->isChecked());
    QVERIFY(!mEnableLogging->text().isEmpty());

    auto mShowHoverHightLights = w.findChild<QCheckBox *>(u"mShowHoverHightLights"_s);
    QVERIFY(mShowHoverHightLights);
    QVERIFY(mShowHoverHightLights->isChecked());
    QVERIFY(!mShowHoverHightLights->text().isEmpty());

    auto mAnimateGif = w.findChild<QCheckBox *>(u"mAnimateGif"_s);
    QVERIFY(mAnimateGif);
    QVERIFY(mAnimateGif->isChecked());
    QVERIFY(!mAnimateGif->text().isEmpty());

    auto mStoreMessageInDataBase = w.findChild<QCheckBox *>(u"mStoreMessageInDataBase"_s);
    QVERIFY(mStoreMessageInDataBase);
    QVERIFY(mStoreMessageInDataBase->isChecked());
    QVERIFY(!mStoreMessageInDataBase->text().isEmpty());
}

#include "moc_configuregeneralwidgettest.cpp"
