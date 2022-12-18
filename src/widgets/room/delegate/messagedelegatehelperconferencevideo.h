/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "delegateutils/textselection.h"
#include "delegateutils/textselectionimpl.h"

#include <QIcon>
#include <QModelIndex>
#include <QSize>
#include <QTextDocument>
#include <lrucache.h>

#include <memory>

class QListView;
class QPainter;
class QRect;
class QMouseEvent;
class QHelpEvent;
class QStyleOptionViewItem;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT MessageDelegateHelperConferenceVideo : public QObject, public DocumentFactoryInterface
{
public:
    explicit MessageDelegateHelperConferenceVideo(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl);
    ~MessageDelegateHelperConferenceVideo() override;
    //    void
    //    draw(const MessageAttachment &msgAttach, QPainter *painter, QRect messageRect, const QModelIndex &index, const QStyleOptionViewItem &option) const
    //    override; QSize sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
    //    override; Q_REQUIRED_RESULT bool handleMouseEvent(const MessageAttachment &msgAttach,
    //                                            QMouseEvent *mouseEvent,
    //                                            QRect attachmentsRect,
    //                                            const QStyleOptionViewItem &option,
    //                                            const QModelIndex &index) override;

private:
    struct ConferenceCallLayout {
        QString audioPath;
        QString title;
        QString description;
        QSize titleSize;
        QSize descriptionSize;
        QRect playerVolumeButtonRect;
        QRect downloadButtonRect;
    };
    //    Q_REQUIRED_RESULT QPoint adaptMousePosition(const QPoint &pos,
    //                                                const MessageAttachment &msgAttach,
    //                                                QRect attachmentsRect,
    //                                                const QStyleOptionViewItem &option) override;
    Q_REQUIRED_RESULT ConferenceCallLayout layoutConferenceCall(const MessageAttachment &msgAttach,
                                                                const QStyleOptionViewItem &option,
                                                                int attachmentsWidth) const;

    QListView *const mListView;
    TextSelectionImpl *const mSelectionImpl;
    RocketChatAccount *mRocketChatAccount = nullptr;
};
