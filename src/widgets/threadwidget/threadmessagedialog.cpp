/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

ThreadMessageDialog::ThreadMessageDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mThreadMessageWidget(new ThreadMessageWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Thread"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mThreadMessageWidget->setObjectName(QStringLiteral("mThreadMessageWidget"));
    mainLayout->addWidget(mThreadMessageWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ThreadMessageDialog::reject);
    readConfig();
    setAttribute(Qt::WA_DeleteOnClose);
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

void ThreadMessageDialog::setRoom(Room *room)
{
    mThreadMessageWidget->setRoom(room);
}

void ThreadMessageDialog::setThreadPreview(const QString &preview)
{
    mThreadMessageWidget->setThreadPreview(preview);
}
