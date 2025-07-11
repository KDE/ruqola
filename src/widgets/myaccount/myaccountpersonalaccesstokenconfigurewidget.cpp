/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesstokenconfigurewidget.h"

#include "connection.h"
#include "model/personalaccesstokeninfosfilterproxymodel.h"
#include "model/personalaccesstokeninfosmodel.h"
#include "myaccountpersonalaccesscreatedialog.h"
#include "myaccountpersonalaccesstokentreeview.h"
#include "personalaccesstoken/generatepersonalaccesstokenjob.h"
#include "personalaccesstoken/getpersonalaccesstokensjob.h"
#include "personalaccesstoken/regeneratepersonalaccesstokenjob.h"
#include "personalaccesstoken/removepersonalaccesstokenjob.h"
#include <KLineEditEventHandler>

#include "dialogs/confirmpassworddialog.h"
#include "personalaccesstokens/personalaccesstokeninfos.h"

#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QLineEdit>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;

MyAccountPersonalAccessTokenConfigureWidget::MyAccountPersonalAccessTokenConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mSearchLineWidget(new QLineEdit(this))
    , mPersonalAccessTokenTreeView(new MyAccountPersonalAccessTokenTreeView(this))
    , mPersonalAccessTokenModel(new PersonalAccessTokenInfosModel(this))
    , mPersonalAccessTokenFilterProxyModel(new PersonalAccessTokenInfosFilterProxyModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mSearchLineWidget->setObjectName(u"mSearchLineWidget"_s);
    mainLayout->addWidget(mSearchLineWidget);
    mSearchLineWidget->setPlaceholderText(i18nc("@info:placeholder", "Search Personal Access Token…"));
    mPersonalAccessTokenTreeView->setObjectName(u"mPersonalAccessTokenTreeView"_s);
    mPersonalAccessTokenTreeView->setProperty("_breeze_borders_sides", QVariant::fromValue(QFlags{Qt::TopEdge}));
    mainLayout->addWidget(mPersonalAccessTokenTreeView);

    KLineEditEventHandler::catchReturnKey(mSearchLineWidget);

    mPersonalAccessTokenModel->setObjectName(u"mPersonalAccessTokenModel"_s);

    mPersonalAccessTokenFilterProxyModel->setObjectName(u"proxyModel"_s);
    mPersonalAccessTokenFilterProxyModel->setSourceModel(mPersonalAccessTokenModel);
    mPersonalAccessTokenTreeView->setModel(mPersonalAccessTokenFilterProxyModel);
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
    connect(mSearchLineWidget, &QLineEdit::textChanged, this, &MyAccountPersonalAccessTokenConfigureWidget::slotTextChanged);
}

MyAccountPersonalAccessTokenConfigureWidget::~MyAccountPersonalAccessTokenConfigureWidget() = default;

void MyAccountPersonalAccessTokenConfigureWidget::slotTextChanged(const QString &str)
{
    mPersonalAccessTokenFilterProxyModel->setFilterString(str);
}

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
    createDialog->setExistingTokenNames(mPersonalAccessTokenModel->tokenNames());
    if (createDialog->exec()) {
        if (mRocketChatAccount) {
            QString password;
            const bool twoFactorAuthenticationEnforcePasswordFallback = true; // mRocketChatAccount->twoFactorAuthenticationEnforcePasswordFallback();
            if (twoFactorAuthenticationEnforcePasswordFallback) {
                QPointer<ConfirmPasswordDialog> dialog(new ConfirmPasswordDialog(this));
                if (dialog->exec()) {
                    password = dialog->password();
                }
                delete dialog;
            }
            if (!password.isEmpty()) {
                auto job = new RocketChatRestApi::GeneratePersonalAccessTokenJob(this);
                if (twoFactorAuthenticationEnforcePasswordFallback) {
                    job->setAuthMethod(u"password"_s);
                    job->setAuthCode(QString::fromLatin1(Utils::convertSha256Password(password)));
                }
                job->setTokenName(createDialog->tokenName());
                job->setBypassTwoFactor(createDialog->bypassTwoFactor());

                mRocketChatAccount->restApi()->initializeRestApiJob(job);
                connect(job, &RocketChatRestApi::GeneratePersonalAccessTokenJob::generateTokenDone, this, [this](const QJsonObject &obj) {
                    const QString token = obj["token"_L1].toString();
                    KMessageBox::information(this,
                                             i18n("<qt>Please save your token carefully as you will no longer be able to view it afterwards.<br>"
                                                  "<b>Token:</b> %1<br>"
                                                  "<b>Your user Id:</b> %2</qt>",
                                                  token,
                                                  QString::fromLatin1(mRocketChatAccount->userId())),
                                             i18nc("@title:window", "Personal Token Created"));
                    initialize();
                });
                if (!job->start()) {
                    qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start GeneratePersonalAccessTokenJob job";
                }
            }
        }
    }
    delete createDialog;
}

