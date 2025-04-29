/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "aitextinfo.h"

#include <QObject>

class AiTextManager : public QObject
{
    Q_OBJECT
public:
    explicit AiTextManager(QObject *parent = nullptr);
    ~AiTextManager() override;

    void load();
    void save();

    [[nodiscard]] QList<AiTextInfo> textInfos() const;
    void setTextInfos(const QList<AiTextInfo> &newTextInfos);

private:
    QList<AiTextInfo> mTextInfos;
};
