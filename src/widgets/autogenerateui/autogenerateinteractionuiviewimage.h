/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewelement.h"
#include "libruqolawidgets_private_export.h"
#include <QMetaType>
class QJsonObject;
// ./packages/ui-kit/src/blocks/elements/ImageElement.ts
class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewImage : public AutoGenerateInteractionUiViewElement
{
public:
    AutoGenerateInteractionUiViewImage();
    ~AutoGenerateInteractionUiViewImage() override;
    void parse(const QJsonObject &json) override;

    [[nodiscard]] QString altText() const;
    void setAltText(const QString &newAltText);

    [[nodiscard]] QString imageUrl() const;
    void setImageUrl(const QString &newImageUrl);

    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewImage &other) const;

    [[nodiscard]] QJsonObject serialize() const override;

private:
    QString mAltText;
    QString mImageUrl;
};

Q_DECLARE_TYPEINFO(AutoGenerateInteractionUiViewImage, Q_RELOCATABLE_TYPE);
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewImage &t);
