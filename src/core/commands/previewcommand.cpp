/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "previewcommand.h"

using namespace Qt::Literals::StringLiterals;
PreviewCommand::PreviewCommand() = default;

PreviewCommand::~PreviewCommand() = default;

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

QString PreviewCommand::type() const
{
    return mType;
}

void PreviewCommand::setType(const QString &newType)
{
    mType = newType;
}
