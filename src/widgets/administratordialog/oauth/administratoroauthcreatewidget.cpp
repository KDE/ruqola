/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthcreatewidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

AdministratorOauthCreateWidget::AdministratorOauthCreateWidget(QWidget *parent)
    : QWidget{parent}
    , mActiveCheckBox(new QCheckBox(i18n("Active"), this))
    , mApplicationName(new QLineEdit(this))
    , mRedirectUrl(new QLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mActiveCheckBox->setObjectName(QStringLiteral("mActiveCheckBox"));
    mainLayout->addWidget(mActiveCheckBox);
    mActiveCheckBox->setChecked(false);

    mApplicationName->setObjectName(QStringLiteral("mApplicationName"));
    mainLayout->addRow(i18n("Name:"), mApplicationName);

    mRedirectUrl->setObjectName(QStringLiteral("mRedirectUrl"));
    mainLayout->addRow(i18n("Redirect Url:"), mRedirectUrl);
}

AdministratorOauthCreateWidget::~AdministratorOauthCreateWidget()
{
}

AdministratorOauthCreateWidget::OauthCreateInfo AdministratorOauthCreateWidget::oauthInfo() const
{
    AdministratorOauthCreateWidget::OauthCreateInfo info;
    info.active = mActiveCheckBox->isChecked();
    info.applicationName = mApplicationName->text().trimmed();
    info.redirectUrl = mRedirectUrl->text().trimmed();
    return info;
}

void AdministratorOauthCreateWidget::setOauthInfo(const OauthCreateInfo &info)
{
    mActiveCheckBox->setChecked(info.active);
    mApplicationName->setText(info.applicationName);
    mRedirectUrl->setText(info.redirectUrl);
}
