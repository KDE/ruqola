/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorydialog.h"
using namespace Qt::Literals::StringLiterals;

#include "directorycontainerwidget.h"
#include "rocketchataccount.h"

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
const char myDirectoryDialog[] = "DirectoryDialog";
}
DirectoryDialog::DirectoryDialog(RocketChatAccount *account, DirectoryWidget::DirectoryType type, QWidget *parent)
    : QDialog(parent)
    , mDirectoryContainerWidget(new DirectoryContainerWidget(account, type, this))
{
    switch (type) {
    case DirectoryWidget::DirectoryType::Room:
        setWindowTitle(i18nc("@title:window", "Join Room - %1", account ? account->accountName() : u"account"_s));
        break;
    case DirectoryWidget::DirectoryType::User:
        setWindowTitle(i18nc("@title:window", "Open Private Conversation - %1", account ? account->accountName() : u"account"_s));
        break;
    case DirectoryWidget::DirectoryType::Team:
        setWindowTitle(i18nc("@title:window", "Join Team - %1", account ? account->accountName() : u"account"_s));
        break;
    case DirectoryWidget::DirectoryType::Unknown:
        Q_UNREACHABLE();
        break;
    }

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mDirectoryContainerWidget->setObjectName(u"mDirectoryContainerWidget"_s);
    mainLayout->addWidget(mDirectoryContainerWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &DirectoryDialog::reject);

    auto openButton = new QPushButton(this);
    button->addButton(openButton, QDialogButtonBox::ActionRole);
    openButton->setEnabled(false);
    switch (type) {
    case DirectoryWidget::DirectoryType::Room:
    case DirectoryWidget::DirectoryType::Team:
        openButton->setText(i18nc("@action:button", "Join"));
        break;
    case DirectoryWidget::DirectoryType::User:
        openButton->setText(i18nc("@action:button", "Open Private Message"));
        break;
    case DirectoryWidget::DirectoryType::Unknown:
        Q_UNREACHABLE();
        break;
    }
    connect(openButton, &QPushButton::clicked, mDirectoryContainerWidget, &DirectoryContainerWidget::slotJoin);

    connect(mDirectoryContainerWidget, &DirectoryContainerWidget::updateJoinButton, openButton, &QPushButton::setEnabled);
    readConfig();
    setAttribute(Qt::WA_DeleteOnClose);
}

DirectoryDialog::~DirectoryDialog()
{
    writeConfig();
}

void DirectoryDialog::readConfig()
{
    create(); // ensure a window is created
    resize(QSize(1024, 768));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myDirectoryDialog));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void DirectoryDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myDirectoryDialog));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_directorydialog.cpp"
