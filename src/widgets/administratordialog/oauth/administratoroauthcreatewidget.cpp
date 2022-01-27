/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthcreatewidget.h"
#include <KLocalizedString>
#include <QVBoxLayout>

AdministratorOauthCreateWidget::AdministratorOauthCreateWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
}

AdministratorOauthCreateWidget::~AdministratorOauthCreateWidget()
{
}
