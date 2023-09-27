/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagehistorydialog.h"
#include "servererrorinfomessagehistorywidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
const char myServerErrorInfoMessageHistoryDialogConfigGroupName[] = "ServerErrorInfoMessageHistoryDialog";
}
ServerErrorInfoMessageHistoryDialog::ServerErrorInfoMessageHistoryDialog(QWidget *parent)
    : QDialog(parent)
    , mServerErrorInfoMessageHistoryWidget(new ServerErrorInfoMessageHistoryWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Server Error Information"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mServerErrorInfoMessageHistoryWidget->setObjectName(QStringLiteral("mServerErrorInfoMessageHistoryWidget"));
    mainLayout->addWidget(mServerErrorInfoMessageHistoryWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ServerErrorInfoMessageHistoryDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &ServerErrorInfoMessageHistoryDialog::accept);
    readConfig();
}

ServerErrorInfoMessageHistoryDialog::~ServerErrorInfoMessageHistoryDialog()
{
    writeConfig();
}

void ServerErrorInfoMessageHistoryDialog::addServerList(const QStringList &serverNames)
{
    mServerErrorInfoMessageHistoryWidget->addServerList(serverNames);
}

void ServerErrorInfoMessageHistoryDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), myServerErrorInfoMessageHistoryDialogConfigGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ServerErrorInfoMessageHistoryDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myServerErrorInfoMessageHistoryDialogConfigGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_servererrorinfomessagehistorydialog.cpp"
