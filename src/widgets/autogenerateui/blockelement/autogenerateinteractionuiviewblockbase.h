/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QJsonObject>
#include <QString>
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewBlockBase : public QObject
{
    Q_OBJECT
public:
    struct StateInfo {
        QString actionId;
        QVariant value;
    };
    explicit AutoGenerateInteractionUiViewBlockBase(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewBlockBase() override;
    [[nodiscard]] QByteArray appId() const;
    void setAppId(const QByteArray &newAppId);

    [[nodiscard]] QByteArray blockId() const;
    void setBlockId(const QByteArray &newBlockId);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewBlockBase &other) const;

    void parse(const QJsonObject &json);
    [[nodiscard]] QJsonObject serialize() const;

    virtual QWidget *generateWidget(QWidget *parent);

    [[nodiscard]] QByteArray type() const;
    void setType(const QByteArray &newType);

    [[nodiscard]] QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> serializeState() const;
    virtual void assignState(const QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> &info) = 0;
    virtual void setErrorMessages(const QMap<QString, QString> &map) = 0;
Q_SIGNALS:
    void actionChanged(const QByteArray &blockId, const QByteArray &actionId, const QString &value);

protected:
    virtual void serializeBlock(QJsonObject &obj) const = 0;
    [[nodiscard]] virtual QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> serializeBlockState() const = 0;
    virtual void parseBlock(const QJsonObject &obj) = 0;

protected:
    QByteArray mAppId;
    QByteArray mBlockId;
    QByteArray mType;
};

class QDebug;
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewBlockBase &t);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewBlockBase::StateInfo &t);
