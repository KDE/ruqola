/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroautheditdialog.h"

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
static const char myConfigAdministratorOauthEditDialogGroupName[] = "AdministratorOauthEditDialog";
}
AdministratorOauthEditDialog::AdministratorOauthEditDialog(QWidget *parent)
    : QDialog(parent)
    , mOauthEditWidget(new AdministratorOauthEditWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Edit Oauth Apps"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mOauthEditWidget->setObjectName(QStringLiteral("mOauthEditWidget"));
    mainLayout->addWidget(mOauthEditWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);

    mOkButton = button->button(QDialogButtonBox::Ok);
    mOkButton->setEnabled(false);
    connect(mOauthEditWidget, &AdministratorOauthEditWidget::enableOkButton, mOkButton, &QPushButton::setEnabled);

    connect(button, &QDialogButtonBox::rejected, this, &AdministratorOauthEditDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorOauthEditDialog::accept);
    readConfig();
}

AdministratorOauthEditDialog::~AdministratorOauthEditDialog()
{
    writeConfig();
}

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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAdministratorOauthEditDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void AdministratorOauthEditDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAdministratorOauthEditDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_administratoroautheditdialog.cpp"
