/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "channellistview.h"
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
    void roomSelected(const ChannelListView::ChannelSelectedInfo &roomInfo);
    void roomPressed(const QString &roomId);
    void selectMessageIdRequested(const QString &messageId);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAccountInitialized();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchRoomTextChanged();
    LIBRUQOLAWIDGETS_NO_EXPORT void clearFilterChannel();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOpenLinkRequested(const QString &link);
    LIBRUQOLAWIDGETS_NO_EXPORT void slotOpenTeamRequested(const QString &identifier);
    LIBRUQOLAWIDGETS_NO_EXPORT void applyChannelSelection();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSelectMessageRequested(const QString &messageId,
                                                               const QString &roomId,
                                                               ParseMessageUrlUtils::RoomIdType roomType,
                                                               ParseMessageUrlUtils::ChannelType channelType);

    ChannelListView *const mChannelView;
    QLineEdit *const mSearchRoomLineEdit;
    QPointer<RocketChatAccount> mCurrentRocketChatAccount;
};
