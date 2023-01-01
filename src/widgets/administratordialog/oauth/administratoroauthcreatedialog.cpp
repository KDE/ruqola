/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthcreatedialog.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
static const char myConfigAdministratorOauthCreateDialogGroupName[] = "AdministratorOauthCreateDialog";
}
AdministratorOauthCreateDialog::AdministratorOauthCreateDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateWidget(new AdministratorOauthCreateWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Add Oauth Apps"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateWidget->setObjectName(QStringLiteral("mCreateWidget"));
    mainLayout->addWidget(mCreateWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorOauthCreateDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorOauthCreateDialog::accept);
    mOkButton = button->button(QDialogButtonBox::Ok);
    mOkButton->setObjectName(QStringLiteral("mOkButton"));
    mOkButton->setEnabled(false);
    connect(mCreateWidget, &AdministratorOauthCreateWidget::enableOkButton, mOkButton, &QPushButton::setEnabled);
    readConfig();
}

AdministratorOauthCreateDialog::~AdministratorOauthCreateDialog()
{
    writeConfig();
}

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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAdministratorOauthCreateDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void AdministratorOauthCreateDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAdministratorOauthCreateDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}
