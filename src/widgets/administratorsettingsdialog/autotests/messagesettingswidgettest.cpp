/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagesettingswidgettest.h"
#include "administratorsettingsdialog/message/messagesettingswidget.h"
#include <QCheckBox>
#include <QFormLayout>
#include <QTest>
QTEST_MAIN(MessageSettingsWidgetTest)

MessageSettingsWidgetTest::MessageSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void MessageSettingsWidgetTest::shouldHaveDefaultValues()
{
    MessageSettingsWidget w(nullptr);

    auto mainLayout = w.findChild<QFormLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    auto mAllowMessageEditing = w.findChild<QCheckBox *>(QStringLiteral("mAllowMessageEditing"));
    QVERIFY(mAllowMessageEditing);
    QVERIFY(!mAllowMessageEditing->isChecked());
    QVERIFY(!mAllowMessageEditing->text().isEmpty());
    // TODO
}
