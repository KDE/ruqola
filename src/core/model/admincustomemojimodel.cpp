/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "admincustomemojimodel.h"
#include "emoticons/emojimanager.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QIcon>

AdminCustomEmojiModel::AdminCustomEmojiModel(RocketChatAccount *account, QObject *parent)
    : CustomBaseModel(parent)
    , mRocketChatAccount(account)
{
}

AdminCustomEmojiModel::~AdminCustomEmojiModel() = default;

int AdminCustomEmojiModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mCustomEmojiList.count();
}

QVariant AdminCustomEmojiModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<CustomEmojiRoles>(section)) {
        case CustomEmojiRoles::Name:
            return i18n("Name");
        case CustomEmojiRoles::Identifier:
            return i18n("Identifier");
        case CustomEmojiRoles::Aliases:
            return i18n("Aliases");
        case CustomEmojiRoles::Icon:
            return {};
        }
    }
    return {};
}

int AdminCustomEmojiModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(CustomEmojiRoles::LastColumn) + 1;
}

QVariant AdminCustomEmojiModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mCustomEmojiList.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const CustomEmoji &customEmoji = mCustomEmojiList.at(index.row());
    const int col = index.column();
    switch (static_cast<CustomEmojiRoles>(col)) {
    case CustomEmojiRoles::Name:
        return customEmoji.name();
    case CustomEmojiRoles::Identifier:
        return customEmoji.identifier();
    case CustomEmojiRoles::Aliases:
        return customEmoji.aliases().join(QLatin1Char(','));
    case CustomEmojiRoles::Icon:
        return createCustomIcon(customEmoji.identifier());
    }
    return {};
}

int AdminCustomEmojiModel::total() const
{
    return mCustomEmojiList.count();
}

void AdminCustomEmojiModel::parseElements(const QJsonObject &obj)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mCustomEmojiList.count() - 1);
        mCustomEmojiList.clear();
        endRemoveRows();
    }
    mCustomEmojiList.parseCustomEmojis(obj);
    if (!mCustomEmojiList.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mCustomEmojiList.count() - 1);
        endInsertRows();
    }
    checkFullList();
    Q_EMIT totalChanged();
}

void AdminCustomEmojiModel::checkFullList()
{
    setHasFullList(mCustomEmojiList.count() == mCustomEmojiList.total());
}

const CustomEmojisInfo &AdminCustomEmojiModel::customEmojis() const
{
    return mCustomEmojiList;
}

void AdminCustomEmojiModel::setCustomEmojis(const CustomEmojisInfo &newCustomEmojis)
{
    if (rowCount() != 0) {
        beginRemoveRows(QModelIndex(), 0, mCustomEmojiList.count() - 1);
        mCustomEmojiList.clear();
        endRemoveRows();
    }
    if (!mCustomEmojiList.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, mCustomEmojiList.count() - 1);
        mCustomEmojiList = newCustomEmojis;
        endInsertRows();
    }
}

void AdminCustomEmojiModel::removeElement(const QString &identifier)
{
    const int userCount = mCustomEmojiList.count();
    for (int i = 0; i < userCount; ++i) {
        if (mCustomEmojiList.at(i).identifier() == identifier) {
            beginRemoveRows(QModelIndex(), i, i);
            mCustomEmojiList.takeAt(i);
            mCustomEmojiList.setTotal(mCustomEmojiList.count()); // Update total
            endRemoveRows();
            Q_EMIT totalChanged();
            break;
        }
    }
}

void AdminCustomEmojiModel::addMoreElements(const QJsonObject &obj)
{
    const int numberOfElement = mCustomEmojiList.count();
    mCustomEmojiList.parseCustomEmojis(obj);
    beginInsertRows(QModelIndex(), numberOfElement, mCustomEmojiList.count() - 1);
    endInsertRows();
    checkFullList();
}

QList<int> AdminCustomEmojiModel::hideColumns() const
{
    return {CustomEmojiRoles::Identifier, CustomEmojiRoles::Icon};
}

QIcon AdminCustomEmojiModel::createCustomIcon(const QString &identifier) const
{
    if (mRocketChatAccount) {
        const QString fileName = mRocketChatAccount->emojiManager()->customEmojiFileNameFromIdentifier(identifier);
        if (!fileName.isEmpty()) {
            const QUrl emojiUrl = mRocketChatAccount->attachmentUrlFromLocalCache(fileName);
            if (!emojiUrl.isEmpty()) {
                const QIcon icon(emojiUrl.toLocalFile());
                return icon;
            }
        }
    }
    return {};
}

#include "moc_admincustomemojimodel.cpp"
