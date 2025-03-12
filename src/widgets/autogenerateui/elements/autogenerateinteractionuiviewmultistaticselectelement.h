/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>
class AutoGenerateInteractionUiViewText;
class AutoGenerateInteractionUiViewOption;
// see ./packages/ui-kit/src/blocks/elements/MultiStaticSelectElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewMultiStaticSelectElement : public AutoGenerateInteractionUiViewActionable
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewMultiStaticSelectElement(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewMultiStaticSelectElement() override;

    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;
    [[nodiscard]] AutoGenerateInteractionUiViewText *placeHolder() const;
    void setPlaceHolder(AutoGenerateInteractionUiViewText *newPlaceHolder);

    [[nodiscard]] QList<AutoGenerateInteractionUiViewOption *> options() const;
    void setOptions(const QList<AutoGenerateInteractionUiViewOption *> &newOptions);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewMultiStaticSelectElement &other) const;

protected:
    void serializeElement(QJsonObject &o) const override;
    void parseElement(const QJsonObject &json) override;

private:
    AutoGenerateInteractionUiViewText *mPlaceHolder = nullptr;
    QList<AutoGenerateInteractionUiViewOption *> mOptions;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewMultiStaticSelectElement)
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewMultiStaticSelectElement &t);
