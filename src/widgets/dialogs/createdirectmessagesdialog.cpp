/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createdirectmessagesdialog.h"
#include "createdirectmessageswidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>

#include <KConfigGroup>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char myConfigCreateDirectMessagesDialogGroupName[] = "CreateDirectMessagesDialog";
}
CreateDirectMessagesDialog::CreateDirectMessagesDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mCreateDirectMessagesWidget(new CreateDirectMessagesWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Create Direct Messages - %1", account ? account->accountName() : QStringLiteral("account")));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateDirectMessagesWidget->setObjectName(QStringLiteral("mCreateDirectMessagesWidget"));
    mainLayout->addWidget(mCreateDirectMessagesWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CreateDirectMessagesDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &CreateDirectMessagesDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
    auto okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    connect(mCreateDirectMessagesWidget, &CreateDirectMessagesWidget::updateOkButton, this, [okButton](bool state) {
        okButton->setEnabled(state);
    });
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CreateDirectMessagesDialog::slotAccepted);
}
CreateDirectMessagesDialog::~CreateDirectMessagesDialog()
{
    writeConfig();
}

void CreateDirectMessagesDialog::slotAccepted()
{
    const QStringList usernames = userNames();
    mCreateDirectMessagesWidget->rocketChatAccount()->createDirectMessages(usernames);
}

QStringList CreateDirectMessagesDialog::userNames() const
{
    return mCreateDirectMessagesWidget->userNames();
}

void CreateDirectMessagesDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myConfigCreateDirectMessagesDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void CreateDirectMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myConfigCreateDirectMessagesDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_createdirectmessagesdialog.cpp"
