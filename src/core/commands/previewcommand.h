/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolacore_export.h"
#include <QMetaType>
#include <QString>

class LIBRUQOLACORE_EXPORT PreviewCommand
{
    Q_GADGET
public:
    enum class TypePreview : uint8_t {
        Unknown,
        Image,
        Video,
        Audio,
        Text,
        Other,
    };
    Q_ENUM(TypePreview);

    PreviewCommand();
    ~PreviewCommand();
    [[nodiscard]] QString id() const;
    void setId(const QString &newId);

    [[nodiscard]] QString value() const;
    void setValue(const QString &newValue);

    [[nodiscard]] PreviewCommand::TypePreview type() const;
    void setType(PreviewCommand::TypePreview newType);

    [[nodiscard]] QString typeStr() const;

    void parse(const QJsonObject &obj);
    [[nodiscard]] bool operator==(const PreviewCommand &other) const;

    [[nodiscard]] bool isValid() const;

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT PreviewCommand::TypePreview convertStringToPreviewType(const QString &str) const;
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT QString convertPreviewTypeToString() const;
    QString mId;
    QString mValue;
    PreviewCommand::TypePreview mType = PreviewCommand::TypePreview::Unknown;
};

QT_DECL_METATYPE_EXTERN_TAGGED(PreviewCommand, Ruqola_PreviewCommand, LIBRUQOLACORE_EXPORT)
Q_DECLARE_TYPEINFO(PreviewCommand, Q_RELOCATABLE_TYPE);
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const PreviewCommand &t);
