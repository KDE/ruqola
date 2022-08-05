/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpreferenceconfigurewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

MyAccountPreferenceConfigureWidget::MyAccountPreferenceConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mHighlightWords(new QLineEdit(this))
    , mDesktopNotification(new QComboBox(this))
    , mEmailNotification(new QComboBox(this))
    , mPushNotification(new QComboBox(this))
    , mViewMode(new QComboBox(this))
    , mUseEmoji(new QCheckBox(i18n("Use Emoji"), this))
    , mConvertAsciiEmoji(new QCheckBox(i18n("Convert Ascii to Emoji"), this))
    , mHideRoles(new QCheckBox(i18n("Hide Roles"), this))
    , mhideAvatars(new QCheckBox(i18n("Hide Avatars"), this))
    , mRocketChatAccount(account)
{
    mUseEmoji->setObjectName(QStringLiteral("mUseEmoji"));
    mConvertAsciiEmoji->setObjectName(QStringLiteral("mConvertAsciiEmoji"));
    mHideRoles->setObjectName(QStringLiteral("mHideRoles"));
    mhideAvatars->setObjectName(QStringLiteral("mhideAvatars"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto highlightWordsLabel = new QLabel(i18n("Highlight words:"), this);
    highlightWordsLabel->setObjectName(QStringLiteral("highlightWordsLabel"));
    highlightWordsLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(highlightWordsLabel);

    mHighlightWords->setObjectName(QStringLiteral("mHighlightWords"));
    new LineEditCatchReturnKey(mHighlightWords, this);
    mHighlightWords->setPlaceholderText(i18n("Use \',\' for separating words"));
    mHighlightWords->setToolTip(i18n("Separate each word with \',\'."));
    connect(mHighlightWords, &QLineEdit::textEdited, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    mainLayout->addWidget(mHighlightWords);

    mDesktopNotification->setObjectName(QStringLiteral("mDesktopNotification"));
    mEmailNotification->setObjectName(QStringLiteral("mEmailNotification"));
    mPushNotification->setObjectName(QStringLiteral("mPushNotification"));
    mViewMode->setObjectName(QStringLiteral("mViewMode"));

    auto desktopNotificationLabel = new QLabel(i18n("Desktop Notification:"), this);
    desktopNotificationLabel->setObjectName(QStringLiteral("desktopNotificationLabel"));
    desktopNotificationLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(desktopNotificationLabel);

    mainLayout->addWidget(mDesktopNotification);

    auto emailNotificationLabel = new QLabel(i18n("Offline Email Notification:"), this);
    emailNotificationLabel->setObjectName(QStringLiteral("emailNotificationLabel"));
    emailNotificationLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(emailNotificationLabel);

    mainLayout->addWidget(mEmailNotification);

    auto pushNotificationLabel = new QLabel(i18n("Push Notification:"), this);
    pushNotificationLabel->setObjectName(QStringLiteral("pushNotificationLabel"));
    pushNotificationLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(pushNotificationLabel);

    mainLayout->addWidget(mPushNotification);

    auto viewModeLabel = new QLabel(i18n("View Mode:"), this);
    viewModeLabel->setObjectName(QStringLiteral("viewModeLabel"));
    viewModeLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(viewModeLabel);

    mainLayout->addWidget(mViewMode);

    mainLayout->addWidget(mUseEmoji);
    connect(mUseEmoji, &QCheckBox::clicked, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    mainLayout->addWidget(mConvertAsciiEmoji);
    connect(mConvertAsciiEmoji, &QCheckBox::clicked, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    mainLayout->addWidget(mHideRoles);
    connect(mHideRoles, &QCheckBox::clicked, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    mainLayout->addWidget(mhideAvatars);
    connect(mhideAvatars, &QCheckBox::clicked, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    mainLayout->addStretch();
    initComboboxValues();
}

MyAccountPreferenceConfigureWidget::~MyAccountPreferenceConfigureWidget() = default;

void MyAccountPreferenceConfigureWidget::initComboboxValues()
{
    // Default ?
    mDesktopNotification->addItem(i18n("All Messages"), QStringLiteral("all"));
    mDesktopNotification->addItem(i18n("Mentions"), QStringLiteral("mentions"));
    mDesktopNotification->addItem(i18n("Nothing"), QStringLiteral("nothing"));

    // Default ?
    mPushNotification->addItem(i18n("All Messages"), QStringLiteral("all"));
    mPushNotification->addItem(i18n("Mentions"), QStringLiteral("mentions"));
    mPushNotification->addItem(i18n("Nothing"), QStringLiteral("nothing"));

    // Default ?
    mEmailNotification->addItem(i18n("Each Mentions"), QStringLiteral("mentions"));
    mEmailNotification->addItem(i18n("Disabled"), QStringLiteral("nothing"));

    mViewMode->addItem(i18n("Normal"), QStringLiteral("0"));
    mViewMode->addItem(i18n("Cozy"), QStringLiteral("1"));
    mViewMode->addItem(i18n("Compact"), QStringLiteral("2"));

    connect(mDesktopNotification, &QComboBox::activated, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    connect(mPushNotification, &QComboBox::activated, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    connect(mEmailNotification, &QComboBox::activated, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    connect(mViewMode, &QComboBox::activated, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
}

void MyAccountPreferenceConfigureWidget::save()
{
    if (mChanged) {
        const QStringList highlightWords = mHighlightWords->text().split(QLatin1Char(','));
        QStringList listWords;
        for (QString word : highlightWords) {
            word = word.trimmed();
            if (!word.isEmpty()) {
                listWords.append(word);
            }
        }

        RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo info;
        info.highlights = listWords;
        info.pushNotifications = mPushNotification->currentData().toString();
        info.desktopNotifications = mDesktopNotification->currentData().toString();
        info.emailNotificationMode = mEmailNotification->currentData().toString();
        info.userId = mRocketChatAccount->userId();
        info.useEmoji = mUseEmoji->isChecked();
        info.hideRoles = mHideRoles->isChecked();
        info.hideAvatars = mhideAvatars->isChecked();
        info.convertAsciiToEmoji = mConvertAsciiEmoji->isChecked();
        mRocketChatAccount->setUserPreferences(info);
    }
}

void MyAccountPreferenceConfigureWidget::load()
{
    const OwnUserPreferences ownUserPreferences = mRocketChatAccount->ownUserPreferences();
    mHighlightWords->setText(ownUserPreferences.highlightWords().join(QLatin1Char(',')));
    mPushNotification->setCurrentIndex(mPushNotification->findData(ownUserPreferences.pushNotifications()));
    mEmailNotification->setCurrentIndex(mEmailNotification->findData(ownUserPreferences.emailNotificationMode()));
    mDesktopNotification->setCurrentIndex(mDesktopNotification->findData(ownUserPreferences.desktopNotifications()));
    mUseEmoji->setChecked(ownUserPreferences.useEmojis());
    mHideRoles->setChecked(ownUserPreferences.hideRoles());
    mhideAvatars->setChecked(ownUserPreferences.hideAvatars());
    mConvertAsciiEmoji->setChecked(ownUserPreferences.convertAsciiEmoji());
    mViewMode->setCurrentIndex(mViewMode->findData(mRocketChatAccount->viewMode()));
    mChanged = false;
}

void MyAccountPreferenceConfigureWidget::setWasChanged()
{
    mChanged = true;
}
