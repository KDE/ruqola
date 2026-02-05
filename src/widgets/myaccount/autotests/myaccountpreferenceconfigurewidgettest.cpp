/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

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
using namespace Qt::Literals::StringLiterals;
MyAccountPreferenceConfigureWidgetTest::MyAccountPreferenceConfigureWidgetTest(QObject *parent)
    : QObject(parent)
{
}

void MyAccountPreferenceConfigureWidgetTest::shouldHaveDefaultValues()
{
    MyAccountPreferenceConfigureWidget w(nullptr);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto highlightWordsLabel = w.findChild<QLabel *>(u"highlightWordsLabel"_s);
    QVERIFY(highlightWordsLabel);
    QCOMPARE(highlightWordsLabel->textFormat(), Qt::PlainText);
    QVERIFY(!highlightWordsLabel->text().isEmpty());

    auto mHighlightWords = w.findChild<QLineEdit *>(u"mHighlightWords"_s);
    QVERIFY(mHighlightWords);
    QVERIFY(mHighlightWords->text().isEmpty());

    auto mDesktopNotification = w.findChild<QComboBox *>(u"mDesktopNotification"_s);
    QVERIFY(mDesktopNotification);
    auto mEmailNotification = w.findChild<QComboBox *>(u"mEmailNotification"_s);
    QVERIFY(mEmailNotification);
    auto mPushNotification = w.findChild<QComboBox *>(u"mPushNotification"_s);
    QVERIFY(mPushNotification);

    auto desktopNotificationLabel = w.findChild<QLabel *>(u"desktopNotificationLabel"_s);
    QVERIFY(desktopNotificationLabel);
    QCOMPARE(desktopNotificationLabel->textFormat(), Qt::PlainText);
    QVERIFY(!desktopNotificationLabel->text().isEmpty());

    auto emailNotificationLabel = w.findChild<QLabel *>(u"emailNotificationLabel"_s);
    QVERIFY(emailNotificationLabel);
    QCOMPARE(emailNotificationLabel->textFormat(), Qt::PlainText);
    QVERIFY(!emailNotificationLabel->text().isEmpty());

    auto pushNotificationLabel = w.findChild<QLabel *>(u"pushNotificationLabel"_s);
    QVERIFY(pushNotificationLabel);
    QCOMPARE(pushNotificationLabel->textFormat(), Qt::PlainText);
    QVERIFY(!pushNotificationLabel->text().isEmpty());

    auto mUseEmojis = w.findChild<QCheckBox *>(u"mUseEmojis"_s);
    QVERIFY(mUseEmojis);
    QVERIFY(!mUseEmojis->isChecked()); // False by default as we didn't load values yet
    QVERIFY(!mUseEmojis->text().isEmpty());

    auto mConvertAsciiEmoji = w.findChild<QCheckBox *>(u"mConvertAsciiEmoji"_s);
    QVERIFY(mConvertAsciiEmoji);
    QVERIFY(!mConvertAsciiEmoji->isChecked()); // False by default as we didn't load values yet
    QVERIFY(!mConvertAsciiEmoji->text().isEmpty());

    auto mHideRoles = w.findChild<QCheckBox *>(u"mHideRoles"_s);
    QVERIFY(mHideRoles);
    QVERIFY(!mHideRoles->isChecked());
    QVERIFY(!mHideRoles->text().isEmpty());

    auto mDisplayAvatars = w.findChild<QCheckBox *>(u"mDisplayAvatars"_s);
    QVERIFY(mDisplayAvatars);
    QVERIFY(!mDisplayAvatars->isChecked());
    QVERIFY(!mDisplayAvatars->text().isEmpty());

    auto downloadLayout = w.findChild<QHBoxLayout *>(u"downloadLayout"_s);
    QVERIFY(downloadLayout);
    QCOMPARE(downloadLayout->contentsMargins(), QMargins{});

    auto downloadDataButton = w.findChild<QPushButton *>(u"downloadDataButton"_s);
    QVERIFY(downloadDataButton);
    QVERIFY(!downloadDataButton->text().isEmpty());

    auto exportDataButton = w.findChild<QPushButton *>(u"exportDataButton"_s);
    QVERIFY(exportDataButton);
    QVERIFY(!exportDataButton->text().isEmpty());

    auto mReceiveLoginDetectionEmails = w.findChild<QCheckBox *>(u"mReceiveLoginDetectionEmails"_s);
    QVERIFY(mReceiveLoginDetectionEmails);
    QVERIFY(!mReceiveLoginDetectionEmails->isChecked()); // False by default as we didn't load values yet
    QVERIFY(!mReceiveLoginDetectionEmails->text().isEmpty());
    QVERIFY(!mReceiveLoginDetectionEmails->toolTip().isEmpty());

    auto mAutomaticAways = w.findChild<QCheckBox *>(u"mAutomaticAways"_s);
    QVERIFY(mAutomaticAways);
    QVERIFY(!mAutomaticAways->isChecked()); // False by default as we didn't load values yet
    QVERIFY(!mAutomaticAways->text().isEmpty());

    auto idleTimeLimitLabel = w.findChild<QLabel *>(u"idleTimeLimitLabel"_s);
    QVERIFY(idleTimeLimitLabel);
    QCOMPARE(idleTimeLimitLabel->textFormat(), Qt::PlainText);
    QVERIFY(!idleTimeLimitLabel->text().isEmpty());

    auto mIdleTimeLimit = w.findChild<QSpinBox *>(u"mIdleTimeLimit"_s);
    QVERIFY(mIdleTimeLimit);
    QVERIFY(!mIdleTimeLimit->toolTip().isEmpty());

    auto downloadWidget = w.findChild<QWidget *>(u"downloadWidget"_s);
    QVERIFY(downloadWidget);

    auto downloadWidgetLayout = downloadWidget->findChild<QVBoxLayout *>(u"downloadWidgetLayout"_s);
    QVERIFY(downloadWidgetLayout);
    QCOMPARE(downloadWidgetLayout->contentsMargins(), QMargins{});

    auto soundWidget = w.findChild<QWidget *>(u"soundWidget"_s);
    QVERIFY(soundWidget);

    auto soundWidgetLayout = soundWidget->findChild<QVBoxLayout *>(u"soundWidgetLayout"_s);
    QVERIFY(soundWidgetLayout);
    QCOMPARE(soundWidgetLayout->contentsMargins(), QMargins{});

    auto mSoundNewRoomNotification = soundWidget->findChild<SoundConfigureWidget *>(u"mSoundNewRoomNotification"_s);
    QVERIFY(mSoundNewRoomNotification);

    auto mSoundNewMessageNotification = soundWidget->findChild<SoundConfigureWidget *>(u"mSoundNewMessageNotification"_s);
    QVERIFY(mSoundNewMessageNotification);

    auto mMuteFocusedConversations = w.findChild<QCheckBox *>(u"mMuteFocusedConversations"_s);
    QVERIFY(mMuteFocusedConversations);
    QVERIFY(!mMuteFocusedConversations->isChecked()); // False by default as we didn't load values yet
    QVERIFY(!mMuteFocusedConversations->text().isEmpty());

    auto mNotificationsSoundVolume = soundWidget->findChild<QSpinBox *>(u"mNotificationsSoundVolume"_s);
    QVERIFY(mNotificationsSoundVolume);

    auto notificationsSoundVolumeLabel = soundWidget->findChild<QLabel *>(u"notificationsSoundVolumeLabel"_s);
    QVERIFY(notificationsSoundVolumeLabel);
    QCOMPARE(notificationsSoundVolumeLabel->textFormat(), Qt::PlainText);
    QVERIFY(!notificationsSoundVolumeLabel->text().isEmpty());

    auto mCallRingerVolume = soundWidget->findChild<QSpinBox *>(u"mCallRingerVolume"_s);
    QVERIFY(mCallRingerVolume);

    auto callRingerVolumeLabel = soundWidget->findChild<QLabel *>(u"callRingerVolumeLabel"_s);
    QVERIFY(callRingerVolumeLabel);
    QCOMPARE(callRingerVolumeLabel->textFormat(), Qt::PlainText);
    QVERIFY(!callRingerVolumeLabel->text().isEmpty());

    auto mMasterVolume = soundWidget->findChild<QSpinBox *>(u"mMasterVolume"_s);
    QVERIFY(mMasterVolume);

    auto masterVolumeLabel = soundWidget->findChild<QLabel *>(u"masterVolumeLabel"_s);
    QVERIFY(masterVolumeLabel);
    QCOMPARE(masterVolumeLabel->textFormat(), Qt::PlainText);
    QVERIFY(!masterVolumeLabel->text().isEmpty());
}

#include "moc_myaccountpreferenceconfigurewidgettest.cpp"
