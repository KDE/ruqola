/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2e2configurewidget.h"
#include "connection.h"
#include "e2e/resetowne2ekeyjob.h"
#include "misc/passwordconfirmwidget.h"
#include "personalaccesstoken/getpersonalaccesstokensjob.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

MyAccount2e2ConfigureWidget::MyAccount2e2ConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mResetE2eKey(new QPushButton(i18n("Reset E2E Key"), this))
    , mRocketChatAccount(account)
    , mPasswordConfirmWidget(new PasswordConfirmWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    auto changePasswordLabel =
        new QLabel(i18n("You can now create encrypted private groups and direct messages. You may also change existing private groups or DMs to encrypted. "
                        "This is end to end encryption so the key to encode/decode your messages will not be saved on the server. For that reason you need to "
                        "store your password somewhere safe. You will be required to enter it on other devices you wish to use e2e encryption on."),
                   this);
    changePasswordLabel->setWordWrap(true);
    changePasswordLabel->setObjectName(QStringLiteral("changePasswordLabel"));
    mainLayout->addWidget(changePasswordLabel);

    // TODO add change password
    mPasswordConfirmWidget->setObjectName(QStringLiteral("mPasswordConfirmWidget"));
    mainLayout->addWidget(mPasswordConfirmWidget);

    auto removePasswordlabel = new QLabel(
        i18n("This option will remove your current E2E key and log you out. "
             "When you login again, Rocket.Chat will generate you a new key and restore your access to any encrypted room that has one or more members online."
             " Due to the nature of the E2E encryption, Rocket.Chat will not be able to restore access to any encrypted room that has no member online."),
        this);
    removePasswordlabel->setObjectName(QStringLiteral("removePasswordlabel"));
    removePasswordlabel->setWordWrap(true);
    mainLayout->addWidget(removePasswordlabel);

    mResetE2eKey->setObjectName(QStringLiteral("mResetE2eKey"));
    mainLayout->addWidget(mResetE2eKey);
    connect(mResetE2eKey, &QPushButton::clicked, this, &MyAccount2e2ConfigureWidget::slotResetE2EKey);
    mainLayout->addStretch(1);

    {
        auto job = new RocketChatRestApi::GetPersonalAccessTokensJob(this);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::GetPersonalAccessTokensJob::getPersonalAccessTokensDone, this, [this](const QJsonObject &obj) {
            qDebug() << " obj " << obj;
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start GetPersonalAccessTokensJob job";
        }
    }
}

void MyAccount2e2ConfigureWidget::save()
{
    // TODO
}

void MyAccount2e2ConfigureWidget::load()
{
    // TODO
}

MyAccount2e2ConfigureWidget::~MyAccount2e2ConfigureWidget() = default;

void MyAccount2e2ConfigureWidget::slotResetE2EKey()
{
    // TODO it uses "/api/v1/method.call/e2e.resetOwnE2EKey"
    // => use restapi for calling ddp method
    auto job = new RocketChatRestApi::ResetOwnE2eKeyJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::ResetOwnE2eKeyJob::resetE2eKeyDone, this, &MyAccount2e2ConfigureWidget::slotReset2E2KeyDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ResetOwnE2eKeyJob job";
    }
}

void MyAccount2e2ConfigureWidget::slotReset2E2KeyDone(const QJsonObject &replyObject)
{
    qDebug() << " replyObject " << replyObject;
}
