/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "exploreappstranslationdialogtest.h"
#include "exploreappstranlationdialog/exploreappstranslationdialog.h"
#include "exploreappstranlationdialog/exploreappstranslationwidget.h"
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ExploreAppsTranslationDialogTest)

ExploreAppsTranslationDialogTest::ExploreAppsTranslationDialogTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ExploreAppsTranslationDialogTest::shouldHaveDefaultValues()
{
    ExploreAppsTranslationDialog d;
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto mExploreAppsTranslationWidget = d.findChild<ExploreAppsTranslationWidget *>(QStringLiteral("mExploreAppsTranslationWidget"));
    QVERIFY(mExploreAppsTranslationWidget);

    auto button = d.findChild<QDialogButtonBox *>(QStringLiteral("button"));
    QVERIFY(button);
}

#include "moc_exploreappstranslationdialogtest.cpp"
