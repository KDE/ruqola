/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurespeechtotextconfigurewidgettest.h"

#include "configuredialog/configurespeechtotextconfigurewidget.h"
#include <QCheckBox>
#include <QStandardPaths>
#include <QTest>
#include <QVBoxLayout>
#include <TextSpeechToText/SpeechToTextConfigureWidget>

using namespace Qt::Literals::StringLiterals;

QTEST_MAIN(ConfigureSpeechToTextConfigureWidgetTest)
ConfigureSpeechToTextConfigureWidgetTest::ConfigureSpeechToTextConfigureWidgetTest(QObject *parent)
    : QObject{parent}
{
    QStandardPaths::setTestModeEnabled(true);
}

void ConfigureSpeechToTextConfigureWidgetTest::shouldHaveDefaultValues()
{
    const ConfigureSpeechToTextConfigureWidget w;
    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mEnableSpeechToText = w.findChild<QCheckBox *>(u"mEnableSpeechToText"_s);
    QVERIFY(mEnableSpeechToText);
    QVERIFY(!mEnableSpeechToText->text().isEmpty());
    QVERIFY(!mEnableSpeechToText->isChecked());

    auto mSpeechToTextWidget = w.findChild<TextSpeechToText::SpeechToTextConfigureWidget *>(u"mSpeechToTextWidget"_s);
    QVERIFY(mSpeechToTextWidget);
}

#include "moc_configurespeechtotextconfigurewidgettest.cpp"
