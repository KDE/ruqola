/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"

// See ./packages/ui-kit/src/blocks/Option.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewOption : public AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewOption();
    ~AutoGenerateInteractionUiViewOption() override;
    void parse(const QJsonObject &json) override;

    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;

    [[nodiscard]] QString value() const;
    void setValue(const QString &newValue);

    [[nodiscard]] QString url() const;
    void setUrl(const QString &newUrl);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewOption &other) const;

private:
    QString mValue;
    QString mUrl;
    // TODO add url/description
    // TODO add textobject
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewOption)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewOption, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewOption &t);
