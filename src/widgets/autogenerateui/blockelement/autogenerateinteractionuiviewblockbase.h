/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDebug>
#include <QJsonObject>
#include <QString>
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewBlockBase : public QObject
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewBlockBase(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewBlockBase() override;
    [[nodiscard]] QString appId() const;
    void setAppId(const QString &newAppId);

    [[nodiscard]] QString blockId() const;
    void setBlockId(const QString &newBlockId);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewBlockBase &other) const;

    void parse(const QJsonObject &json);
    [[nodiscard]] QJsonObject serialize() const;

    virtual QWidget *generateWidget(RocketChatAccount *account, QWidget *parent);

    [[nodiscard]] QString type() const;
    void setType(const QString &newType);

Q_SIGNALS:
    void actionChanged(const QByteArray &blockId, const QString &value);

protected:
    virtual void serializeBlock(QJsonObject &obj) const = 0;
    virtual void parseBlock(const QJsonObject &obj) = 0;

private:
    QString mAppId;
    QString mBlockId;
    QString mType;
};
Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewBlockBase)
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewBlockBase &t);
