/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountprofileconfigurewidget.h"
#include "dialogs/asktwoauthenticationpassworddialog.h"
#include "misc/passwordconfirmwidget.h"
#include "myaccountprofileconfigureavatarwidget.h"
#include "rocketchataccount.h"
#include <KAuthorized>
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <KMessageBox>
#include <KPasswordDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>
#include <kwidgetsaddons_version.h>

MyAccountProfileConfigureWidget::MyAccountProfileConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mEmail(new QLineEdit(this))
    , mEmailInfo(new QLabel(i18n("Your administrator has disabled the changing of email."), this))
    , mName(new QLineEdit(this))
    , mUserName(new QLineEdit(this))
    , mNickName(new QLineEdit(this))
    , mStatusText(new QLineEdit(this))
    , mDeleteMyAccount(new QPushButton(i18n("Delete my Account"), this))
    , mLogoutFromOtherLocation(new QPushButton(i18n("Logout From Other Logged In Locations"), this))
    , mPasswordConfirmWidget(new PasswordConfirmWidget(this))
    , mConfigureAvatarWidget(new MyAccountProfileConfigureAvatarWidget(account, this))
    , mRocketChatAccount(account)
{
    auto topLayout = new QVBoxLayout(this);
    topLayout->setObjectName(QStringLiteral("topLayout"));
    topLayout->setContentsMargins({});

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

    mEmail->setObjectName(QStringLiteral("mEmail"));
    KLineEditEventHandler::catchReturnKey(mEmail);
    mainLayout->addRow(i18n("Email:"), mEmail);

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

    mPasswordConfirmWidget->setObjectName(QStringLiteral("mPasswordConfirmWidget"));
    mainLayout->addRow(mPasswordConfirmWidget);

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
    mRocketChatAccount->logoutFromOtherLocation();
}

void MyAccountProfileConfigureWidget::slotDeleteMyAccount()
{
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
                                           i18n("Do you really delete your account ?"),
                                           i18nc("@title", "Delete my Account"),
                                           KStandardGuiItem::del(),
                                           KStandardGuiItem::cancel())) {
        QPointer<KPasswordDialog> dlg = new KPasswordDialog(this);
#if KWIDGETSADDONS_VERSION <= QT_VERSION_CHECK(5, 249, 0)
        dlg->setRevealPasswordAvailable(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")));
#else
        dlg->setRevealPasswordMode(KAuthorized::authorize(QStringLiteral("lineedit_reveal_password")) ? KPassword::RevealMode::OnlyNew
                                                                                                      : KPassword::RevealMode::Never);
#endif
        dlg->setPrompt(i18n("Current Password"));
        if (dlg->exec()) {
            mRocketChatAccount->deleteOwnAccount(dlg->password());
        }
        delete dlg;
    }
}

void MyAccountProfileConfigureWidget::initialize()
{
    mUserName->setReadOnly(!mRocketChatAccount->allowUsernameChange());
    mEmail->setReadOnly(!mRocketChatAccount->allowEmailChange());
    mEmailInfo->setVisible(!mRocketChatAccount->allowEmailChange());

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
