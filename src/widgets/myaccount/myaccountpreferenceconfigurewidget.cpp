/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

MyAccountPreferenceConfigureWidget::MyAccountPreferenceConfigureWidget(QWidget *parent)
    : QWidget(parent)
    , mHighlightWords(new QLineEdit(this))
    , mDesktopNotification(new QComboBox(this))
    , mEmailNotification(new QComboBox(this))
    , mMobileNotification(new QComboBox(this))
    , mUseEmoji(new QCheckBox(i18n("Use Emoji"), this))
    , mConvertAsciiEmoji(new QCheckBox(i18n("Convert Ascii to Emoji"), this))
    , mHideRoles(new QCheckBox(i18n("Hide Roles"), this))
    , mhideAvatars(new QCheckBox(i18n("Hide Avatars"), this))
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
    mMobileNotification->setObjectName(QStringLiteral("mMobileNotification"));

    auto desktopNotificationLabel = new QLabel(i18n("Desktop Notification:"), this);
    desktopNotificationLabel->setObjectName(QStringLiteral("desktopNotificationLabel"));
    desktopNotificationLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(desktopNotificationLabel);

    mainLayout->addWidget(mDesktopNotification);

    auto emailNotificationLabel = new QLabel(i18n("Email Notification:"), this);
    emailNotificationLabel->setObjectName(QStringLiteral("emailNotificationLabel"));
    emailNotificationLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(emailNotificationLabel);

    mainLayout->addWidget(mEmailNotification);

    auto mobileNotificationLabel = new QLabel(i18n("Mobile Notification:"), this);
    mobileNotificationLabel->setObjectName(QStringLiteral("mobileNotificationLabel"));
    mobileNotificationLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(mobileNotificationLabel);

    mainLayout->addWidget(mMobileNotification);
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

MyAccountPreferenceConfigureWidget::~MyAccountPreferenceConfigureWidget()
{
}

void MyAccountPreferenceConfigureWidget::initComboboxValues()
{
    // Default ?
    mDesktopNotification->addItem(i18n("All Messages"), QStringLiteral("all"));
    mDesktopNotification->addItem(i18n("Mentions"), QStringLiteral("mentions"));
    mDesktopNotification->addItem(i18n("Nothing"), QStringLiteral("nothing"));

    // Default ?
    mMobileNotification->addItem(i18n("All Messages"), QStringLiteral("all"));
    mMobileNotification->addItem(i18n("Mentions"), QStringLiteral("mentions"));
    mMobileNotification->addItem(i18n("Nothing"), QStringLiteral("nothing"));

    // Default ?
    mEmailNotification->addItem(i18n("Each Mentions"), QStringLiteral("mentions"));
    mEmailNotification->addItem(i18n("Disabled"), QStringLiteral("nothing"));

    connect(mDesktopNotification, QOverload<int>::of(&QComboBox::activated), this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    connect(mMobileNotification, QOverload<int>::of(&QComboBox::activated), this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    connect(mEmailNotification, QOverload<int>::of(&QComboBox::activated), this, &MyAccountPreferenceConfigureWidget::setWasChanged);
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
        info.mobileNotifications = mMobileNotification->currentData().toString();
        info.desktopNotifications = mDesktopNotification->currentData().toString();
        info.emailNotificationMode = mEmailNotification->currentData().toString();
        info.userId = Ruqola::self()->rocketChatAccount()->userId();
        info.useEmoji = mUseEmoji->isChecked();
        info.hideRoles = mHideRoles->isChecked();
        info.hideAvatars = mhideAvatars->isChecked();
        info.convertAsciiToEmoji = mConvertAsciiEmoji->isChecked();
        Ruqola::self()->rocketChatAccount()->setUserPreferences(info);
    }
}

void MyAccountPreferenceConfigureWidget::load()
{
    const OwnUserPreferences ownUserPreferences = Ruqola::self()->rocketChatAccount()->ownUserPreferences();
    mHighlightWords->setText(ownUserPreferences.highlightWords().join(QLatin1Char(',')));
    mMobileNotification->setCurrentIndex(mMobileNotification->findData(ownUserPreferences.mobileNotifications()));
    mEmailNotification->setCurrentIndex(mEmailNotification->findData(ownUserPreferences.emailNotificationMode()));
    mDesktopNotification->setCurrentIndex(mDesktopNotification->findData(ownUserPreferences.desktopNotifications()));
    mUseEmoji->setChecked(ownUserPreferences.useEmojis());
    mHideRoles->setChecked(ownUserPreferences.hideRoles());
    mhideAvatars->setChecked(ownUserPreferences.hideAvatars());
    mConvertAsciiEmoji->setChecked(ownUserPreferences.convertAsciiEmoji());
    mChanged = false;
}

void MyAccountPreferenceConfigureWidget::setWasChanged()
{
    mChanged = true;
}
