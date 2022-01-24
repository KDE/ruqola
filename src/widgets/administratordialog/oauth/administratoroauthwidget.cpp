/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "administratoroauthwidget.h"
#include "connection.h"
#include "misc/listoauthappsjob.h"
#include "oauthtreeview.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QLineEdit>
#include <QVBoxLayout>

AdministratorOauthWidget::AdministratorOauthWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mSearchLineWidget(new QLineEdit(this))
    , mOauthTreeView(new OauthTreeView(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineWidget->setObjectName(QStringLiteral("mSearchLineWidget"));
    mainLayout->addWidget(mSearchLineWidget);

    mOauthTreeView->setObjectName(QStringLiteral("mOauthTreeView"));
    mainLayout->addWidget(mOauthTreeView);
}

AdministratorOauthWidget::~AdministratorOauthWidget() = default;

void AdministratorOauthWidget::initialize()
{
    auto oauthListJob = new RocketChatRestApi::ListOauthAppsJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(oauthListJob);
    connect(oauthListJob, &RocketChatRestApi::ListOauthAppsJob::listOauthDone, this, &AdministratorOauthWidget::slotListOauthDone);
    if (!oauthListJob->start()) {
        qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start ListInviteJob";
    }
}

void AdministratorOauthWidget::slotListOauthDone(const QJsonObject &obj)
{
    qDebug() << " obj " << obj;
}
