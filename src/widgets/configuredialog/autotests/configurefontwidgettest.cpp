/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurefontwidgettest.h"
using namespace Qt::Literals::StringLiterals;

#include "configuredialog/configurefontwidget.h"
#include <KFontChooser>
#include <QCheckBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(ConfigureFontWidgetTest)

ConfigureFontWidgetTest::ConfigureFontWidgetTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

void ConfigureFontWidgetTest::shouldHaveDefaultValues()
{
    ConfigureFontWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mCustomFontCheck = w.findChild<QCheckBox *>(u"mCustomFontCheck"_s);
    QVERIFY(mCustomFontCheck);
    QVERIFY(!mCustomFontCheck->isChecked());

    auto mFontChooser = w.findChild<KFontChooser *>(u"mFontChooser"_s);
    QVERIFY(mFontChooser);
    QVERIFY(!mFontChooser->isEnabled());
}

#include "moc_configurefontwidgettest.cpp"
