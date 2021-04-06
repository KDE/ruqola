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

#include "threadmessagedialog.h"
#include "threadmessagewidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace
{
static const char myThreadMessageDialogGroupName[] = "ThreadMessageDialog";
}

ThreadMessageDialog::ThreadMessageDialog(QWidget *parent)
    : QDialog(parent)
    , mThreadMessageWidget(new ThreadMessageWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Threads")); // TODO fixme name
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mThreadMessageWidget->setObjectName(QStringLiteral("mThreadMessageWidget"));
    mainLayout->addWidget(mThreadMessageWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ThreadMessageDialog::reject);
    readConfig();
}

ThreadMessageDialog::~ThreadMessageDialog()
{
    writeConfig();
}

void ThreadMessageDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myThreadMessageDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ThreadMessageDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myThreadMessageDialogGroupName);
    group.writeEntry("Size", size());
}

QString ThreadMessageDialog::threadMessageId() const
{
    return mThreadMessageWidget->threadMessageId();
}

void ThreadMessageDialog::setThreadMessageId(const QString &threadMessageId)
{
    mThreadMessageWidget->setThreadMessageId(threadMessageId);
}

void ThreadMessageDialog::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mThreadMessageWidget->setCurrentRocketChatAccount(account);
}

void ThreadMessageDialog::setRoom(Room *room)
{
    mThreadMessageWidget->setRoom(room);
}

void ThreadMessageDialog::setThreadPreview(const QString &preview)
{
    mThreadMessageWidget->setThreadPreview(preview);
}
