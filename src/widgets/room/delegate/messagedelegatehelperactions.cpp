/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelperactions.h"
#include "autogenerateui/autogenerateinteractionui.h"
#include "autogenerateui/autogenerateinteractionuidialog.h"
#include "common/delegatepaintutil.h"
#include "connection.h"
#include "misc/appsuiinteractionjob.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QListView>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>

MessageDelegateHelperActions::MessageDelegateHelperActions(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageBlockDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageDelegateHelperActions::~MessageDelegateHelperActions() = default;

void MessageDelegateHelperActions::draw(const Block &block,
                                        QPainter *painter,
                                        QRect blockRect,
                                        const QModelIndex &index,
                                        const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const ActionsLayout layout = layoutActions(block, option, blockRect.width());
    for (const auto &button : std::as_const(layout.buttonList)) {
        // Draw button
        const QPen origPen = painter->pen();
        const QBrush origBrush = painter->brush();
        const QPen buttonPen(option.palette.color(QPalette::Highlight).darker());
        QColor backgroundColor = option.palette.color(QPalette::Highlight);
        backgroundColor.setAlpha(60);
        const QBrush buttonBrush(backgroundColor);
        const QRectF buttonRect = button.buttonRect.translated(blockRect.topLeft());
        // Rounded rect
        painter->setPen(buttonPen);
        painter->setBrush(buttonBrush);
        painter->drawRoundedRect(buttonRect, 5, 5);
        painter->setBrush(origBrush);
        painter->setPen(origPen);
        const QRectF r = buttonRect.adjusted((buttonRect.width() - button.buttonRect.width()) / 2, 0, 0, 0);
        painter->drawText(r, Qt::AlignVCenter | Qt::AlignHCenter, button.text);
    }
}

QSize MessageDelegateHelperActions::sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const ActionsLayout layout = layoutActions(block, option, maxWidth);
    if (layout.buttonList.isEmpty()) {
        return {};
    }
    const int height = layout.buttonList.at(0).buttonRect.height() + DelegatePaintUtil::margin();
    const auto buttons = layout.buttonList;
    int width = 0;
    for (const auto &b : buttons) {
        width += b.buttonRect.width();
    }
    return {qMax(0, static_cast<int>(width)), height};
}

bool MessageDelegateHelperActions::handleMouseEvent(const Block &block,
                                                    QMouseEvent *mouseEvent,
                                                    QRect blocksRect,
                                                    const QStyleOptionViewItem &option,
                                                    const QModelIndex &index)
{
    Q_UNUSED(index);
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const QPoint pos = mouseEvent->pos();
        const ActionsLayout layout = layoutActions(block, option, blocksRect.width());
        for (const ButtonLayout &button : layout.buttonList) {
            if (button.buttonRect.translated(blocksRect.topLeft()).contains(pos)) {
                // qDebug() << " button.appId" << button.appId;
                // qDebug() << " button.actionId" << button.actionId;
                // qDebug() << " button.value" << button.value;
                // qDebug() << " button.blockId" << button.blockId;
                // qDebug() << " button.url" << button.blockId;
                const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
                Q_ASSERT(message);
                // qDebug() << " message->roomId" << message->roomId();
                // qDebug() << " message->messageId" << message->messageId();

                if (!button.url.isEmpty()) {
                    Q_EMIT mRocketChatAccount->openLinkRequested(button.url);
                } else {
                    executeBlockAction(button.appId, button.actionId, button.value, button.blockId, message->roomId(), message->messageId());
                }
                return true;
            }
        }
    }
    return false;
}

void MessageDelegateHelperActions::executeBlockAction(const QString &appId,
                                                      const QString &actionId,
                                                      const QString &value,
                                                      const QString &blockId,
                                                      const QByteArray &roomId,
                                                      const QByteArray &messageId)
{
    auto job = new RocketChatRestApi::AppsUiInteractionJob(this);
    RocketChatRestApi::AppsUiInteractionJob::AppsUiInteractionJobInfo info;
    info.methodName = appId;
    info.generateMessageObj(actionId, value, blockId, roomId, messageId);
    job->setAppsUiInteractionJobInfo(info);

    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::AppsUiInteractionJob::appsUiInteractionDone, this, [this](const QJsonObject &replyObject) {
        auto dialog = new AutoGenerateInteractionUiDialog(mRocketChatAccount);
        if (dialog->parse(replyObject)) {
            dialog->exec();
            delete dialog;
        } else {
            delete dialog;
        }
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppsUiInteractionJob job";
    }
}

MessageDelegateHelperActions::ActionsLayout
MessageDelegateHelperActions::layoutActions(const Block &block, const QStyleOptionViewItem &option, int blockRectWidth) const
{
    Q_UNUSED(blockRectWidth)
    ActionsLayout layout;

    qreal x = 0;
    const auto actions = block.blockActions();
    for (const auto &act : actions) {
        ButtonLayout buttonLayout;
        buttonLayout.text = act.text();
        buttonLayout.actionId = act.actionId();
        buttonLayout.appId = block.appId();
        buttonLayout.value = act.value();
        buttonLayout.blockId = act.blockId();
        buttonLayout.url = act.url();
        const QSize buttonSize = option.fontMetrics.size(Qt::TextSingleLine, buttonLayout.text);
        buttonLayout.buttonRect = QRectF(x, 0, buttonSize.width() + 2 * DelegatePaintUtil::margin(), buttonSize.height());
        layout.buttonList.append(std::move(buttonLayout));
        x += buttonLayout.buttonRect.width() + DelegatePaintUtil::margin();
    }
    return layout;
}
