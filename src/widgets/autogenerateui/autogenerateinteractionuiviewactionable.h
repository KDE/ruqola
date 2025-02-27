/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QJsonObject>

class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewActionable();
    virtual ~AutoGenerateInteractionUiViewActionable();
    virtual void parse(const QJsonObject &json);

    [[nodiscard]] QByteArray appId() const;
    void setAppId(const QByteArray &newAppId);

    [[nodiscard]] QByteArray blockId() const;
    void setBlockId(const QByteArray &newBlockId);

    [[nodiscard]] QByteArray actionId() const;
    void setActionId(const QByteArray &newActionId);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewActionable &other) const;

    virtual QWidget *generateWidget(QWidget *parent);

    [[nodiscard]] QByteArray type() const;
    void setType(const QByteArray &newType);

    [[nodiscard]] virtual QJsonObject serialize() const;

private:
    QByteArray mAppId;
    QByteArray mBlockId;
    QByteArray mActionId;
    QByteArray mType;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewActionable)
// Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewActionable, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewActionable &t);
