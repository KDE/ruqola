/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthcreatedialog.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>
namespace
{
const char myConfigAdministratorOauthCreateDialogGroupName[] = "AdministratorOauthCreateDialog";
}
using namespace Qt::Literals::StringLiterals;
AdministratorOauthCreateDialog::AdministratorOauthCreateDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateWidget(new AdministratorOauthCreateWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Add Oauth Apps"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mCreateWidget->setObjectName(u"mCreateWidget"_s);
    mainLayout->addWidget(mCreateWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorOauthCreateDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorOauthCreateDialog::accept);
    mOkButton = button->button(QDialogButtonBox::Ok);
    mOkButton->setObjectName(u"mOkButton"_s);
    mOkButton->setEnabled(false);
    connect(mCreateWidget, &AdministratorOauthCreateWidget::enableOkButton, mOkButton, &QPushButton::setEnabled);
    readConfig();
}

AdministratorOauthCreateDialog::~AdministratorOauthCreateDialog() = default;

AdministratorOauthCreateWidget::OauthCreateInfo AdministratorOauthCreateDialog::oauthInfo() const
{
    return mCreateWidget->oauthInfo();
}

void AdministratorOauthCreateDialog::setOauthInfo(const AdministratorOauthCreateWidget::OauthCreateInfo &info)
{
    mCreateWidget->setOauthInfo(info);
}

void AdministratorOauthCreateDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myConfigAdministratorOauthCreateDialogGroupName), QSize(800, 300));
}

#include "moc_administratoroauthcreatedialog.cpp"
