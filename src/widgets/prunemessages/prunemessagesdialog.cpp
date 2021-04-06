/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
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

PruneMessagesDialog::PruneMessagesDialog(QWidget *parent)
    : QDialog(parent)
    , mPruneMessageWidget(new PruneMessagesWidget(this))
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
