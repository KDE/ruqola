/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationconsoletreewidget.h"
#include "administratordialog/moderationconsole/moderationmessagesdialog.h"
#include "connection.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/commonmessagefilterproxymodel.h"
#include "model/moderationmessagesmodel.h"
#include "model/moderationreportedmessagemodel.h"
#include "model/searchtreebasefilterproxymodel.h"
#include "moderation/moderationdismissreportsjob.h"
#include "moderation/moderationreportsbyusersjob.h"
#include "moderation/moderationuserdeletereportedmessagesjob.h"
#include "moderation/moderationuserreportedmessagesjob.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QTreeView>

ModerationConsoleTreeWidget::ModerationConsoleTreeWidget(RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
    , mCommonMessagesModel(new ModerationMessagesModel(account, this))
{
    mCommonMessageFilterProxyModel = new CommonMessageFilterProxyModel(mCommonMessagesModel, this);
    mModel = new ModerationReportedMessageModel(this);
    mModel->setObjectName(QStringLiteral("mModel"));
    mSearchLineEdit->setPlaceholderText(i18n("Search moderation message..."));

    mProxyModelModel = new SearchTreeBaseFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName(QStringLiteral("mProxyModelModel"));
    mTreeView->setModel(mProxyModelModel);
    connect(this, &ModerationConsoleTreeWidget::doubleClicked, this, &ModerationConsoleTreeWidget::slotShowMessages);
    connect(this, &ModerationConsoleTreeWidget::refreshList, this, [this]() {
        slotLoadElements();
    });
    hideColumns();
    connectModel();
}

ModerationConsoleTreeWidget::~ModerationConsoleTreeWidget() = default;

void ModerationConsoleTreeWidget::setModerationRanges(const AdministratorModerationRangeWidget::DateTimeRange &range)
{
    mModerationRanges = range;
    slotLoadElements();
    // qDebug() << " range " << range;
}

void ModerationConsoleTreeWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? i18n("No moderation message found") : displayShowMessage());
}

QString ModerationConsoleTreeWidget::displayShowMessage() const
{
    QString displayMessageStr = i18np("%1 moderation message (Total: %2)", "%1 moderation messages (Total: %2)", mModel->rowCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

void ModerationConsoleTreeWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    auto job = new RocketChatRestApi::ModerationReportsByUsersJob(this);

    RocketChatRestApi::ModerationReportsByUsersJob::ModerationReportsByUsersInfo info;
    info.mOldest = mModerationRanges.fromDate;
    info.mLatest = mModerationRanges.toDate;
    info.mSelector = searchName;
    RocketChatRestApi::QueryParameters parameters;
    //    QMap<QString, RocketChatRestApi::QueryParameters::SortOrder> map;
    //    map.insert(QStringLiteral("name"), RocketChatRestApi::QueryParameters::SortOrder::Ascendant);
    //    parameters.setSorting(map);
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
        connect(job, &RocketChatRestApi::ModerationReportsByUsersJob::moderationReportByUserDone, this, &ModerationConsoleTreeWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::ModerationReportsByUsersJob::moderationReportByUserDone, this, &ModerationConsoleTreeWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ModerationReportsByUsersJob job";
    }
}

void ModerationConsoleTreeWidget::slotShowMessages(const QModelIndex &newModelIndex)
{
    if (!newModelIndex.isValid()) {
        return;
    }
    auto job = new RocketChatRestApi::ModerationUserReportedMessagesJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    const QModelIndex modelIndex = mModel->index(newModelIndex.row(), ModerationReportedMessageModel::UserId);
    job->setReportedMessageFromUserId(QString::fromLatin1(modelIndex.data().toByteArray()));
    connect(job,
            &RocketChatRestApi::ModerationUserReportedMessagesJob::moderationUserReportedMessagesDone,
            this,
            &ModerationConsoleTreeWidget::slotShowReportedMessages);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ModerationReportsByUsersJob job";
    }
}

void ModerationConsoleTreeWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        QMenu menu(this);
        const QModelIndex newModelIndex = mProxyModelModel->mapToSource(index);
        menu.addAction(QIcon::fromTheme(QStringLiteral("visibility")), i18n("See messages"), this, [this, newModelIndex]() {
            slotShowMessages(newModelIndex);
        });
        menu.addSeparator();

        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Dismiss reports"), this, [this, newModelIndex]() {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), ModerationReportedMessageModel::UserId);
            slotDismissReport(modelIndex);
        });
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Delete all Messages"), this, [this, newModelIndex]() {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), ModerationReportedMessageModel::UserId);
            slotDeleteAllMessages(modelIndex);
        });
        menu.exec(mTreeView->viewport()->mapToGlobal(pos));
    }
}

void ModerationConsoleTreeWidget::slotShowReportedMessages(const QJsonObject &obj)
{
    mCommonMessagesModel->parse(obj);
    ModerationMessagesDialog dlg(mRocketChatAccount, this);
    dlg.setModel(mCommonMessageFilterProxyModel);
    dlg.exec();
}

void ModerationConsoleTreeWidget::slotDismissReport(const QModelIndex &index)
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
        job->setUserIdForMessages(QString::fromLatin1(modelIndex.data().toByteArray()));
        connect(job, &RocketChatRestApi::ModerationDismissReportsJob::moderationDismissReportsDone, this, [this]() {
            Q_EMIT refreshList();
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ModerationDismissReportsJob job";
        }
    }
}

void ModerationConsoleTreeWidget::slotDeleteAllMessages(const QModelIndex &index)
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
        job->setUserIdForMessages(QString::fromLatin1(modelIndex.data().toByteArray()));
        connect(job, &RocketChatRestApi::ModerationUserDeleteReportedMessagesJob::moderationUserDeleteReportedMessagesDone, this, [this]() {
            Q_EMIT refreshList();
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ModerationUserDeleteReportedMessagesJob job";
        }
    }
}

#include "moc_moderationconsoletreewidget.cpp"
