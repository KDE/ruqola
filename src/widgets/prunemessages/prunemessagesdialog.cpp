/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "prunemessagesdialog.h"
#include "prunemessageswidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
const char myConfigGroupName[] = "PruneMessagesDialog";
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
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void PruneMessagesDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}
