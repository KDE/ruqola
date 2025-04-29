/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "aitextinfo.h"

#include <QListView>
class AiTextModel;
class AiTextListView : public QListView
{
    Q_OBJECT
public:
    explicit AiTextListView(QWidget *parent = nullptr);
    ~AiTextListView() override;

    void setAiTextInfos(const QList<AiTextInfo> &infos);
    [[nodiscard]] QList<AiTextInfo> aiTextInfos() const;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    AiTextModel *const mModel;
};
