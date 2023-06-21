/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverdialog.h"
#include "createnewserverwidget.h"
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
static const char myConfigCreateNewAccountDialogGroupName[] = "CreateNewAccountDialog";
}

CreateNewServerDialog::CreateNewServerDialog(QWidget *parent)
    : QDialog(parent)
    , mNewAccountWidget(new CreateNewServerWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Add Server"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mNewAccountWidget->setObjectName(QStringLiteral("mNewAccountWidget"));
    mainLayout->addWidget(mNewAccountWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CreateNewServerDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &CreateNewServerDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setEnabled(false);
    connect(mNewAccountWidget, &CreateNewServerWidget::updateOkButton, this, [this](bool state) {
        mOkButton->setEnabled(state);
    });
}

CreateNewServerDialog::~CreateNewServerDialog()
{
    writeConfig();
}

AccountManager::AccountManagerInfo CreateNewServerDialog::accountInfo() const
{
    const AccountManager::AccountManagerInfo info = mNewAccountWidget->accountInfo();
    return info;
}

void CreateNewServerDialog::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    setWindowTitle(i18nc("@title:window", "Modify Account"));
    mNewAccountWidget->setAccountInfo(info);
}

void CreateNewServerDialog::setExistingAccountName(const QStringList &lst)
{
    mNewAccountWidget->setExistingAccountName(lst);
}

void CreateNewServerDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigCreateNewAccountDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void CreateNewServerDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigCreateNewAccountDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_createnewserverdialog.cpp"
