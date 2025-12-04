/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
#include "libruqolawidgets_private_export.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewDividerBlock : public AutoGenerateInteractionUiViewBlockBase
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewDividerBlock(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewDividerBlock() override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewDividerBlock &other) const;
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;
    void setErrorMessages(const QMap<QString, QString> &map) override;
    void assignState(const QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> &info) override;

protected:
    void parseBlock(const QJsonObject &json) override;
    void serializeBlock(QJsonObject &o) const override;
    [[nodiscard]] QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> serializeBlockState() const override;
};

class QDebug;
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewDividerBlock &t);
