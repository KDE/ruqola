/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "prunemessagesdialog.h"
#include "prunemessageswidget.h"

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
const char myPruneMessagesDialogConfigGroupName[] = "PruneMessagesDialog";
}

PruneMessagesDialog::PruneMessagesDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mPruneMessageWidget(new PruneMessagesWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Prune Messages"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mPruneMessageWidget->setObjectName(QStringLiteral("mPruneMessageWidget"));
    mainLayout->addWidget(mPruneMessageWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &PruneMessagesDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &PruneMessagesDialog::accept);

    QPushButton *okButton = button->button(QDialogButtonBox::Ok);
    okButton->setText(i18n("Prune"));
    okButton->setEnabled(false);
    connect(mPruneMessageWidget, &PruneMessagesWidget::updateOkButton, okButton, &QPushButton::setEnabled);
    readConfig();
}

PruneMessagesDialog::~PruneMessagesDialog()
{
    writeConfig();
}

RocketChatRestApi::RoomsCleanHistoryJob::CleanHistoryInfo PruneMessagesDialog::cleanHistoryInfo() const
{
    return mPruneMessageWidget->cleanHistoryInfo();
}

void PruneMessagesDialog::setRoomName(const QString &roomName)
{
    mPruneMessageWidget->setRoomName(roomName);
}

void PruneMessagesDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myPruneMessagesDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void PruneMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myPruneMessagesDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_prunemessagesdialog.cpp"
