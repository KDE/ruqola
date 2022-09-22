/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showattachmentdialog.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "showattachmentwidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
namespace
{
static const char myShowAttachmentDialogGroupName[] = "ShowAttachmentDialog";
}

ShowAttachmentDialog::ShowAttachmentDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mShowAttachmentWidget(new ShowAttachmentWidget(account, this))
    , mRocketChatAccount(account)
{
    setWindowTitle(i18nc("@title:window", "Show Attachments - %1", account ? account->accountName() : QStringLiteral("account")));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mShowAttachmentWidget->setObjectName(QStringLiteral("mShowAttachmentWidget"));
    mainLayout->addWidget(mShowAttachmentWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ShowAttachmentDialog::reject);
    connect(mShowAttachmentWidget, &ShowAttachmentWidget::loadMoreFileAttachment, this, &ShowAttachmentDialog::slotLoadMoreAttachment);
    connect(mShowAttachmentWidget, &ShowAttachmentWidget::deleteAttachment, this, &ShowAttachmentDialog::slotDeleteAttachment);
    readConfig();
}

ShowAttachmentDialog::~ShowAttachmentDialog()
{
    writeConfig();
}

void ShowAttachmentDialog::slotDeleteAttachment(const QString &fileId)
{
    mRocketChatAccount->deleteFileMessage(mRoomId, fileId, mRoomType);
}

void ShowAttachmentDialog::setModel(FilesForRoomFilterProxyModel *model)
{
    mShowAttachmentWidget->setModel(model);
}

void ShowAttachmentDialog::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

QString ShowAttachmentDialog::roomId() const
{
    return mRoomId;
}

void ShowAttachmentDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowAttachmentDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ShowAttachmentDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowAttachmentDialogGroupName);
    group.writeEntry("Size", size());
}

void ShowAttachmentDialog::slotLoadMoreAttachment()
{
    if (mRoomId.isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "RoomId is empty. It's a bug";
        return;
    }
    mRocketChatAccount->loadMoreFileAttachments(mRoomId, mRoomType);
}

Room::RoomType ShowAttachmentDialog::roomType() const
{
    return mRoomType;
}

void ShowAttachmentDialog::setRoomType(Room::RoomType roomType)
{
    mRoomType = roomType;
}
