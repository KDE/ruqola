/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "autogenerateui/autogenerateinteractionuiviewimage.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>
// ./packages/ui-kit/src/blocks/elements/LinearScaleElement.ts range ?
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewLinearScaleElement : public AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewLinearScaleElement();
    ~AutoGenerateInteractionUiViewLinearScaleElement() override;

    [[nodiscard]] QWidget *generateWidget(RocketChatAccount *account, QWidget *parent) override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewLinearScaleElement &other) const;

protected:
    void serializeElement(QJsonObject &o) const override;
    void parseElement(const QJsonObject &json) override;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewLinearScaleElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewLinearScaleElement, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewLinearScaleElement &t);
