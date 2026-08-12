/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2e2configurewidget.h"

#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "misc/methodcalljob.h"
#include "misc/passwordconfirmwidget.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <KSeparator>
#include <QJsonArray>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
MyAccount2e2ConfigureWidget::MyAccount2e2ConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mPasswordConfirmWidget(new PasswordConfirmWidget(this))
    , mResetE2eKey(new QPushButton(i18nc("@action:button", "Reset E2E Key"), this))
    , mModifyE2ePassword(new QPushButton(i18nc("@action:button", "Modify"), this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    auto changePasswordLabel =
        new QLabel(i18nc("@label:textbox",
                         "You can now create encrypted private groups and direct messages. You may also change existing private groups or DMs to encrypted. "
                         "This is end to end encryption so the key to encode/decode your messages will not be saved on the server. For that reason you need to "
                         "store your password somewhere safe. You will be required to enter it on other devices you wish to use e2e encryption on."),
                   this);
    changePasswordLabel->setWordWrap(true);
    changePasswordLabel->setObjectName(u"changePasswordLabel"_s);
    mainLayout->addWidget(changePasswordLabel);

    mPasswordConfirmWidget->setObjectName(u"mPasswordConfirmWidget"_s);
    mainLayout->addWidget(mPasswordConfirmWidget);
    mModifyE2ePassword->setObjectName(u"mModifyE2ePassword"_s);
    mainLayout->addWidget(mModifyE2ePassword);
    connect(mModifyE2ePassword, &QPushButton::clicked, this, &MyAccount2e2ConfigureWidget::slotModifyPassword);
    mModifyE2ePassword->setEnabled(false);
    connect(mPasswordConfirmWidget, &PasswordConfirmWidget::passwordValidated, mModifyE2ePassword, &QPushButton::setEnabled);

    mainLayout->addWidget(new KSeparator(this));
    auto removePasswordlabel = new QLabel(
        i18n("This option will remove your current E2E key and log you out. "
             "When you login again, Rocket.Chat will generate you a new key and restore your access to any encrypted room that has one or more members online."
             " Due to the nature of the E2E encryption, Rocket.Chat will not be able to restore access to any encrypted room that has no member online."),
        this);
    removePasswordlabel->setObjectName(u"removePasswordlabel"_s);
    removePasswordlabel->setWordWrap(true);
    mainLayout->addWidget(removePasswordlabel);

    mResetE2eKey->setObjectName(u"mResetE2eKey"_s);
    mainLayout->addWidget(mResetE2eKey);
    connect(mResetE2eKey, &QPushButton::clicked, this, &MyAccount2e2ConfigureWidget::slotResetE2EKey);
    mainLayout->addStretch(1);
}

MyAccount2e2ConfigureWidget::~MyAccount2e2ConfigureWidget() = default;

void MyAccount2e2ConfigureWidget::slotModifyPassword()
{
    // TODO generate new public/private keys
    // TODO send it: api/v1/e2e.setUserPublicAndPrivateKeys
}

void MyAccount2e2ConfigureWidget::slotResetE2EKey()
{
    // it uses "/api/v1/method.call/e2e.resetOwnE2EKey"
    // => use restapi for calling ddp method
    auto job = new RocketChatRestApi::MethodCallJob(this);
    const QJsonArray params;
    const QString methodName = u"e2e.resetOwnE2EKey"_s;
    const RocketChatRestApi::MethodCallJob::MethodCallJobInfo info{
        .messageObj = mRocketChatAccount->ddp()->generateJsonObject(methodName, params),
        .methodName = methodName,
        .anonymous = false,
    };
    job->setMethodCallJobInfo(info);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::MethodCallJob::methodCallDone, this, &MyAccount2e2ConfigureWidget::slotReset2E2KeyDone);
    connect(job, &RocketChatRestApi::MethodCallJob::methodCallFailed, this, &MyAccount2e2ConfigureWidget::slotReset2E2KeyFailed);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ResetOwnE2eKeyJob job";
    }
}

void MyAccount2e2ConfigureWidget::slotReset2E2KeyDone(const QJsonObject &replyObject)
{
    qCDebug(RUQOLAWIDGETS_LOG) << "slotReset2E2KeyDone:" << replyObject;
    // The server keeps the key when the method throws. Dropping it locally then would be pointless
    // anyway: the next login fetches it back from the server and stores it again.
    if (replyObject.contains("error"_L1)) {
        return;
    }
    mRocketChatAccount->resetE2eKey();
}

void MyAccount2e2ConfigureWidget::slotReset2E2KeyFailed(const QJsonObject &errorObject)
{
    qCWarning(RUQOLAWIDGETS_LOG) << "slotReset2E2KeyFailed:" << errorObject;
    const QString reason = errorObject.value("reason"_L1).toString();
    KMessageBox::error(this,
                       reason.isEmpty() ? i18n("The server refused to reset your E2E key.") : i18n("The server refused to reset your E2E key: %1", reason),
                       i18nc("@title:window", "Reset E2E Key"));
}

#include "moc_myaccount2e2configurewidget.cpp"
