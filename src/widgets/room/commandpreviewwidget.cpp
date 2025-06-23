/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandpreviewwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "commandpreviewimagedelegate.h"
#include "commands/previewcommandutils.h"
#include "connection.h"
#include "model/previewcommandmodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KBusyIndicatorWidget>
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QListView>
#include <QStackedWidget>

CommandPreviewWidget::CommandPreviewWidget(QWidget *parent)
    : QWidget{parent}
    , mListView(new QListView(this))
    , mPreviewCommandModel(new PreviewCommandModel(this))
    , mStackWidget(new QStackedWidget(this))
    , mCommandPreviewLoadingWidget(new CommandPreviewLoadingWidget(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mStackWidget->setObjectName(u"mStackWidget"_s);
    mainLayout->addWidget(mStackWidget);

    mCommandPreviewLoadingWidget->setObjectName(u"mCommandPreviewLoadingWidget"_s);

    mListView->setObjectName(u"mListView"_s);
    mListView->setViewMode(QListView::IconMode);
    mListView->setFlow(QListView::LeftToRight);
    mListView->setResizeMode(QListView::Adjust);
    mListView->setWrapping(false);
    mListView->setItemDelegate(new CommandPreviewImageDelegate(mListView, this));
    mListView->setModel(mPreviewCommandModel);
    mListView->setSpacing(2);

    mStackWidget->addWidget(mListView);
    mStackWidget->addWidget(mCommandPreviewLoadingWidget);

    mStackWidget->setCurrentWidget(mCommandPreviewLoadingWidget);
    connect(mListView, &QListView::doubleClicked, this, &CommandPreviewWidget::slotDoubleClicked);

    setMaximumHeight(115);
}

CommandPreviewWidget::~CommandPreviewWidget() = default;

void CommandPreviewWidget::keyPressEvent(QKeyEvent *e)
{
    const int key = e->key();
    if (key == Qt::Key_Escape) {
        e->accept();
        hidePreview();
    } else if (key == Qt::Key_Return || key == Qt::Key_Enter) {
        const QModelIndexList selectedIndexes = mListView->selectionModel()->selectedIndexes();
        if (!selectedIndexes.isEmpty()) {
            slotDoubleClicked(selectedIndexes.constFirst());
        }
    }
}

void CommandPreviewWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    if (mCurrentRocketChatAccount != account) {
        mCurrentRocketChatAccount = account;
        mCommandPreviewLoadingWidget->stop();
        hidePreview();
    }
}

void CommandPreviewWidget::setPreviewCommandInfo(const RocketChatRestApi::PreviewsCommandJob::PreviewsCommandInfo &info)
{
    if (mCurrentRocketChatAccount) {
        auto job = new RocketChatRestApi::PreviewsCommandJob(this);
        mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
        job->setPreviewsCommandInfo(info);
        mPreviewCommandInfo = info;
        setVisible(true);
        mStackWidget->setCurrentWidget(mCommandPreviewLoadingWidget);
        mCommandPreviewLoadingWidget->start();
        connect(job, &RocketChatRestApi::PreviewsCommandJob::previewsCommandDone, this, &CommandPreviewWidget::slotParsePreviewCommandItems);

        if (!job->start()) {
            qCDebug(RUQOLAWIDGETS_LOG) << "Impossible to start PreviewsCommandJob job";
        }
    }
}

void CommandPreviewWidget::hidePreview()
{
    setVisible(false);
    mPreviewCommandModel->clear();
}

void CommandPreviewWidget::slotParsePreviewCommandItems(const QJsonObject &replyObject)
{
    const QList<PreviewCommand> commands = PreviewCommandUtils::parsePreviewJson(replyObject);
    mPreviewCommandModel->setPreviewCommands(commands);
    setVisible(!commands.isEmpty());
    mCommandPreviewLoadingWidget->stop();
    mStackWidget->setCurrentWidget(mListView);
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

CommandPreviewLoadingWidget::CommandPreviewLoadingWidget(QWidget *parent)
    : QWidget(parent)
    , mBusyIndicator(new KBusyIndicatorWidget(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mBusyIndicator->setMaximumHeight(30);

    mBusyIndicator->setObjectName(u"mBusyIndicator"_s);
    mainLayout->addWidget(mBusyIndicator, 0, Qt::AlignTop);

    auto label = new QLabel(i18n("Loading…"), this);
    label->setObjectName(u"label"_s);
    QFont f = label->font();
    f.setBold(true);
    f.setItalic(true);
    label->setFont(f);
    mainLayout->addWidget(label, 0, Qt::AlignTop);
    mainLayout->addStretch(1);
}

CommandPreviewLoadingWidget::~CommandPreviewLoadingWidget() = default;

void CommandPreviewLoadingWidget::stop()
{
    mBusyIndicator->stop();
}

void CommandPreviewLoadingWidget::start()
{
    mBusyIndicator->start();
}

#include "moc_commandpreviewwidget.cpp"
