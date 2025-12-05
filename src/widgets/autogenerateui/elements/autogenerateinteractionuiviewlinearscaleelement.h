/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"
class QJsonObject;
class QSpinBox;
// ./packages/ui-kit/src/blocks/elements/LinearScaleElement.ts range ?
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewLinearScaleElement : public AutoGenerateInteractionUiViewActionable
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewLinearScaleElement(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewLinearScaleElement() override;

    [[nodiscard]] ActionElementWidget *generateWidget(QWidget *parent) override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewLinearScaleElement &other) const;

    [[nodiscard]] int minValue() const;
    void setMinValue(int newMinValue);

    [[nodiscard]] int maxValue() const;
    void setMaxValue(int newMaxValue);

    [[nodiscard]] int initialValue() const;
    void setInitialValue(int newInitialValue);

    [[nodiscard]] QVariant currentValue() const override;
    void setCurrentValue(const QVariant &v) override;

    [[nodiscard]] QString preLabel() const;
    void setPreLabel(const QString &newPreLabel);

    [[nodiscard]] QString postLabel() const;
    void setPostLabel(const QString &newPostLabel);

protected:
    void serializeElement(QJsonObject &o) const override;
    void parseElement(const QJsonObject &json) override;

private:
    int mMinValue = -1;
    int mMaxValue = -1;
    int mInitialValue = -1;
    QString mPreLabel;
    QString mPostLabel;
    QSpinBox *mSpinbox = nullptr;
};

LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewLinearScaleElement &t);
