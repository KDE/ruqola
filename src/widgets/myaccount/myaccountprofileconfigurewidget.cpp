/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountprofileconfigurewidget.h"
#include "connection.h"
#include "dialogs/asktwoauthenticationpassworddialog.h"
#include "misc/passwordconfirmwidget.h"
#include "myaccountprofileconfigureavatarwidget.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "users/deleteownaccountjob.h"
#include "users/userslogoutotherclientsjob.h"
#include <KAuthorized>
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KMessageBox>
#include <KPasswordDialog>
#include <KStatefulBrush>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>

MyAccountProfileConfigureWidget::MyAccountProfileConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mConfigureAvatarWidget(new MyAccountProfileConfigureAvatarWidget(account, this))
    , mName(new QLineEdit(this))
    , mUserName(new QLineEdit(this))
    , mEmail(new QLineEdit(this))
    , mEmailInfo(new QLabel(i18nc("@label:textbox", "Your administrator has disabled the changing of email."), this))
    , mUserNameInfo(new QLabel(i18nc("@label:textbox", "Your administrator has disabled the changing of usernames."), this))
    , mNickName(new QLineEdit(this))
    , mStatusText(new QLineEdit(this))
    , mStatusTextInfo(new QLabel(i18nc("@label:textbox", "Your administrator has disabled the changing of status messages."), this))
    , mDeleteMyAccount(new QPushButton(i18nc("@action:button", "Delete my Account"), this))
    , mLogoutFromOtherLocation(new QPushButton(i18nc("@action:button", "Logout From Other Logged In Locations"), this))
    , mPasswordConfirmWidget(new PasswordConfirmWidget(this))
    , mRocketChatAccount(account)
{
    const KStatefulBrush bgBrush(KColorScheme::View, KColorScheme::NegativeText);
    const QColor color = bgBrush.brush(palette()).color();

    QPalette pal = this->palette();
    pal.setColor(QPalette::WindowText, color);

    auto topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("topLayout"));

    mConfigureAvatarWidget->setObjectName(QStringLiteral("mConfigureAvatarWidget"));
    topLayout->addWidget(mConfigureAvatarWidget);

    auto mainLayout = new QFormLayout;
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    topLayout->addLayout(mainLayout);

    mName->setObjectName(QStringLiteral("mName"));
    KLineEditEventHandler::catchReturnKey(mName);
    mainLayout->addRow(i18n("Name:"), mName);
    mName->setClearButtonEnabled(true);

    mUserName->setObjectName(QStringLiteral("mUserName"));
    KLineEditEventHandler::catchReturnKey(mUserName);
    mainLayout->addRow(i18n("Username:"), mUserName);
    mUserNameInfo->setPalette(pal);
    mUserNameInfo->setObjectName(QStringLiteral("mUserNameInfo"));
    mainLayout->addWidget(mUserNameInfo);

    mEmail->setObjectName(QStringLiteral("mEmail"));
    KLineEditEventHandler::catchReturnKey(mEmail);
    mainLayout->addRow(i18n("Email:"), mEmail);

    mEmailInfo->setPalette(pal);
    mEmailInfo->setObjectName(QStringLiteral("mEmailInfo"));
    mainLayout->addWidget(mEmailInfo);

    mNickName->setObjectName(QStringLiteral("mNickName"));
    KLineEditEventHandler::catchReturnKey(mNickName);
    mainLayout->addRow(i18n("Nickname:"), mNickName);
    mNickName->setClearButtonEnabled(true);

    KLineEditEventHandler::catchReturnKey(mStatusText);
    mStatusText->setObjectName(QStringLiteral("mStatusText"));
    mainLayout->addRow(i18n("Status text:"), mStatusText);
    mStatusText->setClearButtonEnabled(true);

    mStatusTextInfo->setPalette(pal);
    mStatusTextInfo->setObjectName(QStringLiteral("mStatusTextInfo"));
    mainLayout->addWidget(mStatusTextInfo);

    mPasswordConfirmWidget->setObjectName(QStringLiteral("mPasswordConfirmWidget"));
    mainLayout->addRow(mPasswordConfirmWidget);
    if (mRocketChatAccount) {
        mPasswordConfirmWidget->setPasswordValidChecks(mRocketChatAccount->ruqolaServerConfig()->passwordSettings());
    }

    mDeleteMyAccount->setObjectName(QStringLiteral("mDeleteMyAccount"));
    mainLayout->addWidget(mDeleteMyAccount);
    connect(mDeleteMyAccount, &QPushButton::clicked, this, &MyAccountProfileConfigureWidget::slotDeleteMyAccount);

    mLogoutFromOtherLocation->setObjectName(QStringLiteral("mLogoutFromOtherLocation"));
    mainLayout->addWidget(mLogoutFromOtherLocation);
    connect(mLogoutFromOtherLocation, &QPushButton::clicked, this, &MyAccountProfileConfigureWidget::slotLogoutFromOtherLocation);
    topLayout->addStretch();
}

MyAccountProfileConfigureWidget::~MyAccountProfileConfigureWidget() = default;

void MyAccountProfileConfigureWidget::slotLogoutFromOtherLocation()
{
    auto job = new RocketChatRestApi::UsersLogoutOtherClientsJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::UsersLogoutOtherClientsJob::usersLogoutOtherClientsDone, this, []() {
        qDebug() << " UsersLogoutOtherClientsJob::usersLogoutOtherClientsDone";
        // TODO ?
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start UsersLogoutOtherClientsJob job";
    }
}

