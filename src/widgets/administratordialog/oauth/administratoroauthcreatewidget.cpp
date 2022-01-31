/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthcreatewidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

AdministratorOauthCreateWidget::AdministratorOauthCreateWidget(QWidget *parent)
    : QWidget{parent}
    , mActiveCheckBox(new QCheckBox(i18n("Active"), this))
    , mApplicationName(new QLineEdit(this))
    , mRedirectUrl(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mActiveCheckBox->setObjectName(QStringLiteral("mActiveCheckBox"));
    mainLayout->addWidget(mActiveCheckBox);

    mApplicationName->setObjectName(QStringLiteral("mApplicationName"));
    mainLayout->addWidget(mApplicationName);

    mRedirectUrl->setObjectName(QStringLiteral("mRedirectUrl"));
    mainLayout->addWidget(mRedirectUrl);
}

AdministratorOauthCreateWidget::~AdministratorOauthCreateWidget()
{
}
