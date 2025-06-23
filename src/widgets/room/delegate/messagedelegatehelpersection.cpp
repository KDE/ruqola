/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messagedelegatehelpersection.h"
using namespace Qt::Literals::StringLiterals;

#include "autogenerateui/autogenerateinteractionutil.h"
#include "common/delegatepaintutil.h"
#include "connection.h"
#include "misc/appsuiinteractionjob.h"
#include "rocketchataccount.h"
#include "ruqola_autogenerateui_debug.h"

#include <KLocalizedString>

#include <QAbstractItemView>
#include <QListView>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>

MessageDelegateHelperSection::MessageDelegateHelperSection(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageBlockDelegateHelperBase(account, view, textSelectionImpl)
    , mMenuIcon(QIcon::fromTheme(u"menu_new"_s))
{
}

MessageDelegateHelperSection::~MessageDelegateHelperSection() = default;

void MessageDelegateHelperSection::draw(const Block &block,
                                        QPainter *painter,
                                        QRect blockRect,
                                        const QModelIndex &index,
                                        const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const SectionLayout layout = layoutSection(block, option, blockRect.width());
    // Draw title and buttons
    const int positionY = blockRect.y() + option.fontMetrics.ascent();
    painter->drawText(blockRect.x(), positionY, layout.sectionText);
    // DrawButton
    if (!layout.buttonText.isEmpty()) {
        const QRectF buttonRect = layout.buttonRect.translated(blockRect.x(), blockRect.y());
        const QPen origPen = painter->pen();
        const QBrush origBrush = painter->brush();
        const QPen buttonPen(option.palette.color(QPalette::Button).darker());
        const QColor backgroundColor = option.palette.color(QPalette::Window);
        const QBrush buttonBrush(backgroundColor);
        painter->setPen(buttonPen);
        painter->setBrush(buttonBrush);

        painter->drawRoundedRect(buttonRect.adjusted(-5, 0, +5, 0), 5, 5);
        painter->setBrush(origBrush);
        painter->setPen(origPen);

        // Fix position
        painter->drawText(buttonRect.x(), positionY, layout.buttonText);
    }
    if (!layout.menuRect.isEmpty()) {
        mMenuIcon.paint(painter, layout.menuRect.translated(blockRect.topLeft()));
    }
}

QSize MessageDelegateHelperSection::sizeHint(const Block &block, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const SectionLayout layout = layoutSection(block, option, maxWidth);
    const int height = layout.sectionTextSize.height() + DelegatePaintUtil::margin();
    return {qMax(0, layout.sectionTextSize.width()), height};
}

bool MessageDelegateHelperSection::handleMouseEvent(const Block &block,
                                                    QMouseEvent *mouseEvent,
                                                    QRect blocksRect,
                                                    const QStyleOptionViewItem &option,
                                                    const QModelIndex &index)
{
    Q_UNUSED(index);
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const QPoint pos = mouseEvent->pos();
        const SectionLayout layout = layoutSection(block, option);
        if (layout.buttonRect.translated(blocksRect.topLeft()).contains(pos)) {
            const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
            AutoGenerateInteractionUtil::MessageBlockMessageActionUserInfo messageBockUserinfo;
            messageBockUserinfo.actionId = block.blockAccessory().actionId();
            messageBockUserinfo.roomId = message->roomId();
            messageBockUserinfo.messageId = message->messageId();
            messageBockUserinfo.threadId = message->threadMessageId();
            messageBockUserinfo.triggerId = QUuid::createUuid().toByteArray(QUuid::Id128);
            messageBockUserinfo.blockId = block.blockId();
            messageBockUserinfo.value = block.blockAccessory().value();
            RocketChatRestApi::AppsUiInteractionJob::AppsUiInteractionJobInfo info;
            info.methodName = block.appId();
            info.messageObj = AutoGenerateInteractionUtil::createBlockMessageActionUser(messageBockUserinfo);
            auto job = new RocketChatRestApi::AppsUiInteractionJob(this);
            mRocketChatAccount->restApi()->initializeRestApiJob(job);
            job->setAppsUiInteractionJobInfo(info);
            // qDebug() << " info " << info;
            connect(job, &RocketChatRestApi::AppsUiInteractionJob::appsUiInteractionDone, this, [](const QJsonObject &replyObject) {
                Q_UNUSED(replyObject);
                qDebug() << " DONE";
            });
            if (!job->start()) {
                qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "Impossible to start AppsUiInteractionJob job";
            }
            return true;
        }
        if (layout.menuRect.translated(blocksRect.topLeft()).contains(pos)) {
            auto parentWidget = const_cast<QWidget *>(option.widget);
            const auto blockAccessory = block.blockAccessory();
            const auto options = blockAccessory.options();
            if (!options.isEmpty()) {
                QMenu menu(parentWidget);
                const Message *message = index.data(MessagesModel::MessagePointer).value<Message *>();
                for (const auto &opt : options) {
                    auto act = menu.addAction(opt.text());
                    const QString value = opt.value();
                    connect(act, &QAction::triggered, this, [this, value, message, block]() {
                        AutoGenerateInteractionUtil::MessageBlockMessageActionUserInfo messageBockUserinfo;
                        messageBockUserinfo.actionId = block.blockAccessory().actionId();
                        messageBockUserinfo.roomId = message->roomId();
                        messageBockUserinfo.messageId = message->messageId();
                        messageBockUserinfo.threadId = message->threadMessageId();
                        messageBockUserinfo.triggerId = QUuid::createUuid().toByteArray(QUuid::Id128);
                        messageBockUserinfo.blockId = block.blockId();
                        messageBockUserinfo.value = block.blockAccessory().value();
                        RocketChatRestApi::AppsUiInteractionJob::AppsUiInteractionJobInfo info;
                        info.methodName = block.appId();
                        info.messageObj = AutoGenerateInteractionUtil::createBlockMessageActionUser(messageBockUserinfo);
                        auto job = new RocketChatRestApi::AppsUiInteractionJob(this);
                        mRocketChatAccount->restApi()->initializeRestApiJob(job);
                        job->setAppsUiInteractionJobInfo(info);
                        // qDebug() << " info " << info;
                        connect(job, &RocketChatRestApi::AppsUiInteractionJob::appsUiInteractionDone, this, [](const QJsonObject &replyObject) {
                            Q_UNUSED(replyObject);
                            // qDebug() << " DONE";
                        });
                        if (!job->start()) {
                            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "Impossible to start AppsUiInteractionJob job";
                        }
                    });
                }
                menu.exec(QCursor::pos());
            }
            return true;
        }
    }
    return false;
}

MessageDelegateHelperSection::SectionLayout
MessageDelegateHelperSection::layoutSection(const Block &block, const QStyleOptionViewItem &option, int blockRectWidth) const
{
    Q_UNUSED(blockRectWidth)
    SectionLayout layout;
    layout.sectionText = block.sectionText();
    layout.sectionTextSize = option.fontMetrics.size(Qt::TextSingleLine, layout.sectionText);
    const auto blockAccessory = block.blockAccessory();
    switch (blockAccessory.type()) {
    case BlockAccessory::AccessoryType::Overflow: {
        if (!blockAccessory.options().isEmpty()) {
            const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
            layout.menuRect = QRect(layout.sectionTextSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
        }
        break;
    }
    case BlockAccessory::AccessoryType::Button: {
        layout.buttonText = blockAccessory.text();
        layout.buttonTextSize = option.fontMetrics.size(Qt::TextSingleLine, layout.buttonText);
        layout.buttonRect =
            QRect(layout.sectionTextSize.width() + DelegatePaintUtil::margin(), 0, layout.buttonTextSize.width(), layout.buttonTextSize.height());
        break;
    }
    case BlockAccessory::AccessoryType::Unknown:
        break;
    }

    return layout;
}
