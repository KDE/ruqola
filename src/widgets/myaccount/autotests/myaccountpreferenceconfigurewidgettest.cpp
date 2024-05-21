/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpreferenceconfigurewidgettest.h"
#include "misc/soundconfigurewidget.h"
#include "myaccount/myaccountpreferenceconfigurewidget.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
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
    auto mPushNotification = w.findChild<QComboBox *>(QStringLiteral("mPushNotification"));
    QVERIFY(mPushNotification);

    auto desktopNotificationLabel = w.findChild<QLabel *>(QStringLiteral("desktopNotificationLabel"));
    QVERIFY(desktopNotificationLabel);
    QCOMPARE(desktopNotificationLabel->textFormat(), Qt::PlainText);
    QVERIFY(!desktopNotificationLabel->text().isEmpty());

    auto emailNotificationLabel = w.findChild<QLabel *>(QStringLiteral("emailNotificationLabel"));
    QVERIFY(emailNotificationLabel);
    QCOMPARE(emailNotificationLabel->textFormat(), Qt::PlainText);
    QVERIFY(!emailNotificationLabel->text().isEmpty());

    auto pushNotificationLabel = w.findChild<QLabel *>(QStringLiteral("pushNotificationLabel"));
    QVERIFY(pushNotificationLabel);
    QCOMPARE(pushNotificationLabel->textFormat(), Qt::PlainText);
    QVERIFY(!pushNotificationLabel->text().isEmpty());

    auto mUseEmojis = w.findChild<QCheckBox *>(QStringLiteral("mUseEmojis"));
    QVERIFY(mUseEmojis);
    QVERIFY(!mUseEmojis->isChecked()); // False by default as we didn't load values yet
    QVERIFY(!mUseEmojis->text().isEmpty());

    auto mConvertAsciiEmoji = w.findChild<QCheckBox *>(QStringLiteral("mConvertAsciiEmoji"));
    QVERIFY(mConvertAsciiEmoji);
    QVERIFY(!mConvertAsciiEmoji->isChecked()); // False by default as we didn't load values yet
    QVERIFY(!mConvertAsciiEmoji->text().isEmpty());

    auto mHideRoles = w.findChild<QCheckBox *>(QStringLiteral("mHideRoles"));
    QVERIFY(mHideRoles);
    QVERIFY(!mHideRoles->isChecked());
    QVERIFY(!mHideRoles->text().isEmpty());

    auto mDisplayAvatars = w.findChild<QCheckBox *>(QStringLiteral("mDisplayAvatars"));
    QVERIFY(mDisplayAvatars);
    QVERIFY(!mDisplayAvatars->isChecked());
    QVERIFY(!mDisplayAvatars->text().isEmpty());

    auto downloadLayout = w.findChild<QHBoxLayout *>(QStringLiteral("downloadLayout"));
    QVERIFY(downloadLayout);
    QCOMPARE(downloadLayout->contentsMargins(), QMargins{});

    auto downloadDataButton = w.findChild<QPushButton *>(QStringLiteral("downloadDataButton"));
    QVERIFY(downloadDataButton);
    QVERIFY(!downloadDataButton->text().isEmpty());

    auto exportDataButton = w.findChild<QPushButton *>(QStringLiteral("exportDataButton"));
    QVERIFY(exportDataButton);
    QVERIFY(!exportDataButton->text().isEmpty());

    auto mReceiveLoginDetectionEmails = w.findChild<QCheckBox *>(QStringLiteral("mReceiveLoginDetectionEmails"));
    QVERIFY(mReceiveLoginDetectionEmails);
    QVERIFY(!mReceiveLoginDetectionEmails->isChecked()); // False by default as we didn't load values yet
    QVERIFY(!mReceiveLoginDetectionEmails->text().isEmpty());
    QVERIFY(!mReceiveLoginDetectionEmails->toolTip().isEmpty());

    auto mAutomaticAways = w.findChild<QCheckBox *>(QStringLiteral("mAutomaticAways"));
    QVERIFY(mAutomaticAways);
    QVERIFY(!mAutomaticAways->isChecked()); // False by default as we didn't load values yet
    QVERIFY(!mAutomaticAways->text().isEmpty());

    auto idleTimeLimitLabel = w.findChild<QLabel *>(QStringLiteral("idleTimeLimitLabel"));
    QVERIFY(idleTimeLimitLabel);
    QCOMPARE(idleTimeLimitLabel->textFormat(), Qt::PlainText);
    QVERIFY(!idleTimeLimitLabel->text().isEmpty());

    auto mIdleTimeLimit = w.findChild<QSpinBox *>(QStringLiteral("mIdleTimeLimit"));
    QVERIFY(mIdleTimeLimit);
    QVERIFY(!mIdleTimeLimit->toolTip().isEmpty());

    auto downloadWidget = w.findChild<QWidget *>(QStringLiteral("downloadWidget"));
    QVERIFY(downloadWidget);

    auto downloadWidgetLayout = downloadWidget->findChild<QVBoxLayout *>(QStringLiteral("downloadWidgetLayout"));
    QVERIFY(downloadWidgetLayout);
    QCOMPARE(downloadWidgetLayout->contentsMargins(), QMargins{});

    auto soundWidget = w.findChild<QWidget *>(QStringLiteral("soundWidget"));
    QVERIFY(soundWidget);

    auto soundWidgetLayout = soundWidget->findChild<QVBoxLayout *>(QStringLiteral("soundWidgetLayout"));
    QVERIFY(soundWidgetLayout);
    QCOMPARE(soundWidgetLayout->contentsMargins(), QMargins{});

    auto mSoundNewRoomNotification = soundWidget->findChild<SoundConfigureWidget *>(QStringLiteral("mSoundNewRoomNotification"));
    QVERIFY(mSoundNewRoomNotification);

    auto mSoundNewMessageNotification = soundWidget->findChild<SoundConfigureWidget *>(QStringLiteral("mSoundNewMessageNotification"));
    QVERIFY(mSoundNewMessageNotification);

    auto mMuteFocusedConversations = w.findChild<QCheckBox *>(QStringLiteral("mMuteFocusedConversations"));
    QVERIFY(mMuteFocusedConversations);
    QVERIFY(!mMuteFocusedConversations->isChecked()); // False by default as we didn't load values yet
    QVERIFY(!mMuteFocusedConversations->text().isEmpty());

    auto mNotificationsSoundVolume = soundWidget->findChild<QSpinBox *>(QStringLiteral("mNotificationsSoundVolume"));
    QVERIFY(mNotificationsSoundVolume);

    auto notificationsSoundVolumeLabel = soundWidget->findChild<QLabel *>(QStringLiteral("notificationsSoundVolumeLabel"));
    QVERIFY(notificationsSoundVolumeLabel);
    QCOMPARE(notificationsSoundVolumeLabel->textFormat(), Qt::PlainText);
    QVERIFY(!notificationsSoundVolumeLabel->text().isEmpty());
}

#include "moc_myaccountpreferenceconfigurewidgettest.cpp"
