/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroautheditdialog.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
const char myConfigAdministratorOauthEditDialogGroupName[] = "AdministratorOauthEditDialog";
}
using namespace Qt::Literals::StringLiterals;
AdministratorOauthEditDialog::AdministratorOauthEditDialog(QWidget *parent)
    : QDialog(parent)
    , mOauthEditWidget(new AdministratorOauthEditWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Edit Oauth Apps"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mOauthEditWidget->setObjectName(u"mOauthEditWidget"_s);
    mainLayout->addWidget(mOauthEditWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);

    mOkButton = button->button(QDialogButtonBox::Ok);
    mOkButton->setEnabled(false);
    connect(mOauthEditWidget, &AdministratorOauthEditWidget::enableOkButton, mOkButton, &QPushButton::setEnabled);

    connect(button, &QDialogButtonBox::rejected, this, &AdministratorOauthEditDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorOauthEditDialog::accept);
    readConfig();
}

AdministratorOauthEditDialog::~AdministratorOauthEditDialog() = default;

AdministratorOauthEditWidget::OauthEditInfo AdministratorOauthEditDialog::oauthInfo() const
{
    return mOauthEditWidget->oauthInfo();
}

void AdministratorOauthEditDialog::setOauthInfo(const AdministratorOauthEditWidget::OauthEditInfo &info)
{
    mOauthEditWidget->setOauthInfo(info);
}

void AdministratorOauthEditDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myConfigAdministratorOauthEditDialogGroupName), QSize(800, 300));
}

#include "moc_administratoroautheditdialog.cpp"
