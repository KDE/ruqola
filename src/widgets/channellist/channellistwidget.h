/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "parsemessageurlutils.h"
#include "room.h"
#include <QPointer>
#include <QWidget>
class ChannelListView;
class QLineEdit;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ChannelListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelListWidget(QWidget *parent = nullptr);
    ~ChannelListWidget() override;

    ChannelListView *channelListView() const;

    void setCurrentRocketChatAccount(RocketChatAccount *account);

    void setLayoutSpacing(int spacing);

Q_SIGNALS:
    void roomSelected(const QString &roomName, const QString &roomId, Room::RoomType roomType);
    void selectMessageIdRequested(const QString &messageId);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    void slotAccountInitialized();
    void slotSearchRoomTextChanged();
    void clearFilterChannel();
    void slotOpenLinkRequested(const QString &link);
    void slotOpenTeamRequested(const QString &identifier);
    void applyChannelSelection();
    void slotSelectMessageRequested(const QString &messageId,
                                    const QString &roomId,
                                    ParseMessageUrlUtils::RoomIdType roomType,
                                    ParseMessageUrlUtils::ChannelType channelType);

    ChannelListView *const mChannelView;
    QLineEdit *const mSearchRoomLineEdit;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
