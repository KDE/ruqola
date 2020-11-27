/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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

#include "showattachmentdialog.h"
#include "showattachmentwidget.h"
#include "ruqolawidgets_debug.h"
#include "ruqola.h"
#include "rocketchataccount.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <KLocalizedString>
namespace {
static const char myConfigGroupName[] = "ShowAttachmentDialog";
}

ShowAttachmentDialog::ShowAttachmentDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Show Attachments"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mShowAttachmentWidget = new ShowAttachmentWidget(this);
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
    Ruqola::self()->rocketChatAccount()->deleteFileMessage(mRoomId, fileId, mRoomType);
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
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ShowAttachmentDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}

void ShowAttachmentDialog::slotLoadMoreAttachment()
{
    if (mRoomId.isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "RoomId is empty. It's a bug";
        return;
    }
    Ruqola::self()->rocketChatAccount()->loadMoreFileAttachments(mRoomId, mRoomType);
}

QString ShowAttachmentDialog::roomType() const
{
    return mRoomType;
}

void ShowAttachmentDialog::setRoomType(const QString &roomType)
{
    mRoomType = roomType;
}
