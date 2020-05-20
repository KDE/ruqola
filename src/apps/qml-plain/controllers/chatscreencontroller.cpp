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

#include "chatscreencontroller.h"

#include "accountmanager.h"
#include "rocketchataccount.h"

#include "model/listmessagesmodel.h"
#include "model/messagemodel.h"
#include "model/roomfilterproxymodel.h"
#include "model/roommodel.h"
#include "model/usersmodel.h"

#include "models/roomavatarsmodel.h"

#include "emoticons/emojimanager.h"
#include "emoticons/emojiqmlmodel.h"
#include "textconverter.h"

#include <QDebug>

namespace RuqolaQmlPlain {
namespace Controllers {

QHash<int, QUrl> ChatScreenController::sPageToUrl = {
    {(int)Page::HomePage, QUrl(QStringLiteral("qrc:/pages/HomePage.qml"))},
    {(int)Page::ChatPage, QUrl(QStringLiteral("qrc:/pages/ChatPage.qml"))},
    {(int)Page::SettingsPage, QUrl(QStringLiteral("qrc:/pages/SettingsPage.qml"))},
    {(int)Page::DirectoryPage, QUrl(QStringLiteral("qrc:/pages/DirectoryPage.qml"))}};

QHash<RocketChatAccount *, RoomAvatarsModel *> ChatScreenController::sRoomAvatarModelByAccount;

ChatScreenController::ChatScreenController(AccountManager *accountManager, QObject *parent)
    : QObject(parent)
    , mAccountManager(accountManager)
{
    connect(
        mAccountManager, &AccountManager::currentAccountChanged, this,
        &ChatScreenController::messageModelChanged);
    connect(
        mAccountManager, &AccountManager::currentAccountChanged, this,
        &ChatScreenController::roomsModelChanged);
    connect(
        mAccountManager, &AccountManager::currentAccountChanged, this,
        &ChatScreenController::usersModelChanged);
    connect(
        mAccountManager, &AccountManager::currentAccountChanged, this,
        &ChatScreenController::bindNewAccount);

    if (mAccountManager->account()) {
        mEmojiModel = new EmojiQmlModel(mAccountManager->account()->emojiManager());
    }
}

MessageModel *ChatScreenController::messageModel() const
{
    return mMessageModel;
}

RoomAvatarsModel *ChatScreenController::roomAvatarsModel() const
{
    if (mAccountManager->account() &&
        !sRoomAvatarModelByAccount.contains(mAccountManager->account())) {
        sRoomAvatarModelByAccount[mAccountManager->account()] =
            new RoomAvatarsModel(mAccountManager->account());
    }
    return sRoomAvatarModelByAccount.value(mAccountManager->account(), nullptr);
}

ListMessagesModel *ChatScreenController::listMessagesModel() const
{
    return mAccountManager->account()->listMessageModel();
}

RoomFilterProxyModel *ChatScreenController::roomsModel() const
{
    return mAccountManager->account()->roomFilterProxyModel();
}

void ChatScreenController::selectRoom(const QString &roomId)
{
    setAboutToChangeModel(true);
    setCurrentRoom(roomId);
    setMessageModel(mAccountManager->account()->messageModelForRoom(roomId));
}

UsersModel *ChatScreenController::usersModel() const
{
    return mAccountManager->account()->usersModel();
}

QString ChatScreenController::currentRoom() const
{
    return mCurrentRoomId;
}

void ChatScreenController::loadHistory()
{
    mAccountManager->account()->loadHistory(mCurrentRoomId);
}

void ChatScreenController::sendMessage(const QString &message)
{
    mAccountManager->account()->sendMessage(mCurrentRoomId, message);
}

void ChatScreenController::setMessageModel(MessageModel *model)
{
    if (model != mMessageModel) {
        if (mMessageModel) {
            mMessageModel->deactivate();
        }
        mMessageModel = model;
        if (mMessageModel) {
            mMessageModel->activate();
            mMessageModel->enableQmlHacks(true);
        }
        Q_EMIT messageModelChanged();
    }
}

void ChatScreenController::setCurrentRoom(const QString &roomId)
{
    if (roomId != mCurrentRoomId) {
        mCurrentRoomId = roomId;
        mAccountManager->account()->switchingToRoom(roomId);
        Q_EMIT currentRoomChanged();
    }
}

QString ChatScreenController::avatarUrl(const QString &userId) const
{
    if (!mAccountManager->account()) {
        return {};
    }
    return mAccountManager->account()->avatarUrl(userId);
}

QString ChatScreenController::roomAvatarUrl(const QString &roomId) const
{
    if (!mAccountManager->account()) {
        return {};
    }

    auto room = mAccountManager->account()->roomModel()->findRoom(roomId);

    if (room->channelType() == QLatin1String("d")) {
        // Getting user avatar
        // User u = mAccountManager->account()->usersModel()->fullUserInfo(room->name());
        qDebug() << room->name() << room->roomCreatorUserId() << *room;
        return avatarUrl(room->roomCreatorUserId());
    }

    // return mAccountManager->account()->avatarUrlByName()
    // mAccountManager->account()->avatarUrl()
    // return mAccountManager->account()->roomModel()->icon(
    //     mAccountManager->account()->roomModel()->findRoom(roomId));
    return {};
}

void ChatScreenController::bindNewAccount()
{
    // TODO: Hack, better implement setEmojiManager in EmojiQmlModel
    if (mEmojiModel) {
        delete mEmojiModel;
        mEmojiModel = nullptr;
        Q_EMIT emojiModelChanged();
    }
    if (mAccountManager->account()) {
        mEmojiModel = new EmojiQmlModel(mAccountManager->account()->emojiManager());
        Q_EMIT emojiModelChanged();
    }

    disconnect(nullptr, &UsersModel::dataChanged, this, &ChatScreenController::usersModelRefreshed);
    if (mAccountManager->account()) {
        connect(
            mAccountManager->account()->usersModel(), &UsersModel::dataChanged, this,
            &ChatScreenController::usersModelRefreshed);
    }
}

QString ChatScreenController::transform(const QString &txt)
{
    Message msg;
    msg.setText(txt);
    TextConverter tc(mAccountManager->account()->emojiManager());
    return tc.convertMessageText(txt, QStringLiteral(""), {msg});
}

void ChatScreenController::goToHomePage()
{
    mCurrentPage = HomePage;
    Q_EMIT currentPageChanged();
    Q_EMIT currentPageUrlChanged();
}

void ChatScreenController::goToChatPage()
{
    mCurrentPage = ChatPage;
    Q_EMIT currentPageChanged();
    Q_EMIT currentPageUrlChanged();
}

void ChatScreenController::goToSettingsPage()
{
    mCurrentPage = SettingsPage;
    Q_EMIT currentPageChanged();
    Q_EMIT currentPageUrlChanged();
}

void ChatScreenController::goToDirectoryPage()
{
    mCurrentPage = DirectoryPage;
    Q_EMIT currentPageChanged();
    Q_EMIT currentPageUrlChanged();
}

bool ChatScreenController::aboutToChangeModel() const
{
    return mAboutToChangeModel;
}

void ChatScreenController::setAboutToChangeModel(bool about)
{
    if (about != mAboutToChangeModel) {
        mAboutToChangeModel = about;
        Q_EMIT aboutToChangeModelChanged();
    }
}

void ChatScreenController::resetAboutToChangeModel()
{
    setAboutToChangeModel(false);
}

ChatScreenController::Page ChatScreenController::currentPage() const
{
    return mCurrentPage;
}

QUrl ChatScreenController::currentPageUrl() const
{
    return sPageToUrl[mCurrentPage];
}

EmojiQmlModel *ChatScreenController::emojiModel() const
{
    return mEmojiModel;
}

void ChatScreenController::toggleMessageReaction(const QString &messageId, const QString &emoji)
{
    const auto &msg = mMessageModel->findMessageById(messageId);
    const auto &reactions = msg.reactions();
    for (const auto &reaction : reactions.reactions()) {
        if (reaction.reactionName() == emoji) {
            const bool doAdd =
                !reaction.userNames().contains(mAccountManager->account()->userName());
            reactToMessage(messageId, emoji, doAdd);
            return;
        }
    }
}

void ChatScreenController::reactToMessage(
    const QString &messageId, const QString &emoji, bool addRemoveFlag)
{
    // TODO: make sure account is valid?
    mAccountManager->account()->reactOnMessage(messageId, emoji, addRemoveFlag);
}

void ChatScreenController::removeMessage(const QString &roomId, const QString &messageId)
{
    // TODO: make sure account is valid?
    mAccountManager->account()->deleteMessage(messageId, roomId);
}

QUrl ChatScreenController::attachmentUrl(const QString &link)
{
    return mAccountManager->account()->attachmentUrl(link);
}

QString ChatScreenController::attachmentUrlLocalFile(const QString &link)
{
    return mAccountManager->account()->attachmentUrl(link).toLocalFile();
}

} // namespace Controllers
} // namespace RuqolaQmlPlain
