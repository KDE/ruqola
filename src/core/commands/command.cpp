/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "command.h"
#include "ruqola_commands_debug.h"
#include <KLocalizedString>
#include <QJsonArray>

#include <QJsonObject>

QT_IMPL_METATYPE_EXTERN_TAGGED(Command, Ruqola_Command)
using namespace Qt::Literals::StringLiterals;

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
    if (mParams == "Slash_Status_Params"_L1) {
        mTranslatedParams = i18n("Status message");
    } else if (mParams == "Slash_Topic_Params"_L1) {
        mTranslatedParams = i18n("Topic message");
    } else if (mParams == "your_message"_L1) {
        mTranslatedParams = i18n("your message");
    } else if (mParams == "your_message_optional"_L1) {
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
    } else if (mDescription == "Archive"_L1) {
        mTranslatedDescription = i18n("Archive");
    } else if (mDescription == "Slash_Gimme_Description"_L1) {
        mTranslatedDescription = i18n("Displays ༼ つ ◕_◕ ༽つ before your message");
    } else if (mDescription == "Slash_LennyFace_Description"_L1) {
        mTranslatedDescription = i18n("Displays ( ͡° ͜ʖ ͡°) after your message");
    } else if (mDescription == "Slash_Shrug_Description"_L1) {
        mTranslatedDescription = i18n("Displays ¯\\_(ツ)_/¯ after your message");
    } else if (mDescription == "Slash_Tableflip_Description"_L1) {
        mTranslatedDescription = i18n("Displays (╯°□°）╯︵ ┻━┻");
    } else if (mDescription == "Slash_TableUnflip_Description"_L1) {
        mTranslatedDescription = i18n("Displays ┬─┬<feff> ノ( ゜-゜ノ)");
    } else if (mDescription == "Create_A_New_Channel"_L1) {
        mTranslatedDescription = i18n("Create a New Channel");
    } else if (mDescription == "Show_the_keyboard_shortcut_list"_L1) {
        mTranslatedDescription = i18n("Show the keyboard shortcut list");
    } else if (mDescription == "Hide_room"_L1) {
        mTranslatedDescription = i18n("Hide Room");
    } else if (mDescription == "Invite_user_to_join_channel"_L1) {
        mTranslatedDescription = i18n("Invite one user to join this channel");
    } else if (mDescription == "Invite_user_to_join_channel_all_to"_L1) {
        mTranslatedDescription = i18n("Invite all users from this channel to join [#channel]");
    } else if (mDescription == "Invite_user_to_join_channel_all_from"_L1) {
        mTranslatedDescription = i18n("Invite all users from [#channel] to join this channel");
    } else if (mDescription == "Join_the_given_channel"_L1) {
        mTranslatedDescription = i18n("Join the given channel");
    } else if (mDescription == "Remove_someone_from_room"_L1) {
        mTranslatedDescription = i18n("Remove someone from the room");
    } else if (mDescription == "Leave_the_current_channel"_L1) {
        mTranslatedDescription = i18n("Leave the current channel");
    } else if (mDescription == "Displays_action_text"_L1) {
        mTranslatedDescription = i18n("Displays action text");
    } else if (mDescription == "Direct_message_someone"_L1) {
        mTranslatedDescription = i18n("Direct message someone");
    } else if (mDescription == "Mute_someone_in_room"_L1) {
        mTranslatedDescription = i18n("Mute someone in the room");
    } else if (mDescription == "Unmute_someone_in_room"_L1) {
        mTranslatedDescription = i18n("Unmute someone in the room");
    } else if (mDescription == "Slash_Status_Description"_L1) {
        mTranslatedDescription = i18n("Set your status message");
    } else if (mDescription == "Slash_Topic_Description"_L1) {
        mTranslatedDescription = i18n("Set topic");
    } else if (mDescription == "Unarchive"_L1) {
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
    mProvidesPreview = obj.value("providesPreview"_L1).toBool();
    mClientOnly = obj.value("clientOnly"_L1).toBool();
    // Add "/" for completion.
    mCommandName = u'/' + obj.value("command"_L1).toString();
    setDescription(obj.value("description"_L1).toString());
    setParams(obj.value("params"_L1).toString());

    const QJsonValue permissionValue = obj.value("permission"_L1);
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
    d.space() << "mParams:" << t.params();
    d.space() << "mCommandName:" << t.commandName();
    d.space() << "mDescription:" << t.description();
    d.space() << "mClientOnly:" << t.clientOnly();
    d.space() << "mProvidesPreview:" << t.providesPreview();
    d.space() << "mPermissions:" << t.permissions();
    return d;
}
