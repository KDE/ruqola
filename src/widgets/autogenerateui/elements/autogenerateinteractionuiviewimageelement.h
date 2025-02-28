/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "autogenerateui/autogenerateinteractionuiviewimage.h"
#include "libruqolawidgets_private_export.h"
#include <QJsonObject>

class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewImageElement : public AutoGenerateInteractionUiViewActionable
{
public:
    AutoGenerateInteractionUiViewImageElement();
    ~AutoGenerateInteractionUiViewImageElement();

    [[nodiscard]] QWidget *generateWidget(QWidget *parent) override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewImageElement &other) const;
    void parse(const QJsonObject &json) override;

    [[nodiscard]] AutoGenerateInteractionUiViewImage image() const;
    void setImage(const AutoGenerateInteractionUiViewImage &newImage);

    [[nodiscard]] QJsonObject serialize() const override;

private:
    AutoGenerateInteractionUiViewImage mImage;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewImageElement)
Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewImageElement, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewImageElement &t);
