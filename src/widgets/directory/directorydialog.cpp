/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "directorydialog.h"
#include "directorytabwidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace
{
const char myDirectoryDialog[] = "DirectoryDialog";
}
DirectoryDialog::DirectoryDialog(QWidget *parent)
    : QDialog(parent)
    , mDirectoryTabWidget(new DirectoryTabWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Directory"));
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
