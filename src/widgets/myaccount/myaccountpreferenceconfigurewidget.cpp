/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpreferenceconfigurewidget.h"
#include "misc/soundconfigurewidget.h"
#include "model/notificationdesktopsoundpreferencemodel.h"

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

using namespace Qt::Literals::StringLiterals;
MyAccountPreferenceConfigureWidget::MyAccountPreferenceConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mHighlightWords(new QLineEdit(this))
    , mDesktopNotification(new QComboBox(this))
    , mEmailNotification(new QComboBox(this))
    , mPushNotification(new QComboBox(this))
    , mReceiveLoginDetectionEmails(new QCheckBox(i18nc("@option:check", "Receive login detection emails"), this))
    , mUseEmojis(new QCheckBox(i18nc("@option:check", "Use Emojis"), this))
    , mConvertAsciiEmoji(new QCheckBox(i18nc("@option:check", "Convert Ascii to Emoji"), this))
    , mHideRoles(new QCheckBox(i18nc("@option:check", "Hide roles"), this))
    , mDisplayAvatars(new QCheckBox(i18nc("@option:check", "Display avatars"), this))
    , mAutomaticAway(new QCheckBox(i18nc("@option:check", "Enable Auto Away"), this))
    , mIdleTimeLimit(new QSpinBox(this))
    , mEmailNotificationLabel(new QLabel(i18nc("@label:textbox", "Offline Email notification:"), this))
    , mSoundNewRoomNotification(new SoundConfigureWidget(account, this))
    , mSoundNewMessageNotification(new SoundConfigureWidget(account, this))
    , mSoundModel(new NotificationDesktopSoundPreferenceModel(this))
    , mMuteFocusedConversations(new QCheckBox(i18nc("@option:check", "Mute Focused Conversations"), this))
    , mNotificationsSoundVolume(new QSpinBox(this))
    , mRocketChatAccount(account)
{
    mUseEmojis->setObjectName(QStringLiteral("mUseEmojis"));
    mConvertAsciiEmoji->setObjectName(QStringLiteral("mConvertAsciiEmoji"));
    mHideRoles->setObjectName(QStringLiteral("mHideRoles"));
    mDisplayAvatars->setObjectName(QStringLiteral("mDisplayAvatars"));
    mSoundNewRoomNotification->setObjectName(QStringLiteral("mSoundNewRoomNotification"));
    mSoundNewMessageNotification->setObjectName(QStringLiteral("mSoundNewMessageNotification"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto highlightWordsLabel = new QLabel(i18nc("@label:textbox", "Highlight words:"), this);
    highlightWordsLabel->setObjectName(QStringLiteral("highlightWordsLabel"));
    highlightWordsLabel->setTextFormat(Qt::PlainText);

    mHighlightWords->setObjectName(QStringLiteral("mHighlightWords"));
    KLineEditEventHandler::catchReturnKey(mHighlightWords);
    mHighlightWords->setPlaceholderText(i18nc("@info:placeholder", "Use \',\' for separating words"));
    mHighlightWords->setToolTip(i18nc("@info:tooltip", "Separate each word with \',\'."));
    connect(mHighlightWords, &QLineEdit::textEdited, this, &MyAccountPreferenceConfigureWidget::setWasChanged);

    createLayout(highlightWordsLabel, mHighlightWords, mainLayout);

    mDesktopNotification->setObjectName(QStringLiteral("mDesktopNotification"));
    mEmailNotification->setObjectName(QStringLiteral("mEmailNotification"));
    mPushNotification->setObjectName(QStringLiteral("mPushNotification"));

    auto desktopNotificationLabel = new QLabel(i18nc("@label:textbox", "Desktop notification:"), this);
    desktopNotificationLabel->setObjectName(QStringLiteral("desktopNotificationLabel"));
    desktopNotificationLabel->setTextFormat(Qt::PlainText);
    mainLayout->addWidget(desktopNotificationLabel);

    mainLayout->addWidget(mDesktopNotification);
    createLayout(desktopNotificationLabel, mDesktopNotification, mainLayout);

    mEmailNotificationLabel->setObjectName(QStringLiteral("emailNotificationLabel"));
    mEmailNotificationLabel->setTextFormat(Qt::PlainText);
    createLayout(mEmailNotificationLabel, mEmailNotification, mainLayout);

    auto pushNotificationLabel = new QLabel(i18nc("@label:textbox", "Push notification:"), this);
    pushNotificationLabel->setObjectName(QStringLiteral("pushNotificationLabel"));
    pushNotificationLabel->setTextFormat(Qt::PlainText);

    createLayout(pushNotificationLabel, mPushNotification, mainLayout);

    mReceiveLoginDetectionEmails->setObjectName(QStringLiteral("mReceiveLoginDetectionEmails"));
    mReceiveLoginDetectionEmails->setToolTip(i18nc("@info:tooltip", "Receive an email each time a new login is detected on your account."));
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

    auto idleTimeLimitLabel = new QLabel(i18nc("@label:textbox", "Idle Time Limit:"), this);
    idleTimeLimitLabel->setObjectName(QStringLiteral("idleTimeLimitLabel"));
    idleTimeLimitLabel->setTextFormat(Qt::PlainText);

    mIdleTimeLimit->setObjectName(QStringLiteral("mIdleTimeLimit"));
    mIdleTimeLimit->setMaximum(9999);
    mIdleTimeLimit->setToolTip(i18nc("@info:tooltip", "Period of time until status changes to away. Value needs to be in seconds."));
    connect(mIdleTimeLimit, &QSpinBox::valueChanged, this, &MyAccountPreferenceConfigureWidget::setWasChanged);

    createLayout(idleTimeLimitLabel, mIdleTimeLimit, mainLayout);

    QWidget *soundWidget = new QWidget(this);
    soundWidget->setObjectName(QStringLiteral("soundWidget"));
    auto soundWidgetLayout = new QVBoxLayout(soundWidget);
    soundWidgetLayout->setObjectName(QStringLiteral("soundWidgetLayout"));
    soundWidgetLayout->setContentsMargins({});

    soundWidgetLayout->addWidget(new KSeparator(this));

    auto newRoomNotificationLabel = new QLabel(i18nc("@label:textbox", "New Room Notification:"), this);
    newRoomNotificationLabel->setObjectName(QStringLiteral("newRoomNotificationLabel"));
    newRoomNotificationLabel->setTextFormat(Qt::PlainText);

    createLayout(newRoomNotificationLabel, mSoundNewRoomNotification, soundWidgetLayout);

    auto newMessageNotificationLabel = new QLabel(i18nc("@label:textbox", "New Message Notification:"), this);
    newMessageNotificationLabel->setObjectName(QStringLiteral("newMessageNotificationLabel"));
    newMessageNotificationLabel->setTextFormat(Qt::PlainText);

    createLayout(newMessageNotificationLabel, mSoundNewMessageNotification, soundWidgetLayout);

    mMuteFocusedConversations->setObjectName(QStringLiteral("mMuteFocusedConversations"));
    soundWidgetLayout->addWidget(mMuteFocusedConversations);
    connect(mMuteFocusedConversations, &QCheckBox::clicked, this, &MyAccountPreferenceConfigureWidget::setWasChanged);

    auto notificationsSoundVolumeLabel = new QLabel(i18nc("@label:textbox", "Notifications sound volume:"), this);
    notificationsSoundVolumeLabel->setObjectName(QStringLiteral("notificationsSoundVolumeLabel"));
    notificationsSoundVolumeLabel->setTextFormat(Qt::PlainText);

    mNotificationsSoundVolume->setRange(0, 100);
    mNotificationsSoundVolume->setObjectName(QStringLiteral("mNotificationsSoundVolume"));
    connect(mNotificationsSoundVolume, &QSpinBox::valueChanged, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    createLayout(notificationsSoundVolumeLabel, mNotificationsSoundVolume, soundWidgetLayout);

    if (mRocketChatAccount) {
        mSoundModel->setCustomSoundManager(mRocketChatAccount->customSoundManager());
        mSoundNewRoomNotification->setSoundModel(mSoundModel, true);
        mSoundNewMessageNotification->setSoundModel(mSoundModel, true);
    }

    mainLayout->addWidget(soundWidget);

    QWidget *downloadWidget = new QWidget(this);
    downloadWidget->setObjectName(QStringLiteral("downloadWidget"));
    auto downloadWidgetLayout = new QVBoxLayout(downloadWidget);
    downloadWidgetLayout->setObjectName(QStringLiteral("downloadWidgetLayout"));
    downloadWidgetLayout->setContentsMargins({});

    downloadWidgetLayout->addWidget(new KSeparator(this));

    auto downloadLayout = new QHBoxLayout;
    downloadLayout->setObjectName(QStringLiteral("downloadLayout"));
    downloadLayout->setContentsMargins({});

    downloadWidgetLayout->addLayout(downloadLayout);

    auto downloadDataButton = new QPushButton(i18nc("@action:button", "Download my Data (HTML)"), this);
    downloadDataButton->setObjectName(QStringLiteral("downloadDataButton"));
    downloadLayout->addWidget(downloadDataButton);
    connect(downloadDataButton, &QPushButton::clicked, this, [this]() {
        downloadData(false);
    });

    auto exportDataButton = new QPushButton(i18nc("@action:button", "Export my Data (JSON)"), this);
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
        if (!mRocketChatAccount->ruqolaServerConfig()->deviceManagementEnableLoginEmails()
            || !mRocketChatAccount->ruqolaServerConfig()->deviceManagementAllowLoginEmailpreference()) {
            mReceiveLoginDetectionEmails->setVisible(false);
        }
        connect(mRocketChatAccount, &RocketChatAccount::ownUserPreferencesChanged, this, &MyAccountPreferenceConfigureWidget::load);
    }
}

MyAccountPreferenceConfigureWidget::~MyAccountPreferenceConfigureWidget() = default;

void MyAccountPreferenceConfigureWidget::createLayout(QLabel *label, QWidget *widget, QVBoxLayout *layout)
{
    auto newMessageNotificationLayout = new QHBoxLayout;
    newMessageNotificationLayout->setContentsMargins({});
    newMessageNotificationLayout->addWidget(label);
    newMessageNotificationLayout->addWidget(widget);
    layout->addLayout(newMessageNotificationLayout);
}

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
                             i18nc("@title:window", "Download File Requested"));
}

