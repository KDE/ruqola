/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportedmessageconsoletreewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/moderationconsole/moderationmessagesdialog.h"
#include "connection.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/commonmessagefilterproxymodel.h"
#include "model/moderationmessagesmodel.h"
#include "model/moderationreportedmessagemodel.h"
#include "model/moderationreportedmessageproxymodel.h"
#include "model/searchtreebasefilterproxymodel.h"
#include "moderation/moderationdismissreportsjob.h"
#include "moderation/moderationreportsbyusersjob.h"
#include "moderation/moderationuserdeletereportedmessagesjob.h"
#include "moderation/moderationuserreportedmessagesjob.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "users/setuseractivestatusjob.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QLabel>
#include <QMenu>
#include <QTreeView>

ModerationReportedMessageConsoleTreeWidget::ModerationReportedMessageConsoleTreeWidget(RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
    , mCommonMessagesModel(new ModerationMessagesModel(account, this))
{
    mCommonMessageFilterProxyModel = new CommonMessageFilterProxyModel(mCommonMessagesModel, this);
    mModel = new ModerationReportedMessageModel(this);
    mModel->setObjectName(u"mModel"_s);
    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search moderation message…"));

    mProxyModelModel = new ModerationReportedMessageProxyModel(mModel, this);
    mProxyModelModel->setObjectName(u"mProxyModelModel"_s);
    mTreeView->setModel(mProxyModelModel);
    connect(this, &ModerationReportedMessageConsoleTreeWidget::doubleClicked, this, &ModerationReportedMessageConsoleTreeWidget::slotShowMessages);
    connect(this, &ModerationReportedMessageConsoleTreeWidget::refreshList, this, [this]() {
        slotLoadElements();
    });
    hideColumns();
    connectModel();
}

ModerationReportedMessageConsoleTreeWidget::~ModerationReportedMessageConsoleTreeWidget() = default;

void ModerationReportedMessageConsoleTreeWidget::setModerationRanges(const AdministratorModerationRangeWidget::DateTimeRange &range)
{
    mModerationRanges = range;
    slotLoadElements();
    // qDebug() << " range " << range;
}

void ModerationReportedMessageConsoleTreeWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? i18n("No moderation message found") : displayShowMessage());
}

QString ModerationReportedMessageConsoleTreeWidget::displayShowMessage() const
{
    QString displayMessageStr = i18np("%1 moderation message (Total: %2)", "%1 moderation messages (Total: %2)", mModel->rowCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

void ModerationReportedMessageConsoleTreeWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    auto job = new RocketChatRestApi::ModerationReportsByUsersJob(this);

    RocketChatRestApi::ModerationReportsByUsersJob::ModerationReportsByUsersInfo info;
    info.mOldest = mModerationRanges.fromDate;
    info.mLatest = mModerationRanges.toDate;
    info.mSelector = searchName;
    RocketChatRestApi::QueryParameters parameters;
    if (offset != -1) {
        parameters.setOffset(offset);
    }
    if (count != -1) {
        parameters.setCount(count);
    }

    job->setQueryParameters(parameters);
    if (info.isValid()) {
        job->setModerationReportsByUsersInfo(info);
    }

    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    if (offset != -1) {
        connect(job,
                &RocketChatRestApi::ModerationReportsByUsersJob::moderationReportByUserDone,
                this,
                &ModerationReportedMessageConsoleTreeWidget::slotLoadMoreElementDone);
    } else {
        connect(job,
                &RocketChatRestApi::ModerationReportsByUsersJob::moderationReportByUserDone,
                this,
                &ModerationReportedMessageConsoleTreeWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ModerationReportsByUsersJob job";
    }
}

void ModerationReportedMessageConsoleTreeWidget::slotShowMessages(const QModelIndex &newModelIndex)
{
    if (!newModelIndex.isValid()) {
        return;
    }
    auto job = new RocketChatRestApi::ModerationUserReportedMessagesJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    const QModelIndex modelIndex = mModel->index(newModelIndex.row(), ModerationReportedMessageModel::UserId);
    job->setReportedMessageFromUserId(modelIndex.data().toByteArray());
    connect(job,
            &RocketChatRestApi::ModerationUserReportedMessagesJob::moderationUserReportedMessagesDone,
            this,
            &ModerationReportedMessageConsoleTreeWidget::slotShowReportedMessages);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ModerationReportsByUsersJob job";
    }
}

void ModerationReportedMessageConsoleTreeWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        QMenu menu(this);
        const QModelIndex newModelIndex = mProxyModelModel->mapToSource(index);
        menu.addAction(QIcon::fromTheme(u"visibility"_s), i18nc("@action", "See messages"), this, [this, newModelIndex]() {
            slotShowMessages(newModelIndex);
        });
        menu.addSeparator();

        menu.addAction(QIcon::fromTheme(u"list-remove"_s), i18nc("@action", "Dismiss reports"), this, [this, newModelIndex]() {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), ModerationReportedMessageModel::UserId);
            slotDismissReport(modelIndex);
        });
        menu.addAction(QIcon::fromTheme(u"edit-delete"_s), i18nc("@action", "Delete all Messages"), this, [this, newModelIndex]() {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), ModerationReportedMessageModel::UserId);
            slotDeleteAllMessages(modelIndex);
        });
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(u"list-remove"_s), i18nc("@action", "Deactivate user"), this, [this, newModelIndex]() {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), ModerationReportedMessageModel::UserId);
            slotDesactivateUser(modelIndex);
        });
        menu.exec(mTreeView->viewport()->mapToGlobal(pos));
    }
}

