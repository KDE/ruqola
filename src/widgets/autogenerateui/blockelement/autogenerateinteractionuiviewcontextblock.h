/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateui/blockelement/autogenerateinteractionuiviewblockbase.h"
#include "libruqolawidgets_private_export.h"
#include <QDebug>
class AutoGenerateInteractionUiViewElement;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewContextBlock : public AutoGenerateInteractionUiViewBlockBase
{
public:
    AutoGenerateInteractionUiViewContextBlock();
    ~AutoGenerateInteractionUiViewContextBlock() override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewContextBlock &other) const;
    [[nodiscard]] QWidget *generateWidget(QWidget *parent) const override;

    [[nodiscard]] QList<AutoGenerateInteractionUiViewElement *> elements() const;
    void setElements(const QList<AutoGenerateInteractionUiViewElement *> &newElements);

protected:
    void serializeBlock(QJsonObject &o) const override;
    void parseBlock(const QJsonObject &json) override;

private:
    QList<AutoGenerateInteractionUiViewElement *> mElements;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewContextBlock)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewContextBlock, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewElement &t);
