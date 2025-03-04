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
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewImageElement(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewImageElement() override;

    [[nodiscard]] QWidget *generateWidget(RocketChatAccount *account, QWidget *parent) override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewImageElement &other) const;

    [[nodiscard]] AutoGenerateInteractionUiViewImage image() const;
    void setImage(const AutoGenerateInteractionUiViewImage &newImage);

protected:
    void serializeElement(QJsonObject &o) const override;
    void parseElement(const QJsonObject &json) override;

private:
    AutoGenerateInteractionUiViewImage mImage;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewImageElement)
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewImageElement &t);
