/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>
// ./packages/ui-kit/src/blocks/elements/LinearScaleElement.ts range ?
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewLinearScaleElement : public AutoGenerateInteractionUiViewActionable
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewLinearScaleElement(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewLinearScaleElement() override;

    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewLinearScaleElement &other) const;

    [[nodiscard]] int minValue() const;
    void setMinValue(int newMinValue);

    [[nodiscard]] int maxValue() const;
    void setMaxValue(int newMaxValue);

    [[nodiscard]] int initialValue() const;
    void setInitialValue(int newInitialValue);

protected:
    void serializeElement(QJsonObject &o) const override;
    void parseElement(const QJsonObject &json) override;

private:
    int mMinValue = -1;
    int mMaxValue = -1;
    int mInitialValue = -1;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewLinearScaleElement)
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewLinearScaleElement &t);
