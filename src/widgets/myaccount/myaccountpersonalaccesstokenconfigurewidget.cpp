/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesstokenconfigurewidget.h"
#include "connection.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/personalaccesstokeninfosfilterproxymodel.h"
#include "model/personalaccesstokeninfosmodel.h"
#include "myaccountpersonalaccesstokentreeview.h"
#include "personalaccesstoken/getpersonalaccesstokensjob.h"
#include "personalaccesstokens/personalaccesstokeninfos.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QLineEdit>
#include <QVBoxLayout>

MyAccountPersonalAccessTokenConfigureWidget::MyAccountPersonalAccessTokenConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mSearchLineWidget(new QLineEdit(this))
    , mPersonalAccessTokenTreeView(new MyAccountPersonalAccessTokenTreeView(account, this))
    , mPersonalAccessTokenModel(new PersonalAccessTokenInfosModel(this))

{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineWidget->setObjectName(QStringLiteral("mSearchLineWidget"));
    mainLayout->addWidget(mSearchLineWidget);
    mSearchLineWidget->setPlaceholderText(i18n("Search Personal Access Token..."));
    mPersonalAccessTokenTreeView->setObjectName(QStringLiteral("mPersonalAccessTokenTreeView"));
    mainLayout->addWidget(mPersonalAccessTokenTreeView);

    new LineEditCatchReturnKey(mSearchLineWidget, this);

    mPersonalAccessTokenModel->setObjectName(QStringLiteral("mPersonalAccessTokenModel"));

    auto proxyModel = new PersonalAccessTokenInfosFilterProxyModel(this);
    proxyModel->setObjectName(QStringLiteral("proxyModel"));
    proxyModel->setSourceModel(mPersonalAccessTokenModel);
    mPersonalAccessTokenTreeView->setModel(proxyModel);
    mPersonalAccessTokenTreeView->setColumnHidden(PersonalAccessTokenInfosModel::CreateAtDateTime, true);
}

MyAccountPersonalAccessTokenConfigureWidget::~MyAccountPersonalAccessTokenConfigureWidget() = default;

void MyAccountPersonalAccessTokenConfigureWidget::initialize()
{
    if (mRocketChatAccount) {
        auto job = new RocketChatRestApi::GetPersonalAccessTokensJob(this);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::GetPersonalAccessTokensJob::getPersonalAccessTokensDone, this, [this](const QJsonObject &obj) {
            PersonalAccessTokenInfos info;
            info.parsePersonalAccessTokenInfos(obj);
            qDebug() << " info " << info;
            mPersonalAccessTokenModel->insertPersonalAccessTokenInfos(info);
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start GetPersonalAccessTokensJob job";
        }
    }
}
