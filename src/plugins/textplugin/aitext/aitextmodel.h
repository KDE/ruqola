/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "aitextinfo.h"

#include <QAbstractListModel>

class AiTextModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AiTextModel(QObject *parent = nullptr);
    ~AiTextModel() override;

    enum AiTextRoles : uint16_t {
        TextRole = Qt::UserRole + 1,
        EnabledRole,
    };

    [[nodiscard]] int rowCount(const QModelIndex & = {}) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

    [[nodiscard]] QList<AiTextInfo> infos() const;
    void setInfos(const QList<AiTextInfo> &newAskItems);

    void addItem(const AiTextInfo &msg);
    void removeInfo(int index);

private:
    QList<AiTextInfo> mTextInfos;
};
