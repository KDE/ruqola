/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorydialog.h"
#include "directorytabwidget.h"
#include "rocketchataccount.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace
{
const char myDirectoryDialog[] = "DirectoryDialog";
}
DirectoryDialog::DirectoryDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mDirectoryTabWidget(new DirectoryTabWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Directory - %1", account->accountName()));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mDirectoryTabWidget->setObjectName(QStringLiteral("mDirectoryTabWidget"));
    mainLayout->addWidget(mDirectoryTabWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &DirectoryDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &DirectoryDialog::accept);

    readConfig();
    setAttribute(Qt::WA_DeleteOnClose);
}

DirectoryDialog::~DirectoryDialog()
{
    writeConfig();
}

void DirectoryDialog::fillTabs()
{
    mDirectoryTabWidget->fillTabs();
}

void DirectoryDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myDirectoryDialog);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void DirectoryDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myDirectoryDialog);
    group.writeEntry("Size", size());
}
