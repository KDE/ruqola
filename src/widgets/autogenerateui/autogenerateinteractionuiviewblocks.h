/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateui/blockelement/autogenerateinteractionuiviewblockbase.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonArray>
#include <QList>
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewBlocks : public QObject
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewBlocks(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewBlocks() override;
    void parse(const QJsonArray &array);
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewBlocks &other) const;

    [[nodiscard]] QList<AutoGenerateInteractionUiViewBlockBase *> blockElements() const;
    void setBlockElements(const QList<AutoGenerateInteractionUiViewBlockBase *> &newBlockElements);

    void generateWidget(QWidget *parent);
    [[nodiscard]] QJsonArray serialize() const;

    [[nodiscard]] QJsonObject serializeState() const;
    void setErrorMessages(const QMap<QString, QString> &map);

    [[nodiscard]] QMap<QString, QList<AutoGenerateInteractionUiViewBlockBase::StateInfo>> createStateInfos() const;
    void assignState(const QMap<QString, QList<AutoGenerateInteractionUiViewBlockBase::StateInfo>> &map);

Q_SIGNALS:
    void actionChanged(const QByteArray &blockId, const QByteArray &actionId, const QString &value);

private:
    QList<AutoGenerateInteractionUiViewBlockBase *> mBlockElements;
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewBlocks &t);
