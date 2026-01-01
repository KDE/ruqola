/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "reactions.h"
QT_IMPL_METATYPE_EXTERN_TAGGED(Reactions, Ruqola_Reactions)

#include "ruqola_message_memory_debug.h"
#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
Reactions::Reactions()

{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " Reactions created " << this;
}

Reactions::Reactions(const Reactions &other)
    : QSharedData(other)
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " Reactions(const Reactions &other) created " << this;
    mReactions = other.reactions();
}

Reactions::~Reactions()
{
    qCDebug(RUQOLA_MESSAGE_MEMORY_LOG) << " Reactions deleted " << this;
}

void Reactions::setReactions(const QList<Reaction> &reactions)
{
    mReactions = reactions;
}

QList<Reaction> Reactions::reactions() const
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
        const QJsonValue usernames = obj.value("usernames"_L1);
        if (!usernames.isUndefined()) {
            const QJsonArray array = usernames.toArray();
            for (int i = 0; i < array.count(); ++i) {
                users.append(array.at(i).toString());
            }
            if (!users.isEmpty()) {
                Reaction r;
                r.setReactionName(str, emojiManager);
                r.setUserNames(users);
                mReactions.append(std::move(r));
            }
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
        d.space() << t.reactions().at(i) << "\n";
    }
    return d;
}

QJsonObject Reactions::serialize(const Reactions &reactions)
{
    QJsonObject obj;
    for (int i = 0; i < reactions.reactions().count(); ++i) {
        QJsonObject react;
        react["usernames"_L1] = QJsonArray::fromStringList(reactions.reactions().at(i).userNames());
        obj[reactions.reactions().at(i).reactionName()] = react;
    }
    return obj;
}

Reactions *Reactions::deserialize(const QJsonObject &o, EmojiManager *emojiManager)
{
    QList<Reaction> reacts;
    const QStringList lst = o.keys();
    QStringList users;
    for (const QString &str : lst) {
        const QJsonObject obj = o.value(str).toObject();
        const QJsonValue usernames = obj.value("usernames"_L1);
        if (!usernames.isUndefined()) {
            const QJsonArray array = usernames.toArray();
            for (int i = 0; i < array.count(); ++i) {
                users.append(array.at(i).toString());
            }
            if (!users.isEmpty()) {
                Reaction r;
                r.setReactionName(str, emojiManager);
                r.setUserNames(users);
                reacts.append(std::move(r));
            }
        }
        users.clear();
    }
    auto final = new Reactions;
    final->setReactions(reacts);
    return final;
}

bool Reactions::isEmpty() const
{
    return mReactions.isEmpty();
}
