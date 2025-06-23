/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagehistorydialog.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);

    mServerErrorInfoMessageHistoryWidget->setObjectName(u"mServerErrorInfoMessageHistoryWidget"_s);
    mainLayout->addWidget(mServerErrorInfoMessageHistoryWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ServerErrorInfoMessageHistoryDialog::reject);
    readConfig();
}

ServerErrorInfoMessageHistoryDialog::~ServerErrorInfoMessageHistoryDialog()
{
    writeConfig();
}

void ServerErrorInfoMessageHistoryDialog::addServerList(const QList<AccountManager::AccountDisplayInfo> &infos)
{
    mServerErrorInfoMessageHistoryWidget->addServerList(infos);
}

void ServerErrorInfoMessageHistoryDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myServerErrorInfoMessageHistoryDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ServerErrorInfoMessageHistoryDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myServerErrorInfoMessageHistoryDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_servererrorinfomessagehistorydialog.cpp"
