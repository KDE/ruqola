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
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewContextBlock(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewContextBlock() override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewContextBlock &other) const;
    [[nodiscard]] QWidget *generateWidget(RocketChatAccount *account, QWidget *parent) const override;

    [[nodiscard]] QList<AutoGenerateInteractionUiViewElement *> elements() const;
    void setElements(const QList<AutoGenerateInteractionUiViewElement *> &newElements);

protected:
    void serializeBlock(QJsonObject &o) const override;
    void parseBlock(const QJsonObject &json) override;

private:
    QList<AutoGenerateInteractionUiViewElement *> mElements;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewContextBlock)
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewElement &t);
