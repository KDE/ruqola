/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploredatabasedialog.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
const char myExploreDatabaseDialogConfigGroupName[] = "ExploreDatabaseDialog";
}
ExploreDatabaseDialog::ExploreDatabaseDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Database Info"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    //    mServerErrorInfoMessageHistoryWidget->setObjectName(QStringLiteral("mServerErrorInfoMessageHistoryWidget"));
    //    mainLayout->addWidget(mServerErrorInfoMessageHistoryWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ExploreDatabaseDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ExploreDatabaseDialog::accept);

    readConfig();
}

ExploreDatabaseDialog::~ExploreDatabaseDialog()
{
    writeConfig();
}

void ExploreDatabaseDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), myExploreDatabaseDialogConfigGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ExploreDatabaseDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myExploreDatabaseDialogConfigGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}
