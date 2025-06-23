/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthcreatewidget.h"
using namespace Qt::Literals::StringLiterals;

#include <KLocalizedString>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>

AdministratorOauthCreateWidget::AdministratorOauthCreateWidget(QWidget *parent)
    : QWidget{parent}
    , mActiveCheckBox(new QCheckBox(i18nc("@option:check", "Active"), this))
    , mApplicationName(new QLineEdit(this))
    , mRedirectUrl(new QLineEdit(this))
{
    auto mainLayout = new QFormLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mActiveCheckBox->setObjectName(u"mActiveCheckBox"_s);
    mainLayout->addWidget(mActiveCheckBox);
    mActiveCheckBox->setChecked(false);

    mApplicationName->setObjectName(u"mApplicationName"_s);
    mainLayout->addRow(i18n("Name:"), mApplicationName);

    mRedirectUrl->setObjectName(u"mRedirectUrl"_s);
    mainLayout->addRow(i18n("Redirect Url:"), mRedirectUrl);
    connect(mApplicationName, &QLineEdit::textEdited, this, &AdministratorOauthCreateWidget::slotTextChanged);
    connect(mRedirectUrl, &QLineEdit::textEdited, this, &AdministratorOauthCreateWidget::slotTextChanged);
}

AdministratorOauthCreateWidget::~AdministratorOauthCreateWidget() = default;

void AdministratorOauthCreateWidget::slotTextChanged()
{
    Q_EMIT enableOkButton(!mRedirectUrl->text().trimmed().isEmpty() && !mApplicationName->text().trimmed().isEmpty());
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

QDebug operator<<(QDebug d, const AdministratorOauthCreateWidget::OauthCreateInfo &info)
{
    d.space() << "active : " << info.active;
    d.space() << "applicationName : " << info.applicationName;
    d.space() << "redirectUrl : " << info.redirectUrl;
    return d;
}

#include "moc_administratoroauthcreatewidget.cpp"
