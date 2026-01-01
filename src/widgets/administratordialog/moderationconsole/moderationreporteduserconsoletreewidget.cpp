/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreporteduserconsoletreewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "administratordialog/moderationconsole/moderationmessagesdialog.h"
#include "administratordialog/moderationconsole/moderationusersdialog.h"
#include "connection.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/commonmessagefilterproxymodel.h"
#include "model/moderationmessagesmodel.h"
#include "model/moderationreportedusermodel.h"
#include "model/moderationreporteduserproxymodel.h"
#include "model/searchtreebasefilterproxymodel.h"
#include "moderation/moderationdismissuserreportsjob.h"
#include "moderation/moderationreportsbyuseridjob.h"
#include "moderation/moderationreportuserinfos.h"
#include "moderation/moderationuserreportsjob.h"

#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "users/setuseractivestatusjob.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QTreeView>

ModerationReportedUserConsoleTreeWidget::ModerationReportedUserConsoleTreeWidget(RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
    , mCommonMessagesModel(new ModerationMessagesModel(account, this))
{
    mCommonMessageFilterProxyModel = new CommonMessageFilterProxyModel(mCommonMessagesModel, this);
    mModel = new ModerationReportedUserModel(this);
    mModel->setObjectName(u"mModel"_s);
    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search moderation message…"));

    mProxyModelModel = new ModerationReportedUserProxyModel(mModel, this);
    mProxyModelModel->setObjectName(u"mProxyModelModel"_s);
    mTreeView->setModel(mProxyModelModel);
    connect(this, &ModerationReportedUserConsoleTreeWidget::doubleClicked, this, &ModerationReportedUserConsoleTreeWidget::slotShowReports);
    connect(this, &ModerationReportedUserConsoleTreeWidget::refreshList, this, [this]() {
        slotLoadElements();
    });
    hideColumns();
    connectModel();
}

ModerationReportedUserConsoleTreeWidget::~ModerationReportedUserConsoleTreeWidget() = default;

void ModerationReportedUserConsoleTreeWidget::setModerationRanges(const AdministratorModerationRangeWidget::DateTimeRange &range)
{
    mModerationRanges = range;
    slotLoadElements();
    // qDebug() << " range " << range;
}

void ModerationReportedUserConsoleTreeWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? i18n("No moderation message found") : displayShowMessage());
}

QString ModerationReportedUserConsoleTreeWidget::displayShowMessage() const
{
    QString displayMessageStr = i18np("%1 moderation message (Total: %2)", "%1 moderation messages (Total: %2)", mModel->rowCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

void ModerationReportedUserConsoleTreeWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    auto job = new RocketChatRestApi::ModerationUserReportsJob(this);

    RocketChatRestApi::ModerationUserReportsJob::ModerationUserReportsInfo info;
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
    if (info.isValid()) {
        job->setModerationUserReportsInfo(info);
    }
    job->setQueryParameters(parameters);

    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    if (offset != -1) {
        connect(job,
                &RocketChatRestApi::ModerationUserReportsJob::moderationUserReportJobDone,
                this,
                &ModerationReportedUserConsoleTreeWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::ModerationUserReportsJob::moderationUserReportJobDone, this, &ModerationReportedUserConsoleTreeWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ModerationReportsByUsersJob job";
    }
}

void ModerationReportedUserConsoleTreeWidget::slotShowReports(const QModelIndex &newModelIndex)
{
    if (!newModelIndex.isValid()) {
        return;
    }
    auto job = new RocketChatRestApi::ModerationReportsByUserIdJob(this);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    const QModelIndex modelIndex = mModel->index(newModelIndex.row(), ModerationReportedUserModel::Identifier);
    job->setModerationUserId(modelIndex.data().toByteArray());
    connect(job,
            &RocketChatRestApi::ModerationReportsByUserIdJob::moderationReportsByUserIdDone,
            this,
            &ModerationReportedUserConsoleTreeWidget::slotShowReportedUsers);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start ModerationReportsByUsersJob job";
    }
}

void ModerationReportedUserConsoleTreeWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        QMenu menu(this);
        const QModelIndex newModelIndex = mProxyModelModel->mapToSource(index);
        menu.addAction(QIcon::fromTheme(u"visibility"_s), i18nc("@action", "See reports"), this, [this, newModelIndex]() {
            slotShowReports(newModelIndex);
        });
        menu.addSeparator();

        menu.addAction(QIcon::fromTheme(u"list-remove"_s), i18nc("@action", "Dismiss reports"), this, [this, newModelIndex]() {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), ModerationReportedUserModel::Identifier);
            slotDismissReport(modelIndex);
        });

        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(u"list-remove"_s), i18nc("@action", "Deactivate user"), this, [this, newModelIndex]() {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), ModerationReportedUserModel::UserName);
            slotDesactivateUser(modelIndex);
        });
        menu.exec(mTreeView->viewport()->mapToGlobal(pos));
    }
}

void ModerationReportedUserConsoleTreeWidget::slotDesactivateUser(const QModelIndex &index)
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
        qDebug() << " userId " << userId;
        job->setActivate(false);
        job->setActivateUserId(userId);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::SetUserActiveStatusJob::setUserActiveStatusDone, this, [this](const QJsonObject &replyObject) {
            // TODO use ModerationDismissUserReportsJob
            qDebug() << " XCXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << replyObject;
            // TODO delete messages
            slotLoadElements();
        });
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start SetUserActiveStatusJob job";
        }
    }
}

void ModerationReportedUserConsoleTreeWidget::slotShowReportedUsers(const QJsonObject &obj)
{
    ModerationReportUserInfos moderationReportUserInfos;
    moderationReportUserInfos.parseModerationReportUserInfos(obj);

    qDebug() << "ModerationReportedUserConsoleTreeWidget::slotShowReportedUsers obj " << obj << moderationReportUserInfos;
    ModerationUsersDialog dlg(mRocketChatAccount, this);
    dlg.setModerationReportUserInfos(moderationReportUserInfos);
    dlg.exec();
}

void ModerationReportedUserConsoleTreeWidget::slotDismissReport(const QModelIndex &index)
{
    if (KMessageBox::questionTwoActions(this,
                                        i18n("All reports will be deleted and the reported messages won't be affected."),
                                        i18nc("@title:window", "Dismiss All Reports"),
                                        KStandardGuiItem::remove(),
                                        KStandardGuiItem::cancel())
        == KMessageBox::ButtonCode::PrimaryAction) {
#if 0
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
#endif
    }
}

#include "moc_moderationreporteduserconsoletreewidget.cpp"