QString MyAccountPreferenceConfigureWidget::desktopPushNotificationI18n(const QString &value) const
{
    if (value == "all"_L1) {
        return i18n("All Messages");
    } else if (value == "mentions"_L1) {
        return i18n("Mentions");
    } else if (value == "nothing"_L1) {
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
    mEmailNotification->addItem(i18n("Default (Every Mention/DM)"), QStringLiteral("default"));
    mEmailNotification->addItem(i18n("Every Mention/DM"), QStringLiteral("mentions"));
    mEmailNotification->addItem(i18n("Disabled"), QStringLiteral("nothing"));
    if (mRocketChatAccount && !mRocketChatAccount->ruqolaServerConfig()->allowEmailNotifications()) {
        mEmailNotification->setEnabled(false);
        mEmailNotificationLabel->setText(i18n("Your Rocket.Chat administrator has disabled email notifications"));
    }

    connect(mDesktopNotification, &QComboBox::activated, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    connect(mPushNotification, &QComboBox::activated, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    connect(mEmailNotification, &QComboBox::activated, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    connect(mSoundNewRoomNotification, &SoundConfigureWidget::soundChanged, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
    connect(mSoundNewMessageNotification, &SoundConfigureWidget::soundChanged, this, &MyAccountPreferenceConfigureWidget::setWasChanged);
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
        info.notificationsSoundVolume = mNotificationsSoundVolume->value();
        info.enableAutoAway = RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToState(mAutomaticAway->isChecked());
        info.muteFocusedConversations =
            RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToState(mMuteFocusedConversations->isChecked());
        info.newMessageNotification = mSoundNewMessageNotification->identifier();
        info.newRoomNotification = mSoundNewRoomNotification->identifier();
        if (mRocketChatAccount) {
            if (mRocketChatAccount->ruqolaServerConfig()->deviceManagementEnableLoginEmails()
                && mRocketChatAccount->ruqolaServerConfig()->deviceManagementAllowLoginEmailpreference()) {
                info.receiveLoginDetectionEmail =
                    RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo::convertToState(mReceiveLoginDetectionEmails->isChecked());
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
    mSoundNewRoomNotification->setCurrentSound(ownUserPreferences.newRoomNotification());
    mSoundNewMessageNotification->setCurrentSound(ownUserPreferences.newMessageNotification());
    mMuteFocusedConversations->setChecked(ownUserPreferences.muteFocusedConversations());
    mNotificationsSoundVolume->setValue(ownUserPreferences.notificationsSoundVolume());
    mChanged = false;
}

void MyAccountPreferenceConfigureWidget::setWasChanged()
{
    mChanged = true;
}

#include "moc_myaccountpreferenceconfigurewidget.cpp"
