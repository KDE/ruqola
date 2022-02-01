/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroautheditwidget.h"
#include <KLocalizedString>
#include <QVBoxLayout>

AdministratorOauthEditWidget::AdministratorOauthEditWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
}

AdministratorOauthEditWidget::~AdministratorOauthEditWidget()
{
}

AdministratorOauthEditWidget::OauthEditInfo AdministratorOauthEditWidget::oauthInfo() const
{
    // TODO
    return {};
}

void AdministratorOauthEditWidget::setOauthInfo(const OauthEditInfo &info)
{
    // TODO
}