void MyAccountProfileConfigureWidget::slotDeleteMyAccount()
{
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
                                           i18n("Do you really want to delete your account?"),
                                           i18nc("@title", "Delete my Account"),
                                           KStandardGuiItem::del(),
                                           KStandardGuiItem::cancel())) {
        QPointer<KPasswordDialog> dlg = new KPasswordDialog(this);
        dlg->setRevealPasswordMode(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")) ? KPassword::RevealMode::OnlyNew
                                                                                                      : KPassword::RevealMode::Never);
        dlg->setPrompt(i18n("Current Password"));
        if (dlg->exec()) {
            auto job = new RocketChatRestApi::DeleteOwnAccountJob(this);
            job->setPassword(dlg->password());
            mRocketChatAccount->restApi()->initializeRestApiJob(job);
            if (!job->start()) {
                qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start DeleteOwnAccountJob";
            }
        }
        delete dlg;
    }
}

void MyAccountProfileConfigureWidget::initialize()
{
    mUserName->setReadOnly(!mRocketChatAccount->allowUsernameChange());
    mUserNameInfo->setVisible(!mRocketChatAccount->allowUsernameChange());

    mEmail->setReadOnly(!mRocketChatAccount->allowEmailChange());
    mEmailInfo->setVisible(!mRocketChatAccount->allowEmailChange());

    mStatusTextInfo->setVisible(!mRocketChatAccount->allowCustomStatusMessage());
    mPasswordConfirmWidget->setVisible(mRocketChatAccount->allowPasswordChange());
    mDeleteMyAccount->setVisible(mRocketChatAccount->allowDeleteOwnAccount());
    mConfigureAvatarWidget->setVisible(mRocketChatAccount->allowAvatarChanged());
}

void MyAccountProfileConfigureWidget::load()
{
    mOwnUser = mRocketChatAccount->ownUser();
    mEmail->setText(mOwnUser.email());
    mName->setText(mOwnUser.name());
    mUserName->setText(mOwnUser.userName());
    mNickName->setText(mOwnUser.nickName());
    mStatusText->setText(mOwnUser.statusText());
    Utils::AvatarInfo info;
    info.avatarType = Utils::AvatarType::User;
    info.identifier = mOwnUser.userName();
    const QUrl iconUrlStr = QUrl(mRocketChatAccount->avatarUrl(info));
    if (!iconUrlStr.isEmpty()) {
        const QString iconPath{QUrl(iconUrlStr).toLocalFile()};
        mConfigureAvatarWidget->setCurrentIconPath(iconPath);
    }
}

void MyAccountProfileConfigureWidget::save()
{
    RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo updateInfo;
    if (!mEmail->isReadOnly() && (mOwnUser.email() != mEmail->text())) {
        updateInfo.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::Email;
        updateInfo.email = mEmail->text();
    }
    if (!mNickName->isReadOnly() && (mOwnUser.nickName() != mNickName->text())) {
        updateInfo.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::NickName;
        updateInfo.nickName = mNickName->text();
    }
    if (!mUserName->isReadOnly() && (mOwnUser.userName() != mUserName->text())) {
        updateInfo.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::UserName;
        updateInfo.userName = mUserName->text();
    }
    if (!mStatusText->isReadOnly() && (mOwnUser.statusText() != mStatusText->text())) {
        updateInfo.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::StatusText;
        updateInfo.statusText = mStatusText->text();
    }
    if (!mName->isReadOnly() && (mOwnUser.name() != mName->text())) {
        updateInfo.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::Name;
        updateInfo.name = mName->text();
    }
    if (mPasswordConfirmWidget->isVisible() && mPasswordConfirmWidget->isNewPasswordConfirmed()) {
        updateInfo.type |= RocketChatRestApi::UsersUpdateOwnBasicInfoJob::UpdateOwnBasicInfo::BasicInfoType::Password;
        updateInfo.newPassword = mPasswordConfirmWidget->password(); // Not encrypt it ???!
        QPointer<KPasswordDialog> dlg = new KPasswordDialog(this);
        dlg->setPrompt(i18n("Current Password"));
        dlg->setRevealPasswordMode(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")) ? KPassword::RevealMode::OnlyNew
                                                                                                      : KPassword::RevealMode::Never);
        if (dlg->exec()) {
            updateInfo.currentPassword = Utils::convertSha256Password(dlg->password());
        } else {
            delete dlg;
            return;
        }
        delete dlg;
    }
    if (mRocketChatAccount->ownUser().servicePassword().email2faEnabled()) { // TODO verify it
        QPointer<AskTwoAuthenticationPasswordDialog> dlg = new AskTwoAuthenticationPasswordDialog(this);
        dlg->setRocketChatAccount(mRocketChatAccount);
        QString code;
        if (dlg->exec()) {
            code = dlg->code();
            qWarning() << " Code not used yet ! Implement it";
            // TODO use code !
        }
        delete dlg;
    }

    // TODO add more.
    if (updateInfo.isValid()) {
        mRocketChatAccount->updateOwnBasicInfo(updateInfo);
    }
}

#include "moc_myaccountprofileconfigurewidget.cpp"
