/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandpreviewwidget.h"
#include "commandpreviewimagedelegate.h"
#include "commands/previewcommandutils.h"
#include "connection.h"
#include "model/previewcommandmodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QListView>

CommandPreviewWidget::CommandPreviewWidget(QWidget *parent)
    : QWidget{parent}
    , mListView(new QListView(this))
    , mPreviewCommandModel(new PreviewCommandModel(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mListView->setObjectName(QStringLiteral("mListView"));
    mainLayout->addWidget(mListView);
    mListView->setViewMode(QListView::IconMode);
    mListView->setFlow(QListView::LeftToRight);
    mListView->setResizeMode(QListView::Adjust);
    mListView->setWrapping(false);
    mListView->setItemDelegate(new CommandPreviewImageDelegate(mListView, this));

    mListView->setModel(mPreviewCommandModel);
    connect(mListView, &QListView::doubleClicked, this, &CommandPreviewWidget::slotDoubleClicked);
}

CommandPreviewWidget::~CommandPreviewWidget() = default;

void CommandPreviewWidget::keyPressEvent(QKeyEvent *e)
{
    const int key = e->key();
    if (key == Qt::Key_Escape) {
        e->accept();
        setVisible(false);
        mPreviewCommandModel->clear();
    }
}
void CommandPreviewWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mCurrentRocketChatAccount = account;
    // Hide it when we switch account
    setVisible(false);
    mPreviewCommandModel->clear();
}

void CommandPreviewWidget::setPreviewCommandInfo(const RocketChatRestApi::PreviewsCommandJob::PreviewsCommandInfo &info)
{
    if (mCurrentRocketChatAccount) {
        auto job = new RocketChatRestApi::PreviewsCommandJob(this);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        job->setPreviewsCommandInfo(info);
        mPreviewCommandInfo = info;
        connect(job, &RocketChatRestApi::PreviewsCommandJob::previewsCommandDone, this, &CommandPreviewWidget::slotParsePreviewCommandItems);
        if (!job->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start PreviewsCommandJob job";
        }
    }
}

void CommandPreviewWidget::slotParsePreviewCommandItems(const QJsonObject &replyObject)
{
    const QList<PreviewCommand> commands = PreviewCommandUtils::parsePreviewJson(replyObject);
    mPreviewCommandModel->setPreviewCommands(commands);
    setVisible(!commands.isEmpty());
}

void CommandPreviewWidget::slotDoubleClicked(const QModelIndex &index)
{
    const PreviewCommand command = index.data(static_cast<int>(PreviewCommandModel::PreviewCommandRoles::PreviewCommandInfo)).value<PreviewCommand>();
    RocketChatRestApi::PreviewsCommandJob::PreviewsCommandItemInfo info;
    info.value = command.value();
    info.id = command.id();
    info.type = command.typeStr();

    mPreviewCommandInfo.itemInfo = std::move(info);
    setVisible(false);
    mPreviewCommandModel->clear();
    Q_EMIT sendPreviewCommandInfo(mPreviewCommandInfo);
}

#include "moc_commandpreviewwidget.cpp"
