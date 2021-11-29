/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "reactions.h"
#include <QJsonArray>
#include <QJsonObject>

Reactions::Reactions() = default;

void Reactions::setReactions(const QVector<Reaction> &reactions)
{
    mReactions = reactions;
}

QVector<Reaction> Reactions::reactions() const
{
    return mReactions;
}

void Reactions::parseReactions(const QJsonObject &reacts, EmojiManager *emojiManager)
{
    mReactions.clear();
    const QStringList lst = reacts.keys();
    QStringList users;
    for (const QString &str : lst) {
        users.clear();
        const QJsonObject obj = reacts.value(str).toObject();
        const QJsonValue usernames = obj.value(QLatin1String("usernames"));
        if (!usernames.isUndefined()) {
            const QJsonArray array = usernames.toArray();
            for (int i = 0; i < array.count(); ++i) {
                users.append(array.at(i).toString());
            }
        }
        if (!users.isEmpty()) {
            Reaction r;
            r.setReactionName(str, emojiManager);
            r.setUserNames(users);
            mReactions.append(r);
        }
    }
}

bool Reactions::operator==(const Reactions &other) const
{
    return mReactions == other.reactions();
}

QDebug operator<<(QDebug d, const Reactions &t)
{
    for (int i = 0; i < t.reactions().count(); i++) {
        d << t.reactions().at(i) << "\n";
    }
    return d;
}

QJsonObject Reactions::serialize(const Reactions &reactions)
{
    QJsonObject obj;
    for (int i = 0; i < reactions.reactions().count(); ++i) {
        QJsonObject react;
        react[QLatin1String("usernames")] = QJsonArray::fromStringList(reactions.reactions().at(i).userNames());
        obj[reactions.reactions().at(i).reactionName()] = react;
    }
    return obj;
}

Reactions Reactions::fromJSon(const QJsonObject &o, EmojiManager *emojiManager)
{
    QVector<Reaction> reacts;
    const QStringList lst = o.keys();
    QStringList users;
    for (const QString &str : lst) {
        const QJsonObject obj = o.value(str).toObject();
        const QJsonValue usernames = obj.value(QLatin1String("usernames"));
        if (!usernames.isUndefined()) {
            const QJsonArray array = usernames.toArray();
            for (int i = 0; i < array.count(); ++i) {
                users.append(array.at(i).toString());
            }
        }
        if (!users.isEmpty()) {
            Reaction r;
            r.setReactionName(str, emojiManager);
            r.setUserNames(users);
            reacts.append(r);
        }
        users.clear();
    }
    Reactions final;
    final.setReactions(reacts);
    return final;
}

bool Reactions::isEmpty() const
{
    return mReactions.isEmpty();
}
