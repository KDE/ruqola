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

    auto mAllowMessageDeleting = w.findChild<QCheckBox *>(QStringLiteral("mAllowMessageDeleting"));
    QVERIFY(mAllowMessageDeleting);
    QVERIFY(!mAllowMessageDeleting->isChecked());
    QVERIFY(!mAllowMessageDeleting->text().isEmpty());

    auto mShowEditedStatus = w.findChild<QCheckBox *>(QStringLiteral("mShowEditedStatus"));
    QVERIFY(mShowEditedStatus);
    QVERIFY(!mShowEditedStatus->isChecked());
    QVERIFY(!mShowEditedStatus->text().isEmpty());

    auto mShowDeletedStatus = w.findChild<QCheckBox *>(QStringLiteral("mShowDeletedStatus"));
    QVERIFY(mShowDeletedStatus);
    QVERIFY(!mShowDeletedStatus->isChecked());
    QVERIFY(!mShowDeletedStatus->text().isEmpty());

    auto mAllowMessagePinning = w.findChild<QCheckBox *>(QStringLiteral("mAllowMessagePinning"));
    QVERIFY(mAllowMessagePinning);
    QVERIFY(!mAllowMessagePinning->isChecked());
    QVERIFY(!mAllowMessagePinning->text().isEmpty());

    auto mAllowMessageSnippeting = w.findChild<QCheckBox *>(QStringLiteral("mAllowMessageSnippeting"));
    QVERIFY(mAllowMessageSnippeting);
    QVERIFY(!mAllowMessageSnippeting->isChecked());
    QVERIFY(!mAllowMessageSnippeting->text().isEmpty());
}
