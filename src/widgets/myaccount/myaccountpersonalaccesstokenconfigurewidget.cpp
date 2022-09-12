/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesstokenconfigurewidget.h"
#include "connection.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/personalaccesstokeninfosfilterproxymodel.h"
#include "model/personalaccesstokeninfosmodel.h"
#include "myaccountpersonalaccesscreatedialog.h"
#include "myaccountpersonalaccesstokentreeview.h"
#include "personalaccesstoken/generatepersonalaccesstokenjob.h"
#include "personalaccesstoken/getpersonalaccesstokensjob.h"
#include "personalaccesstoken/regeneratepersonalaccesstokenjob.h"
#include "personalaccesstoken/removepersonalaccesstokenjob.h"

#include "personalaccesstokens/personalaccesstokeninfos.h"

#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
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
    connect(mPersonalAccessTokenTreeView,
            &MyAccountPersonalAccessTokenTreeView::createToken,
            this,
            &MyAccountPersonalAccessTokenConfigureWidget::slotCreateToken);
    connect(mPersonalAccessTokenTreeView,
            &MyAccountPersonalAccessTokenTreeView::removeToken,
            this,
            &MyAccountPersonalAccessTokenConfigureWidget::slotRemoveToken);
    connect(mPersonalAccessTokenTreeView,
            &MyAccountPersonalAccessTokenTreeView::regenerateToken,
            this,
            &MyAccountPersonalAccessTokenConfigureWidget::slotRegenerateToken);
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
            mPersonalAccessTokenModel->insertPersonalAccessTokenInfos(info);
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start GetPersonalAccessTokensJob job";
        }
    }
}

void MyAccountPersonalAccessTokenConfigureWidget::slotCreateToken()
{
    QPointer<MyAccountPersonalAccessCreateDialog> createDialog = new MyAccountPersonalAccessCreateDialog(this);
    if (createDialog->exec()) {
        if (mRocketChatAccount) {
            auto job = new RocketChatRestApi::GeneratePersonalAccessTokenJob(this);
            job->setTokenName(createDialog->tokenName());
            job->setBypassTwoFactor(createDialog->bypassTwoFactor());

            mRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::GeneratePersonalAccessTokenJob::generateTokenDone, this, [this](const QJsonObject &obj) {
                qDebug() << " obj " << obj;
                const QString token = obj[QLatin1String("token")].toString();

                // TODO update list

                //            PersonalAccessTokenInfos info;
                //            info.parsePersonalAccessTokenInfos(obj);
                //            mPersonalAccessTokenModel->insertPersonalAccessTokenInfos(info);
            });
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start GeneratePersonalAccessTokenJob job";
            }
        }
    }
    delete createDialog;
}

void MyAccountPersonalAccessTokenConfigureWidget::slotRemoveToken(const QString &tokenName)
{
    if (mRocketChatAccount) {
        auto job = new RocketChatRestApi::RemovePersonalAccessTokenJob(this);
        job->setTokenName(tokenName);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::RemovePersonalAccessTokenJob::removeTokenDone, this, [this](const QJsonObject &obj) {
            qDebug() << " obj " << obj;
            KMessageBox::information(this, i18n("Personal Token removed."), i18n("Remove Personal Token"));
            // TODO update list.
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start RemovePersonalAccessTokenJob job";
        }
    }
}

void MyAccountPersonalAccessTokenConfigureWidget::slotRegenerateToken(const QString &tokenName)
{
    if (mRocketChatAccount) {
        auto job = new RocketChatRestApi::RegeneratePersonalAccessTokenJob(this);
        job->setTokenName(tokenName);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::RegeneratePersonalAccessTokenJob::regenerateTokenDone, this, [this](const QJsonObject &obj) {
            const QString token = obj[QLatin1String("token")].toString();
            // Add KMessageBox::
            qDebug() << " obj " << obj;
            //            PersonalAccessTokenInfos info;
            //            info.parsePersonalAccessTokenInfos(obj);
            //            mPersonalAccessTokenModel->insertPersonalAccessTokenInfos(info);
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start RegeneratePersonalAccessTokenJob job";
        }
    }
}
