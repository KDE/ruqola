/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccount2e2configurewidget.h"
#include "connection.h"
#include "e2e/resetowne2ekeyjob.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QPushButton>
#include <QVBoxLayout>

MyAccount2e2ConfigureWidget::MyAccount2e2ConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mResetE2eKey(new QPushButton(i18n("Reset E2E Key"), this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mResetE2eKey->setObjectName(QStringLiteral("mResetE2eKey"));
    mainLayout->addWidget(mResetE2eKey);
    connect(mResetE2eKey, &QPushButton::clicked, this, &MyAccount2e2ConfigureWidget::slotResetE2EKey);
}

MyAccount2e2ConfigureWidget::~MyAccount2e2ConfigureWidget() = default;

void MyAccount2e2ConfigureWidget::slotResetE2EKey()
{
    auto job = new RocketChatRestApi::ResetOwnE2eKeyJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::ResetOwnE2eKeyJob::resetE2eKeyDone, this, &MyAccount2e2ConfigureWidget::slotReset2E2KeyDone);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ResetOwnE2eKeyJob job";
    }
}

void MyAccount2e2ConfigureWidget::slotReset2E2KeyDone(const QJsonObject &replyObject)
{
}
