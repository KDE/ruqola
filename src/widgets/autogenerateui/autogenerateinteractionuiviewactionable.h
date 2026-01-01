/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QObject>
class QJsonObject;
class ActionElementWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewActionable : public QObject
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewActionable(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewActionable() override;
    virtual void parse(const QJsonObject &json);

    [[nodiscard]] QByteArray appId() const;
    void setAppId(const QByteArray &newAppId);

    [[nodiscard]] QByteArray blockId() const;
    void setBlockId(const QByteArray &newBlockId);

    [[nodiscard]] QByteArray actionId() const;
    void setActionId(const QByteArray &newActionId);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewActionable &other) const;

    virtual ActionElementWidget *generateWidget(QWidget *parent);

    [[nodiscard]] QByteArray type() const;
    void setType(const QByteArray &newType);

    [[nodiscard]] QJsonObject serialize() const;

    virtual QVariant currentValue() const;
    virtual void setCurrentValue(const QVariant &);
    void setErrorMessages(const QMap<QString, QString> &map);
Q_SIGNALS:
    void actionChanged(const QByteArray &actionId, const QString &values);

protected:
    virtual void serializeElement(QJsonObject &o) const = 0;
    virtual void parseElement(const QJsonObject &json) = 0;

protected:
    QByteArray mAppId;
    QByteArray mBlockId;
    QByteArray mActionId;
    QByteArray mType;
    ActionElementWidget *mActionElementWidget = nullptr;
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewActionable &t);