void MyAccountPersonalAccessTokenConfigureWidget::slotRemoveToken(const QString &tokenName)
{
    if (mRocketChatAccount) {
        QString password;
        const bool twoFactorAuthenticationEnforcePasswordFallback = true; // mRocketChatAccount->twoFactorAuthenticationEnforcePasswordFallback();
        if (twoFactorAuthenticationEnforcePasswordFallback) {
            QPointer<ConfirmPasswordDialog> dialog(new ConfirmPasswordDialog(this));
            if (dialog->exec()) {
                password = dialog->password();
            }
            delete dialog;
        }

        if (!password.isEmpty()) {
            auto job = new RocketChatRestApi::RemovePersonalAccessTokenJob(this);
            job->setTokenName(tokenName);
            if (twoFactorAuthenticationEnforcePasswordFallback) {
                job->setAuthMethod(u"password"_s);
                job->setAuthCode(QString::fromLatin1(Utils::convertSha256Password(password)));
            }

            mRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::RemovePersonalAccessTokenJob::removeTokenDone, this, [this, tokenName]() {
                mPersonalAccessTokenModel->removeToken(tokenName);
                KMessageBox::information(this, i18n("Personal Token removed."), i18nc("@title:window", "Remove Personal Token"));
            });
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start RemovePersonalAccessTokenJob job";
            }
        }
    }
}

void MyAccountPersonalAccessTokenConfigureWidget::slotRegenerateToken(const QString &tokenName)
{
    if (mRocketChatAccount) {
        QString password;
        const bool twoFactorAuthenticationEnforcePasswordFallback = true; // mRocketChatAccount->twoFactorAuthenticationEnforcePasswordFallback();
        if (twoFactorAuthenticationEnforcePasswordFallback) {
            QPointer<ConfirmPasswordDialog> dialog(new ConfirmPasswordDialog(this));
            if (dialog->exec()) {
                password = dialog->password();
            }
            delete dialog;
        }

        if (!password.isEmpty()) {
            auto job = new RocketChatRestApi::RegeneratePersonalAccessTokenJob(this);
            if (twoFactorAuthenticationEnforcePasswordFallback) {
                job->setAuthMethod(u"password"_s);
                job->setAuthCode(QString::fromLatin1(Utils::convertSha256Password(password)));
            }
            job->setTokenName(tokenName);
            mRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::RegeneratePersonalAccessTokenJob::regenerateTokenDone, this, [this](const QJsonObject &obj) {
                const QString token = obj["token"_L1].toString();
                KMessageBox::information(this,
                                         i18n("<qt>Please save your token carefully as you will no longer be able to view it afterwards.<br>"
                                              "<b>Token:</b> %1<br>"
                                              "<b>Your user Id:</b> %2</qt>",
                                              token,
                                              QString::fromLatin1(mRocketChatAccount->userId())),
                                         i18nc("@title:window", "Personal Token Regenerated"));
                initialize();
            });
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start RegeneratePersonalAccessTokenJob job";
            }
        }
    }
}

#include "moc_myaccountpersonalaccesstokenconfigurewidget.cpp"
