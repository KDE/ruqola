/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelperactions.h"

#include "common/delegatepaintutil.h"
#include "connection.h"
#include "dialogs/showvideodialog.h"
#include "misc/messageattachmentdownloadandsavejob.h"
#include "misc/methodcalljob.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>

using namespace Qt::Literals::StringLiterals;
MessageAttachmentDelegateHelperActions::MessageAttachmentDelegateHelperActions(RocketChatAccount *account,
                                                                               QListView *view,
                                                                               TextSelectionImpl *textSelectionImpl)
    : MessageDelegateHelperBase(account, view, textSelectionImpl)
{
}

MessageAttachmentDelegateHelperActions::~MessageAttachmentDelegateHelperActions() = default;

void MessageAttachmentDelegateHelperActions::draw(const MessageAttachmentActions &act,
                                                  QPainter *painter,
                                                  QRect messageRect,
                                                  [[maybe_unused]] const QModelIndex &index,
                                                  const QStyleOptionViewItem &option) const
{
    const ActionsLayout layout = layoutActions(act, option, messageRect.width());
    for (const auto &button : std::as_const(layout.buttonList)) {
        // Draw button
        const QPen origPen = painter->pen();
        const QBrush origBrush = painter->brush();
        const QPen buttonPen(option.palette.color(QPalette::Highlight).darker());
        QColor backgroundColor = option.palette.color(QPalette::Highlight);
        backgroundColor.setAlpha(60);
        const QBrush buttonBrush(backgroundColor);
        const QRectF buttonRect = button.buttonRect.translated(messageRect.topLeft());
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

QSize MessageAttachmentDelegateHelperActions::sizeHint(const MessageAttachmentActions &act,
                                                       [[maybe_unused]] const QModelIndex &index,
                                                       int maxWidth,
                                                       const QStyleOptionViewItem &option) const
{
    const ActionsLayout layout = layoutActions(act, option, maxWidth);
    if (layout.buttonList.isEmpty()) {
        return {};
    }
    switch (act.alignment()) {
    case MessageAttachmentActions::AlignmentButton::Horizontal: {
        const int height = layout.buttonList.at(0).buttonRect.height() + DelegatePaintUtil::margin();
        const auto buttons = layout.buttonList;
        int width = 0;
        for (const auto &b : buttons) {
            width += b.buttonRect.width();
        }
        return {qMax(0, static_cast<int>(width)), height};
    }
    case MessageAttachmentActions::AlignmentButton::Vertical: {
        const auto buttons = layout.buttonList;
        int height = 0;
        int width = 0;
        for (const auto &b : buttons) {
            width = qMax(width, static_cast<int>(b.buttonRect.width()));
            height += b.buttonRect.height();
        }
        return {qMax(0, static_cast<int>(width)), height};
    }
    case MessageAttachmentActions::AlignmentButton::Unknown: {
        qCWarning(RUQOLAWIDGETS_LOG) << "MessageAttachmentActions::AlignmentButton::Unknown it's a bug";
        return {};
    }
    }
    return {};
}

bool MessageAttachmentDelegateHelperActions::handleMouseEvent(const MessageAttachmentActions &act,
                                                              QMouseEvent *mouseEvent,
                                                              QRect attachmentsRect,
                                                              const QStyleOptionViewItem &option,
                                                              const QModelIndex &index)
{
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const QPoint pos = mouseEvent->pos();
        const ActionsLayout layout = layoutActions(act, option, attachmentsRect.width());
        for (const ButtonLayout &button : layout.buttonList) {
            if (button.buttonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
                const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
                Q_ASSERT(message);
                // qDebug() << " message->roomId" << message->roomId();
                qDebug() << " act->msg" << button.message;
                qDebug() << " act->msg" << button.text;
                auto job = new RocketChatRestApi::MethodCallJob(this);
                QJsonArray params;
                QJsonObject obj;
                obj["msg"_L1] = button.message;
                obj["rid"_L1] = QString::fromLatin1(message->roomId());
                obj["_id"_L1] = u"foo"_s; // TODO fix me
                // {\"_id\":\"ocq2cYp9Ekd4W2uEQ\",\"rid\":\"H7Q9djXQ4iShzD9T2jYJat6TN6C3TTSMjk\",\"msg\":\"/auto-reply status\"}
                params.append(obj);
                const QString methodName = u"sendMessage"_s;
                const RocketChatRestApi::MethodCallJob::MethodCallJobInfo info{
                    .messageObj = mRocketChatAccount->ddp()->generateJsonObject(methodName, params),
                    .methodName = methodName,
                    .anonymous = false,
                };
                job->setMethodCallJobInfo(info);
                // qDebug() << " info " << info;
                mRocketChatAccount->restApi()->initializeRestApiJob(job);
                if (!job->start()) {
                    qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start sendMessage job (from RocketChatRestApi::MethodCallJob)";
                }
                return true;
            }
        }
    }
    return false;
}

MessageAttachmentDelegateHelperActions::ActionsLayout
MessageAttachmentDelegateHelperActions::layoutActions(const MessageAttachmentActions &act, const QStyleOptionViewItem &option, int attachmentsWidth) const
{
    ActionsLayout layout;

    qreal x = 0;
    qreal y = 0;
    const MessageAttachmentActions::AlignmentButton alignment = act.alignment();
    const bool horizontal = (alignment == MessageAttachmentActions::AlignmentButton::Horizontal);
    const auto actions = act.actions();
    for (const auto &action : actions) {
        ButtonLayout buttonLayout;
        buttonLayout.text = action.text();
        buttonLayout.message = action.msg();
        const QSize buttonSize = option.fontMetrics.size(Qt::TextSingleLine, buttonLayout.text);
        if (horizontal) {
            buttonLayout.buttonRect = QRectF(x, y, buttonSize.width() + 2 * DelegatePaintUtil::margin(), buttonSize.height());
            x += buttonLayout.buttonRect.width() + DelegatePaintUtil::margin();
        } else {
            buttonLayout.buttonRect = QRectF(x, y, buttonSize.width() + 2 * DelegatePaintUtil::margin(), buttonSize.height());
            y += buttonLayout.buttonRect.height() + DelegatePaintUtil::margin();
        }
        layout.buttonList.append(std::move(buttonLayout));
    }
    return layout;
}