void ModerationReportedMessageConsoleTreeWidget::slotDesactivateUser(const QModelIndex &index)
{
    if (KMessageBox::questionTwoActions(
            this,
            i18n("User will be unable to log in unless reactivated.\nAll reported messages will be permanently deleted from their respective room."),
            i18nc("@title:window", "Deactivate User"),
            KStandardGuiItem::remove(),
            KStandardGuiItem::cancel())
        == KMessageBox::ButtonCode::PrimaryAction) {
        auto job = new RocketChatRestApi::SetUserActiveStatusJob(this);
        const QByteArray userId = index.data().toByteArray();
        // qDebug() << " userId " << userId;
        job->setActivate(false);
        job->setActivateUserId(userId);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::SetUserActiveStatusJob::setUserActiveStatusDone, this, [this](const QJsonObject &replyObject) {
            Q_UNUSED(replyObject)
            // TODO delete messages
            slotLoadElements();
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start SetUserActiveStatusJob job";
        }
    }
}

void ModerationReportedMessageConsoleTreeWidget::slotShowReportedMessages(const QJsonObject &obj)
{
    // qDebug() << "ModerationReportedMessageConsoleTreeWidget::slotShowReportedMessages obj " << obj;
    // Fix parse user/messages (message ok here)
    mCommonMessagesModel->parse(obj);
    ModerationMessagesDialog dlg(mRocketChatAccount, this);
    dlg.setModel(mCommonMessageFilterProxyModel);
    dlg.exec();
}

void ModerationReportedMessageConsoleTreeWidget::slotDismissReport(const QModelIndex &index)
{
    if (KMessageBox::questionTwoActions(this,
                                        i18n("Are you sure you want to dismiss and delete all reports for this user's messages? This action cannot be undone."),
                                        i18nc("@title:window", "Dismiss"),
                                        KStandardGuiItem::remove(),
                                        KStandardGuiItem::cancel())
        == KMessageBox::ButtonCode::PrimaryAction) {
        auto job = new RocketChatRestApi::ModerationDismissReportsJob(this);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        const QModelIndex modelIndex = mModel->index(index.row(), ModerationReportedMessageModel::UserId);
        job->setUserIdForMessages(modelIndex.data().toByteArray());
        connect(job, &RocketChatRestApi::ModerationDismissReportsJob::moderationDismissReportsDone, this, [this]() {
            Q_EMIT refreshList();
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ModerationDismissReportsJob job";
        }
    }
}

void ModerationReportedMessageConsoleTreeWidget::slotDeleteAllMessages(const QModelIndex &index)
{
    if (KMessageBox::questionTwoActions(this,
                                        i18n("Are you sure you want to delete all reported messages from this user?\n"
                                             "The messages will be deleted from the message history and no one will be able to see it.\n"
                                             "This action cannot be undone."),
                                        i18nc("@title:window", "Delete All Messages"),
                                        KStandardGuiItem::remove(),
                                        KStandardGuiItem::cancel())
        == KMessageBox::ButtonCode::PrimaryAction) {
        auto job = new RocketChatRestApi::ModerationUserDeleteReportedMessagesJob(this);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        const QModelIndex modelIndex = mModel->index(index.row(), ModerationReportedMessageModel::UserId);
        job->setUserIdForMessages(modelIndex.data().toByteArray());
        connect(job, &RocketChatRestApi::ModerationUserDeleteReportedMessagesJob::moderationUserDeleteReportedMessagesDone, this, [this]() {
            Q_EMIT refreshList();
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ModerationUserDeleteReportedMessagesJob job";
        }
    }
}

#include "moc_moderationreportedmessageconsoletreewidget.cpp"
