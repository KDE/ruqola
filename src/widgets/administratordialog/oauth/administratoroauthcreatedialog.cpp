/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratoroauthcreatedialog.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
namespace
{
static const char myConfigAdministratorOauthCreateDialogGroupName[] = "AdministratorOauthCreateDialog";
}
AdministratorOauthCreateDialog::AdministratorOauthCreateDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Add Oauth Apps"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    //    mCreateWidget->setObjectName(QStringLiteral("mCreateWidget"));
    //    mainLayout->addWidget(mCreateWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorOauthCreateDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorOauthCreateDialog::accept);
    //    mOkButton = button->button(QDialogButtonBox::Ok);
    //    mOkButton->setEnabled(false);
    //    connect(mCreateWidget, &AdministratorCustomUserStatusCreateWidget::updateOkButton, mOkButton, &QPushButton::setEnabled);
    readConfig();
}

AdministratorOauthCreateDialog::~AdministratorOauthCreateDialog()
{
    writeConfig();
}

void AdministratorOauthCreateDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAdministratorOauthCreateDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void AdministratorOauthCreateDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAdministratorOauthCreateDialogGroupName);
    group.writeEntry("Size", size());
}
