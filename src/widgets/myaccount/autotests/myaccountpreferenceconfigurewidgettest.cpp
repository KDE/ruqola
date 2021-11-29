/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "myaccountpreferenceconfigurewidgettest.h"
#include "myaccount/myaccountpreferenceconfigurewidget.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(MyAccountPreferenceConfigureWidgetTest)
MyAccountPreferenceConfigureWidgetTest::MyAccountPreferenceConfigureWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void MyAccountPreferenceConfigureWidgetTest::shouldHaveDefaultValues()
{
    MyAccountPreferenceConfigureWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(QStringLiteral("mainLayout"));
    QVERIFY(mainLayout);

    auto highlightWordsLabel = w.findChild<QLabel *>(QStringLiteral("highlightWordsLabel"));
    QVERIFY(highlightWordsLabel);
    QCOMPARE(highlightWordsLabel->textFormat(), Qt::PlainText);
    QVERIFY(!highlightWordsLabel->text().isEmpty());

    auto mHighlightWords = w.findChild<QLineEdit *>(QStringLiteral("mHighlightWords"));
    QVERIFY(mHighlightWords);
    QVERIFY(mHighlightWords->text().isEmpty());

    auto mDesktopNotification = w.findChild<QComboBox *>(QStringLiteral("mDesktopNotification"));
    QVERIFY(mDesktopNotification);
    auto mEmailNotification = w.findChild<QComboBox *>(QStringLiteral("mEmailNotification"));
    QVERIFY(mEmailNotification);
    auto mMobileNotification = w.findChild<QComboBox *>(QStringLiteral("mMobileNotification"));
    QVERIFY(mMobileNotification);

    auto desktopNotificationLabel = w.findChild<QLabel *>(QStringLiteral("desktopNotificationLabel"));
    QVERIFY(desktopNotificationLabel);
    QCOMPARE(desktopNotificationLabel->textFormat(), Qt::PlainText);
    QVERIFY(!desktopNotificationLabel->text().isEmpty());

    auto emailNotificationLabel = w.findChild<QLabel *>(QStringLiteral("emailNotificationLabel"));
    QVERIFY(emailNotificationLabel);
    QCOMPARE(emailNotificationLabel->textFormat(), Qt::PlainText);
    QVERIFY(!emailNotificationLabel->text().isEmpty());

    auto mobileNotificationLabel = w.findChild<QLabel *>(QStringLiteral("mobileNotificationLabel"));
    QVERIFY(mobileNotificationLabel);
    QCOMPARE(mobileNotificationLabel->textFormat(), Qt::PlainText);
    QVERIFY(!mobileNotificationLabel->text().isEmpty());

    auto mUseEmoji = w.findChild<QCheckBox *>(QStringLiteral("mUseEmoji"));
    QVERIFY(mUseEmoji);
    QVERIFY(!mUseEmoji->isChecked()); // False by default as we didn't load values yet
    QVERIFY(!mUseEmoji->text().isEmpty());

    auto mConvertAsciiEmoji = w.findChild<QCheckBox *>(QStringLiteral("mConvertAsciiEmoji"));
    QVERIFY(mConvertAsciiEmoji);
    QVERIFY(!mConvertAsciiEmoji->isChecked()); // False by default as we didn't load values yet
    QVERIFY(!mConvertAsciiEmoji->text().isEmpty());

    auto mHideRoles = w.findChild<QCheckBox *>(QStringLiteral("mHideRoles"));
    QVERIFY(mHideRoles);
    QVERIFY(!mHideRoles->isChecked());
    QVERIFY(!mHideRoles->text().isEmpty());

    auto mhideAvatars = w.findChild<QCheckBox *>(QStringLiteral("mhideAvatars"));
    QVERIFY(mhideAvatars);
    QVERIFY(!mhideAvatars->isChecked());
    QVERIFY(!mhideAvatars->text().isEmpty());
}
