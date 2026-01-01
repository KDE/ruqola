/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>

#include "libruqolawidgets_private_export.h"
#include "model/listmessagesfilterproxymodel.h"
class MessageListView;
class QLabel;
class ListMessagesFilterProxyModel;
class RocketChatAccount;
class Room;
class SearchMessageLineWidget;
namespace TextEditTextToSpeech
{
class TextToSpeechContainerWidget;
}

class LIBRUQOLAWIDGETS_TESTS_EXPORT ShowListMessageBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowListMessageBaseWidget(RocketChatAccount *account, QWidget *parent = nullptr);
    ~ShowListMessageBaseWidget() override;

    void setFilterProxyModel(ListMessagesFilterProxyModel *model);
    [[nodiscard]] MessageListView *messageListView() const;

    void setRoom(Room *room);

    void addMessageLineWidget(QWidget *w);

    void setFilteringByType(ListMessagesFilterProxyModel::FilteringByType type);
Q_SIGNALS:
    void loadMoreElements();

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void updateLabel();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotSearchMessageTextChanged(const QString &str);

    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT QString displayShowMessageInRoom() const;
    SearchMessageLineWidget *const mSearchMessageLineWidget;
    QLabel *const mMessageListInfo;
    MessageListView *const mMessageListView;
    ListMessagesFilterProxyModel *mModel = nullptr;
    TextEditTextToSpeech::TextToSpeechContainerWidget *const mTextToSpeechWidget = nullptr;
};
