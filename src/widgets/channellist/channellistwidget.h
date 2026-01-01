/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channellistview.h"
#include "libruqolawidgets_private_export.h"
#include "parserocketchaturlutils.h"
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
    void roomSelected(const ChannelListView::ChannelSelectedInfo &roomInfo);
    void roomPressed(const QByteArray &roomId);
    void selectMessageIdRequested(const QByteArray &messageId);
    void forceLineEditFocus();

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void selectMessageId(const QByteArray &messageId);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAccountInitialized();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchRoomTextChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void clearFilterChannel();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOpenLinkRequested(const QString &link);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOpenTeamRequested(const QByteArray &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void applyChannelSelection();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSelectMessageRequested(const QByteArray &messageId,
                                                               const QString &roomId,
                                                               ParseRocketChatUrlUtils::RoomIdType roomType,
                                                               ParseRocketChatUrlUtils::ChannelType channelType);

    ChannelListView *const mChannelView;
    QLineEdit *const mSearchRoomLineEdit;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
