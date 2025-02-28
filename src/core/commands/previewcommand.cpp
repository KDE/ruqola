/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "previewcommand.h"
#include "ruqola_commands_debug.h"
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;
PreviewCommand::PreviewCommand() = default;

PreviewCommand::~PreviewCommand() = default;

PreviewCommand::TypePreview PreviewCommand::convertStringToPreviewType(const QString &str) const
{
    if (str == "image"_L1) {
        return PreviewCommand::TypePreview::Image;
    } else if (str == "video"_L1) {
        return PreviewCommand::TypePreview::Video;
    } else if (str == "audio"_L1) {
        return PreviewCommand::TypePreview::Audio;
    } else if (str == "text"_L1) {
        return PreviewCommand::TypePreview::Text;
    } else if (str == "other"_L1) {
        return PreviewCommand::TypePreview::Other;
    } else {
        qCWarning(RUQOLA_COMMANDS_LOG) << "Invalid preview command type " << str;
    }
    return PreviewCommand::TypePreview::Unknown;
}

QDebug operator<<(QDebug d, const PreviewCommand &t)
{
    d.space() << "id:" << t.id();
    d.space() << "value:" << t.value();
    d.space() << "type:" << t.type();
    return d;
}

QString PreviewCommand::id() const
{
    return mId;
}

void PreviewCommand::setId(const QString &newId)
{
    mId = newId;
}

QString PreviewCommand::value() const
{
    return mValue;
}

void PreviewCommand::setValue(const QString &newValue)
{
    mValue = newValue;
}

PreviewCommand::TypePreview PreviewCommand::type() const
{
    return mType;
}

void PreviewCommand::setType(PreviewCommand::TypePreview newType)
{
    mType = newType;
}

void PreviewCommand::parse(const QJsonObject &obj)
{
    mType = convertStringToPreviewType(obj["type"_L1].toString());
    mValue = obj["value"_L1].toString();
    mId = obj["id"_L1].toString();
}

bool PreviewCommand::operator==(const PreviewCommand &other) const
{
    return other.id() == id() && other.type() == type() && other.value() == value();
}

bool PreviewCommand::isValid() const
{
    return mType != PreviewCommand::TypePreview::Unknown && !mId.isEmpty() && !mValue.isEmpty();
}

#include "moc_previewcommand.cpp"
