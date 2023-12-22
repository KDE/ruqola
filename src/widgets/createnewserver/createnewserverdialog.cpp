/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverdialog.h"
#include "createnewserver/createnewserverstackwidget.h"
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
    , mCreateNewServerStackWidget(new CreateNewServerStackWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Add Server"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateNewServerStackWidget->setObjectName(QStringLiteral("mCreateNewServerStackWidget"));
    mainLayout->addWidget(mCreateNewServerStackWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CreateNewServerDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &CreateNewServerDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setEnabled(false);
    connect(mCreateNewServerStackWidget, &CreateNewServerStackWidget::updateOkButton, this, [this](bool state) {
        mOkButton->setEnabled(state);
    });
    connect(mCreateNewServerStackWidget, &CreateNewServerStackWidget::authentication, this, &CreateNewServerDialog::authentication);
}

CreateNewServerDialog::~CreateNewServerDialog()
{
    writeConfig();
}

AccountManager::AccountManagerInfo CreateNewServerDialog::accountInfo() const
{
    const AccountManager::AccountManagerInfo info = mCreateNewServerStackWidget->accountInfo();
    return info;
}

void CreateNewServerDialog::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    setWindowTitle(i18nc("@title:window", "Modify Account"));
    mCreateNewServerStackWidget->setAccountInfo(info);
}

void CreateNewServerDialog::setExistingAccountName(const QStringList &lst)
{
    mCreateNewServerStackWidget->setExistingAccountName(lst);
}

void CreateNewServerDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myConfigCreateNewAccountDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void CreateNewServerDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myConfigCreateNewAccountDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_createnewserverdialog.cpp"
