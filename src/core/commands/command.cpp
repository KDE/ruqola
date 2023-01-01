/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "command.h"
#include "ruqola_commands_debug.h"
#include <KLocalizedString>
#include <QJsonArray>

Command::Command() = default;

QString Command::params() const
{
    return mParams;
}

void Command::setParams(const QString &params)
{
    mParams = params;
    convertParamsI18n();
}

QString Command::commandName() const
{
    return mCommandName;
}

void Command::setCommandName(const QString &commandName)
{
    mCommandName = commandName;
}

QString Command::description() const
{
    return mDescription;
}

void Command::setDescription(const QString &description)
{
    mDescription = description;
    convertDescriptionI18n();
}

void Command::convertParamsI18n()
{
    if (mParams.isEmpty()) {
        return;
    }
    if (mParams == QLatin1String("Slash_Status_Params")) {
        mTranslatedParams = i18n("Status message");
    } else if (mParams == QLatin1String("Slash_Topic_Params")) {
        mTranslatedParams = i18n("Topic message");
    } else if (mParams == QLatin1String("your_message")) {
        mTranslatedParams = i18n("your message");
    } else if (mParams == QLatin1String("your_message_optional")) {
        mTranslatedParams = i18n("your message (optional)");
    } else {
        mTranslatedParams = mParams;
    }
}

QString Command::translatedParams() const
{
    return mTranslatedParams;
}

void Command::convertDescriptionI18n()
{
    if (mDescription.isEmpty()) {
        return;
    } else if (mDescription == QLatin1String("Archive")) {
        mTranslatedDescription = i18n("Archive");
    } else if (mDescription == QLatin1String("Slash_Gimme_Description")) {
        mTranslatedDescription = i18n("Displays ༼ つ ◕_◕ ༽つ before your message");
    } else if (mDescription == QLatin1String("Slash_LennyFace_Description")) {
        mTranslatedDescription = i18n("Displays ( ͡° ͜ʖ ͡°) after your message");
    } else if (mDescription == QLatin1String("Slash_Shrug_Description")) {
        mTranslatedDescription = i18n("Displays ¯\\_(ツ)_/¯ after your message");
    } else if (mDescription == QLatin1String("Slash_Tableflip_Description")) {
        mTranslatedDescription = i18n("Displays (╯°□°）╯︵ ┻━┻");
    } else if (mDescription == QLatin1String("Slash_TableUnflip_Description")) {
        mTranslatedDescription = i18n("Displays ┬─┬<feff> ノ( ゜-゜ノ)");
    } else if (mDescription == QLatin1String("Create_A_New_Channel")) {
        mTranslatedDescription = i18n("Create a New Channel");
    } else if (mDescription == QLatin1String("Show_the_keyboard_shortcut_list")) {
        mTranslatedDescription = i18n("Show the keyboard shortcut list");
    } else if (mDescription == QLatin1String("Hide_room")) {
        mTranslatedDescription = i18n("Hide Room");
    } else if (mDescription == QLatin1String("Invite_user_to_join_channel")) {
        mTranslatedDescription = i18n("Invite one user to join this channel");
    } else if (mDescription == QLatin1String("Invite_user_to_join_channel_all_to")) {
        mTranslatedDescription = i18n("Invite all users from this channel to join [#channel]");
    } else if (mDescription == QLatin1String("Invite_user_to_join_channel_all_from")) {
        mTranslatedDescription = i18n("Invite all users from [#channel] to join this channel");
    } else if (mDescription == QLatin1String("Join_the_given_channel")) {
        mTranslatedDescription = i18n("Join the given channel");
    } else if (mDescription == QLatin1String("Remove_someone_from_room")) {
        mTranslatedDescription = i18n("Remove someone from the room");
    } else if (mDescription == QLatin1String("Leave_the_current_channel")) {
        mTranslatedDescription = i18n("Leave the current channel");
    } else if (mDescription == QLatin1String("Displays_action_text")) {
        mTranslatedDescription = i18n("Displays action text");
    } else if (mDescription == QLatin1String("Direct_message_someone")) {
        mTranslatedDescription = i18n("Direct message someone");
    } else if (mDescription == QLatin1String("Mute_someone_in_room")) {
        mTranslatedDescription = i18n("Mute someone in the room");
    } else if (mDescription == QLatin1String("Unmute_someone_in_room")) {
        mTranslatedDescription = i18n("Unmute someone in the room");
    } else if (mDescription == QLatin1String("Slash_Status_Description")) {
        mTranslatedDescription = i18n("Set your status message");
    } else if (mDescription == QLatin1String("Slash_Topic_Description")) {
        mTranslatedDescription = i18n("Set topic");
    } else if (mDescription == QLatin1String("Unarchive")) {
        mTranslatedDescription = i18n("Unarchive");
    } else {
        mTranslatedDescription = mDescription;
    }
}

bool Command::clientOnly() const
{
    return mClientOnly;
}

void Command::setClientOnly(bool clientOnly)
{
    mClientOnly = clientOnly;
}

void Command::parseCommand(const QJsonObject &obj)
{
    // qDebug() << "Command::parseCommand " << obj;
    mProvidesPreview = obj.value(QStringLiteral("providesPreview")).toBool();
    mClientOnly = obj.value(QStringLiteral("clientOnly")).toBool();
    // Add "/" for completion.
    mCommandName = QLatin1Char('/') + obj.value(QStringLiteral("command")).toString();
    setDescription(obj.value(QStringLiteral("description")).toString());
    setParams(obj.value(QStringLiteral("params")).toString());

    const QJsonValue permissionValue = obj.value(QStringLiteral("permission"));
    if (!permissionValue.isUndefined()) {
        if (permissionValue.isString()) {
            mPermissions.append(permissionValue.toString());
        } else if (permissionValue.isArray()) {
            const QJsonArray permissionArray = permissionValue.toArray();
            for (int i = 0, total = permissionArray.size(); i < total; ++i) {
                mPermissions.append(permissionArray.at(i).toString());
            }
        } else {
            qCWarning(RUQOLA_COMMANDS_LOG) << " Invalid permission type " << obj;
        }
    }
}

bool Command::operator==(const Command &other) const
{
    return mParams == other.params() && mCommandName == other.commandName() && mDescription == other.description() && mClientOnly == other.clientOnly()
        && mProvidesPreview == other.providesPreview() && mPermissions == other.permissions();
}

bool Command::isValid() const
{
    return !mCommandName.isEmpty();
}

bool Command::providesPreview() const
{
    return mProvidesPreview;
}

void Command::setProvidesPreview(bool providesPreview)
{
    mProvidesPreview = providesPreview;
}

QStringList Command::permissions() const
{
    return mPermissions;
}

void Command::setPermissions(const QStringList &permissions)
{
    mPermissions = permissions;
}

QString Command::translatedDescription() const
{
    return mTranslatedDescription;
}

QDebug operator<<(QDebug d, const Command &t)
{
    d << " mParams : " << t.params();
    d << " mCommandName : " << t.commandName();
    d << " mDescription : " << t.description();
    d << " mClientOnly : " << t.clientOnly();
    d << " mProvidesPreview : " << t.providesPreview();
    d << " mPermissions : " << t.permissions();
    return d;
}
