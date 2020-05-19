/*
 * Copyright 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <QDebug>
#include <QHash>
#include <QObject>
#include <QUrl>

class AccountManager;
class EmojiQmlModel;
class ListMessagesModel;
class MessageModel;
class RoomFilterProxyModel;
class UsersModel;

namespace RuqolaQmlPlain {
namespace Controllers {

class ChatScreenController : public QObject
{
    Q_OBJECT

  public:
    enum Page {
        HomePage,
        ChatPage,
        SettingsPage,
        DirectoryPage,
    };
    Q_ENUM(Page);

  private:
    Q_PROPERTY(MessageModel *messageModel READ messageModel NOTIFY messageModelChanged)
    Q_PROPERTY(
        ListMessagesModel *listMessagesModel READ listMessagesModel NOTIFY listMessagesModelChanged)
    Q_PROPERTY(RoomFilterProxyModel *roomsModel READ roomsModel NOTIFY roomsModelChanged)
    Q_PROPERTY(UsersModel *usersModel READ usersModel NOTIFY usersModelChanged)
    Q_PROPERTY(QString currentRoom READ currentRoom NOTIFY currentRoomChanged)
    Q_PROPERTY(RuqolaQmlPlain::Controllers::ChatScreenController::Page currentPage READ currentPage
                   NOTIFY currentPageChanged)
    Q_PROPERTY(QUrl currentPageUrl READ currentPageUrl NOTIFY currentPageUrlChanged)
    Q_PROPERTY(EmojiQmlModel *emojiModel READ emojiModel NOTIFY emojiModelChanged)
    // Used to make sure that any intermediate contentY change during the model
    // update doesn't trigger loadHistory
    Q_PROPERTY(bool aboutToChangeModel READ aboutToChangeModel NOTIFY aboutToChangeModelChanged)

  public:
    ChatScreenController(AccountManager *accountManager, QObject *parent = nullptr);

    ListMessagesModel *listMessagesModel() const;
    MessageModel *messageModel() const;
    RoomFilterProxyModel *roomsModel() const;
    UsersModel *usersModel() const;
    QString currentRoom() const;
    Page currentPage() const;
    QUrl currentPageUrl() const;
    EmojiQmlModel *emojiModel() const;

    Q_REQUIRED_RESULT bool aboutToChangeModel() const;
    Q_INVOKABLE QString avatarUrl(const QString &) const;
    Q_INVOKABLE QString roomAvatarUrl(const QString &) const;

    Q_INVOKABLE void selectRoom(const QString &roomId);
    Q_INVOKABLE void loadHistory();
    Q_INVOKABLE void sendMessage(const QString &message);
    Q_INVOKABLE QString transform(const QString &msg);
    Q_INVOKABLE void resetAboutToChangeModel();
    Q_INVOKABLE void toggleMessageReaction(const QString &messageId, const QString &emoji);
    Q_INVOKABLE void
    reactToMessage(const QString &messageId, const QString &emoji, bool addRemoveFlag);
    Q_INVOKABLE void removeMessage(const QString &roomId, const QString &messageId);
    Q_INVOKABLE QUrl attachmentUrl(const QString &link);
    Q_INVOKABLE QString attachmentUrlLocalFile(const QString &link);

    // Called from the buttons on the top of the room list
    Q_INVOKABLE void goToHomePage();
    Q_INVOKABLE void goToChatPage();
    Q_INVOKABLE void goToSettingsPage();
    Q_INVOKABLE void goToDirectoryPage();

  Q_SIGNALS:
    void messageModelChanged();
    void listMessagesModelChanged();
    void roomsModelChanged();
    void usersModelChanged();
    void currentRoomChanged();
    void usersModelRefreshed();
    void currentPageChanged();
    void currentPageUrlChanged();
    void aboutToChangeModelChanged();
    void emojiModelChanged();

    void emojiInserted(const QString &emojiText);

  private:
    AccountManager *mAccountManager = nullptr;
    MessageModel *mMessageModel = nullptr;
    ListMessagesModel *mListMessagesModel = nullptr;
    RoomFilterProxyModel *mRoomsModel = nullptr;
    UsersModel *mUsersModel = nullptr;
    Page mCurrentPage = HomePage;

    QString mCurrentRoomId;
    bool mAboutToChangeModel = false;

    EmojiQmlModel *mEmojiModel = nullptr;

    static QHash<int, QUrl> sPageToUrl;

    void setMessageModel(MessageModel *);
    void setCurrentRoom(const QString &);
    void setAboutToChangeModel(bool about);

    void bindNewAccount();
};

} // namespace Controllers
} // namespace RuqolaQmlPlain
