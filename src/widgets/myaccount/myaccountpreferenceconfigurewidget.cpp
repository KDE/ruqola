/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpreferenceconfigurewidget.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "users/userrequestdatadownloadjob.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSeparator>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

MyAccountPreferenceConfigureWidget::MyAccountPreferenceConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mHighlightWords(new QLineEdit(this))
    , mDesktopNotification(new QComboBox(this))
    , mEmailNotification(new QComboBox(this))
    , mPushNotification(new QComboBox(this))
    , mUseEmojis(new QCheckBox(i18n("Use Emojis"), this))
    , mConvertAsciiEmoji(new QCheckBox(i18n("Convert Ascii to Emoji"), this))
    , mHideRoles(new QCheckBox(i18n("Hide roles"), this))
    , mDisplayAvatars(new QCheckBox(i18n("Display avatars"), this))
    , mReceiveLoginDetectionEmails(new QCheckBox(i18n("Receive login detection emails"), this))
    , mIdleTimeLimit(new QSpinBox(this))
    , mAutomaticAway(new QCheckBox(i18n("Enable Auto Away"), this))
    , mEmailNotificationLabel(new QLabel(i18n("Offline Email notification:"), this))
    , mRocketChatAccount(account)
{
    mUseEmojis->setObjectName(QStringLiteral("mUseEmojis"));
    mConvertAsciiEmoji->setObjectName(QStringLiteral("mConvertAsciiEmoji"));
    mHideRoles->setObjectName(QStringLiteral("mHideRoles"));
    mDisplayAvatars->setObjectName(QStringLiteral("mDisplayAvatars"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto highlightWordsLabel = new QLabel(i18n("Highlight words:"), this);
    highlightWordsLabel->setObjectName(QStringLiteral("highlightWordsLabel"));
    highlightWordsLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(highlightWordsLabel);

    mHighlightWords->setObjectName(QStringLiteral("mHighlightWords"));
    KLineEditEventHandler::catchReturnKey(mHighlightWords);
    mHighlightWords->setPlaceholderText(i18n("Use \',\' for separating words"));
    mHighlightWords->setToolTip(i18n("Separate each word with \',\'."));
    connect(mHighlightWords, &QLineEdit::textEdited, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    mainLayout->addWidget(mHighlightWords);

    mDesktopNotification->setObjectName(QStringLiteral("mDesktopNotification"));
    mEmailNotification->setObjectName(QStringLiteral("mEmailNotification"));
    mPushNotification->setObjectName(QStringLiteral("mPushNotification"));

    auto desktopNotificationLabel = new QLabel(i18n("Desktop notification:"), this);
    desktopNotificationLabel->setObjectName(QStringLiteral("desktopNotificationLabel"));
    desktopNotificationLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(desktopNotificationLabel);

    mainLayout->addWidget(mDesktopNotification);

    mEmailNotificationLabel->setObjectName(QStringLiteral("emailNotificationLabel"));
    mEmailNotificationLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(mEmailNotificationLabel);

    mainLayout->addWidget(mEmailNotification);

    auto pushNotificationLabel = new QLabel(i18n("Push notification:"), this);
    pushNotificationLabel->setObjectName(QStringLiteral("pushNotificationLabel"));
    pushNotificationLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(pushNotificationLabel);

    mainLayout->addWidget(mPushNotification);

    mReceiveLoginDetectionEmails->setObjectName(QStringLiteral("mReceiveLoginDetectionEmails"));
    mReceiveLoginDetectionEmails->setToolTip(i18n("Receive an email each time a new login is detected on your account."));
    connect(mReceiveLoginDetectionEmails, &QCheckBox::clicked, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    mainLayout->addWidget(mReceiveLoginDetectionEmails);

    mainLayout->addWidget(mUseEmojis);
    connect(mUseEmojis, &QCheckBox::clicked, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    mainLayout->addWidget(mConvertAsciiEmoji);
    connect(mConvertAsciiEmoji, &QCheckBox::clicked, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    mainLayout->addWidget(mHideRoles);
    connect(mHideRoles, &QCheckBox::clicked, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    mainLayout->addWidget(mDisplayAvatars);
    connect(mDisplayAvatars, &QCheckBox::clicked, this, &MyAccountPreferenceConfigureWidget::setWasChanged);

    mainLayout->addWidget(new KSeparator(this));

    mAutomaticAway->setObjectName(QStringLiteral("mAutomaticAways"));
    connect(mAutomaticAway, &QCheckBox::clicked, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    mainLayout->addWidget(mAutomaticAway);

    auto idleTimeLimitLabel = new QLabel(i18n("Idle Time Limit:"), this);
    idleTimeLimitLabel->setObjectName(QStringLiteral("idleTimeLimitLabel"));
    idleTimeLimitLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(idleTimeLimitLabel);

    mIdleTimeLimit->setObjectName(QStringLiteral("mIdleTimeLimit"));
    mIdleTimeLimit->setMaximum(9999);
    mIdleTimeLimit->setToolTip(i18n("Period of time until status changes to away. Value needs to be in seconds."));
    connect(mIdleTimeLimit, &QSpinBox::valueChanged, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    mainLayout->addWidget(mIdleTimeLimit);

    QWidget *downloadWidget = new QWidget;
    downloadWidget->setObjectName(QStringLiteral("downloadWidget"));
    auto downloadWidgetLayout = new QVBoxLayout(downloadWidget);
    downloadWidgetLayout->setContentsMargins({});

    downloadWidgetLayout->addWidget(new KSeparator(this));

    auto downloadLayout = new QHBoxLayout;
    downloadLayout->setObjectName(QStringLiteral("downloadLayout"));
    downloadLayout->setContentsMargins({});

    downloadWidgetLayout->addLayout(downloadLayout);

    auto downloadDataButton = new QPushButton(i18n("Download my Data (HTML)"), this);
    downloadDataButton->setObjectName(QStringLiteral("downloadDataButton"));
    downloadLayout->addWidget(downloadDataButton);
    connect(downloadDataButton, &QPushButton::clicked, this, [this]() {
        downloadData(false);
    });

    auto exportDataButton = new QPushButton(i18n("Export my Data (JSON)"), this);
    exportDataButton->setObjectName(QStringLiteral("exportDataButton"));
    downloadLayout->addWidget(exportDataButton);
    connect(exportDataButton, &QPushButton::clicked, this, [this]() {
        downloadData(true);
    });

    mainLayout->addWidget(downloadWidget);

    mainLayout->addStretch();
    if (mRocketChatAccount && !mRocketChatAccount->ruqolaServerConfig()->userDataDownloadEnabled()) {
        downloadWidget->setVisible(false);
    }
    initComboboxValues();
    if (mRocketChatAccount) {
        if (!mRocketChatAccount->ruqolaServerConfig()->hasAtLeastVersion(5, 4, 0)) {
            mReceiveLoginDetectionEmails->setVisible(false);
        } else if (!mRocketChatAccount->ruqolaServerConfig()->deviceManagementEnableLoginEmails()
                   || !mRocketChatAccount->ruqolaServerConfig()->deviceManagementAllowLoginEmailpreference()) {
            mReceiveLoginDetectionEmails->setVisible(false);
        }
        connect(mRocketChatAccount, &RocketChatAccount::ownUserPreferencesChanged, this, &MyAccountPreferenceConfigureWidget::load);
    }
}

MyAccountPreferenceConfigureWidget::~MyAccountPreferenceConfigureWidget() = default;

void MyAccountPreferenceConfigureWidget::downloadData(bool fullData)
{
    auto job = new RocketChatRestApi::UserRequestDataDownloadJob(this);
    job->setFullExport(fullData);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job,
            &RocketChatRestApi::UserRequestDataDownloadJob::userRequestDataDownloadDone,
            this,
            &MyAccountPreferenceConfigureWidget::slotUserRequestDataDownloadDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start UserRequestDataDownloadJob job";
    }
}

void MyAccountPreferenceConfigureWidget::slotUserRequestDataDownloadDone()
{
    KMessageBox::information(this,
                             i18n("Your data file will be generated. A link to download it will be sent to your email address when ready."),
                             i18n("Download File Requested"));
}

QString MyAccountPreferenceConfigureWidget::desktopPushNotificationI18n(const QString &value) const
{
    if (value == QLatin1String("all")) {
        return i18n("All Messages");
    } else if (value == QLatin1String("mentions")) {
        return i18n("Mentions");
    } else if (value == QLatin1String("nothing")) {
        return i18n("Nothing");
    }
    qCWarning(RUQOLAWIDGETS_LOG) << "String not found: " << value << " It's a bug";
    return value;
}

void MyAccountPreferenceConfigureWidget::initComboboxValues()
{
    const QString desktopNotificationDefaultValue = mRocketChatAccount
        ? i18n("Default (%1)", desktopPushNotificationI18n(mRocketChatAccount->ruqolaServerConfig()->accountsDefaultUserPreferencesDesktopNotifications()))
        : i18n("Default");
    mDesktopNotification->addItem(desktopNotificationDefaultValue, QStringLiteral("default"));
    mDesktopNotification->addItem(desktopPushNotificationI18n(QStringLiteral("all")), QStringLiteral("all"));
    mDesktopNotification->addItem(desktopPushNotificationI18n(QStringLiteral("mentions")), QStringLiteral("mentions"));
    mDesktopNotification->addItem(desktopPushNotificationI18n(QStringLiteral("nothing")), QStringLiteral("nothing"));

    const QString pushNotificationDefaultValue = mRocketChatAccount
        ? i18n("Default (%1)", desktopPushNotificationI18n(mRocketChatAccount->ruqolaServerConfig()->accountsDefaultUserPreferencesPushNotifications()))
        : i18n("Default");
    mPushNotification->addItem(pushNotificationDefaultValue, QStringLiteral("default"));
    mPushNotification->addItem(desktopPushNotificationI18n(QStringLiteral("all")), QStringLiteral("all"));
    mPushNotification->addItem(desktopPushNotificationI18n(QStringLiteral("mentions")), QStringLiteral("mentions"));
    mPushNotification->addItem(desktopPushNotificationI18n(QStringLiteral("nothing")), QStringLiteral("nothing"));

    // TODO default value from server
    mEmailNotification->addItem(i18n("Default"), QStringLiteral("default"));
    mEmailNotification->addItem(i18n("Each Mentions"), QStringLiteral("mentions"));
    mEmailNotification->addItem(i18n("Disabled"), QStringLiteral("nothing"));
    if (mRocketChatAccount && !mRocketChatAccount->ruqolaServerConfig()->allowEmailNotifications()) {
        mEmailNotification->setEnabled(false);
        mEmailNotificationLabel->setText(i18n("Your Rocket.Chat administrator has disabled email notifications"));
    }

    connect(mDesktopNotification, &QComboBox::activated, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    connect(mPushNotification, &QComboBox::activated, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    connect(mEmailNotification, &QComboBox::activated, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
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
        info.useEmoji = RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToState(mUseEmojis->isChecked());
        info.hideRoles = RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToState(mHideRoles->isChecked());
        info.displayAvatars = RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToState(mDisplayAvatars->isChecked());
        info.convertAsciiToEmoji = RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToState(mConvertAsciiEmoji->isChecked());
        info.idleTimeLimit = mIdleTimeLimit->value();
        info.enableAutoAway = RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToState(mAutomaticAway->isChecked());
        if (mRocketChatAccount) {
            if (mRocketChatAccount->ruqolaServerConfig()->hasAtLeastVersion(5, 4, 0)) {
                if (mRocketChatAccount->ruqolaServerConfig()->deviceManagementEnableLoginEmails()
                    && mRocketChatAccount->ruqolaServerConfig()->deviceManagementAllowLoginEmailpreference()) {
                    info.receiveLoginDetectionEmail =
                        RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToState(mReceiveLoginDetectionEmails->isChecked());
                }
            }
        }
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
    mUseEmojis->setChecked(ownUserPreferences.useEmojis());
    mHideRoles->setChecked(ownUserPreferences.hideRoles());
    mDisplayAvatars->setChecked(ownUserPreferences.displayAvatars());
    mConvertAsciiEmoji->setChecked(ownUserPreferences.convertAsciiEmoji());
    mReceiveLoginDetectionEmails->setChecked(ownUserPreferences.receiveLoginDetectionEmail());
    mIdleTimeLimit->setValue(ownUserPreferences.idleTimeLimit());
    mAutomaticAway->setChecked(ownUserPreferences.enableAutoAway());
    mChanged = false;
}

void MyAccountPreferenceConfigureWidget::setWasChanged()
{
    mChanged = true;
}

#include "moc_myaccountpreferenceconfigurewidget.cpp"
