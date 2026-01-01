/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountprofileconfigurewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "connection.h"
#include "dialogs/asktwoauthenticationpassworddialog.h"
#include "misc/passwordconfirmwidget.h"
#include "myaccountprofileconfigureavatarwidget.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "users/deleteownaccountjob.h"
#include "users/userslogoutotherclientsjob.h"
#include "users/usersupdateownbasicinfojob.h"
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
    topLayout->setObjectName(u"topLayout"_s);

    mConfigureAvatarWidget->setObjectName(u"mConfigureAvatarWidget"_s);
    topLayout->addWidget(mConfigureAvatarWidget);

    auto mainLayout = new QFormLayout;
    mainLayout->setObjectName(u"mainLayout"_s);
    topLayout->addLayout(mainLayout);

    mName->setObjectName(u"mName"_s);
    KLineEditEventHandler::catchReturnKey(mName);
    mainLayout->addRow(i18n("Name:"), mName);
    mName->setClearButtonEnabled(true);

    mUserName->setObjectName(u"mUserName"_s);
    KLineEditEventHandler::catchReturnKey(mUserName);
    mainLayout->addRow(i18n("Username:"), mUserName);
    mUserNameInfo->setPalette(pal);
    mUserNameInfo->setObjectName(u"mUserNameInfo"_s);
    mainLayout->addWidget(mUserNameInfo);

    mEmail->setObjectName(u"mEmail"_s);
    KLineEditEventHandler::catchReturnKey(mEmail);
    mainLayout->addRow(i18n("Email:"), mEmail);

    mEmailInfo->setPalette(pal);
    mEmailInfo->setObjectName(u"mEmailInfo"_s);
    mainLayout->addWidget(mEmailInfo);

    mNickName->setObjectName(u"mNickName"_s);
    KLineEditEventHandler::catchReturnKey(mNickName);
    mainLayout->addRow(i18n("Nickname:"), mNickName);
    mNickName->setClearButtonEnabled(true);

    KLineEditEventHandler::catchReturnKey(mStatusText);
    mStatusText->setObjectName(u"mStatusText"_s);
    mainLayout->addRow(i18n("Status text:"), mStatusText);
    mStatusText->setClearButtonEnabled(true);

    mStatusTextInfo->setPalette(pal);
    mStatusTextInfo->setObjectName(u"mStatusTextInfo"_s);
    mainLayout->addWidget(mStatusTextInfo);

    mPasswordConfirmWidget->setObjectName(u"mPasswordConfirmWidget"_s);
    mainLayout->addRow(mPasswordConfirmWidget);
    if (mRocketChatAccount) {
        mPasswordConfirmWidget->setPasswordValidChecks(mRocketChatAccount->ruqolaServerConfig()->passwordSettings());
    }

    mDeleteMyAccount->setObjectName(u"mDeleteMyAccount"_s);
    mainLayout->addWidget(mDeleteMyAccount);
    connect(mDeleteMyAccount, &QPushButton::clicked, this, &MyAccountProfileConfigureWidget::slotDeleteMyAccount);

    mLogoutFromOtherLocation->setObjectName(u"mLogoutFromOtherLocation"_s);
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
        dlg->setRevealPasswordMode(KAuthorized::authorize(u"lineedit_reveal_password"_s) ? KPassword::RevealMode::OnlyNew : KPassword::RevealMode::Never);
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
    mUserName->setReadOnly(!mRocketChatAccount->ruqolaServerConfig()->allowUsernameChange());
    mUserNameInfo->setVisible(!mRocketChatAccount->ruqolaServerConfig()->allowUsernameChange());

    mEmail->setReadOnly(!mRocketChatAccount->ruqolaServerConfig()->allowEmailChange());
    mEmailInfo->setVisible(!mRocketChatAccount->ruqolaServerConfig()->allowEmailChange());

    mStatusTextInfo->setVisible(!mRocketChatAccount->ruqolaServerConfig()->allowCustomStatusMessage());
    mPasswordConfirmWidget->setVisible(mRocketChatAccount->ruqolaServerConfig()->allowPasswordChange());
    mDeleteMyAccount->setVisible(mRocketChatAccount->ruqolaServerConfig()->allowDeleteOwnAccount());
    mConfigureAvatarWidget->setVisible(mRocketChatAccount->ruqolaServerConfig()->allowAvatarChanged());
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
        dlg->setRevealPasswordMode(KAuthorized::authorize(u"lineedit_reveal_password"_s) ? KPassword::RevealMode::OnlyNew : KPassword::RevealMode::Never);
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
        if (dlg->exec()) {
            const QString code = dlg->code();
            qWarning() << " Code not used yet ! Implement it";
            // TODO use code !
        }
        delete dlg;
    }

    // TODO add more.
    if (updateInfo.isValid()) {
        auto job = new RocketChatRestApi::UsersUpdateOwnBasicInfoJob(this);
        job->setUpdateOwnBasicInfo(updateInfo);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        // Clear all other tokens when password was changed
        // TODO fix me connect(job, &UsersUpdateOwnBasicInfoJob::passwordChanged, this, &Connection::updateOwnBasicInfoDone);

        if (!job->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start UsersUpdateOwnBasicInfoJob";
        }
    }
}

#include "moc_myaccountprofileconfigurewidget.cpp"
