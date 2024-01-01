/*
   SPDX-FileCopyrightText: 2021-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorydialog.h"
#include "directorytabwidget.h"
#include "rocketchataccount.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
const char myDirectoryDialog[] = "DirectoryDialog";
}
DirectoryDialog::DirectoryDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mDirectoryTabWidget(new DirectoryTabWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Directory - %1", account ? account->accountName() : QStringLiteral("account")));
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
    create(); // ensure a window is created
    resize(QSize(1024, 768));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myDirectoryDialog));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void DirectoryDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myDirectoryDialog));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_directorydialog.cpp"
