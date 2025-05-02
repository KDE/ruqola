/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "aitextinfo.h"

#include <QWidget>
class AiTextListView;
class AiTextMenuConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AiTextMenuConfigureWidget(QWidget *parent = nullptr);
    ~AiTextMenuConfigureWidget() override;

    void setAiTextInfos(const QList<AiTextInfo> &infos);
    [[nodiscard]] QList<AiTextInfo> aiTextInfos() const;

private:
    AiTextListView *const mListView;
};
