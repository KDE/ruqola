/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountpersonalaccesstokenconfigurewidget.h"
#include "model/personalaccesstokeninfosmodel.h"
#include "personalaccesstokentreeview.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QLineEdit>
#include <QVBoxLayout>

MyAccountPersonalAccessTokenConfigureWidget::MyAccountPersonalAccessTokenConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mSearchLineWidget(new QLineEdit(this))
    , mPersonalAccessTokenTreeView(new PersonalAccessTokenTreeView(account, this))
    , mPersonalAccessTokenModel(new PersonalAccessTokenInfosModel(this))

{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchLineWidget->setObjectName(QStringLiteral("mSearchLineWidget"));
    mainLayout->addWidget(mSearchLineWidget);
    mSearchLineWidget->setPlaceholderText(i18n("Search Personal Access Token..."));
    mPersonalAccessTokenTreeView->setObjectName(QStringLiteral("mPersonalAccessTokenTreeView"));

    mPersonalAccessTokenModel->setObjectName(QStringLiteral("mPersonalAccessTokenModel"));
}

MyAccountPersonalAccessTokenConfigureWidget::~MyAccountPersonalAccessTokenConfigureWidget() = default;
