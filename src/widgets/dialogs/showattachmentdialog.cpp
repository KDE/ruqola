/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showattachmentdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "attachments/fileattachments.h"
#include "connection.h"
#include "dialogs/showimagedialog.h"
#include "misc/methodcalljob.h"
#include "rocketchataccount.h"
#include "rooms/roomsimagesjob.h"
#include "ruqolawidgets_debug.h"
#include "showattachmentwidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
const char myShowAttachmentDialogGroupName[] = "ShowAttachmentDialog";
}

ShowAttachmentDialog::ShowAttachmentDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mShowAttachmentWidget(new ShowAttachmentWidget(account, this))
    , mRocketChatAccount(account)
{
    setWindowTitle(i18nc("@title:window", "Show Attachments - %1", account ? account->accountName() : u"account"_s));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    setAttribute(Qt::WA_DeleteOnClose);

    mShowAttachmentWidget->setObjectName(u"mShowAttachmentWidget"_s);
    mainLayout->addWidget(mShowAttachmentWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ShowAttachmentDialog::reject);
    connect(mShowAttachmentWidget, &ShowAttachmentWidget::loadMoreFileAttachment, this, &ShowAttachmentDialog::slotLoadMoreAttachment);
    connect(mShowAttachmentWidget, &ShowAttachmentWidget::deleteAttachment, this, &ShowAttachmentDialog::slotDeleteAttachment);
    connect(mShowAttachmentWidget, &ShowAttachmentWidget::showImage, this, &ShowAttachmentDialog::slotShowImage);
    readConfig();
}

ShowAttachmentDialog::~ShowAttachmentDialog()
{
    writeConfig();
}

void ShowAttachmentDialog::slotShowImage(const QByteArray &fileId)
{
    auto dlg = new ShowImageDialog(mRocketChatAccount, this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->showImages(fileId, mRoomId);
    dlg->show();
}

void ShowAttachmentDialog::slotDeleteAttachment(const QByteArray &fileId)
{
    auto job = new RocketChatRestApi::MethodCallJob(this);
    RocketChatRestApi::MethodCallJob::MethodCallJobInfo info;
    info.methodName = u"deleteFileMessage"_s;
    info.anonymous = false;
    const QJsonArray params{{QString::fromLatin1(fileId)}};
    info.messageObj = mRocketChatAccount->ddp()->generateJsonObject(info.methodName, params);
    job->setMethodCallJobInfo(std::move(info));
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::MethodCallJob::methodCallDone, this, [this](const QJsonObject &replyObject) {
        Q_UNUSED(replyObject);
        mRocketChatAccount->roomFiles(mRoomId, mRoomType);
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start getRoomByTypeAndName job";
    }
}

void ShowAttachmentDialog::setModel(FilesForRoomFilterProxyModel *model)
{
    mShowAttachmentWidget->setModel(model);
}

void ShowAttachmentDialog::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

QByteArray ShowAttachmentDialog::roomId() const
{
    return mRoomId;
}

void ShowAttachmentDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowAttachmentDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowAttachmentDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowAttachmentDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
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

#include "moc_showattachmentdialog.cpp"
