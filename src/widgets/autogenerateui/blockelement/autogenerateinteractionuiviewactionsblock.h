/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"
#include <QDebug>

class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewActionsBlock : public AutoGenerateInteractionUiViewBlockBase
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewActionsBlock(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewActionsBlock() override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewActionsBlock &other) const;

    [[nodiscard]] QList<AutoGenerateInteractionUiViewActionable *> elements() const;
    void setElements(const QList<AutoGenerateInteractionUiViewActionable *> &newElements);
    QWidget *generateWidget(RocketChatAccount *account, QWidget *parent = nullptr) const override;

protected:
    void serializeBlock(QJsonObject &o) const override;
    void parseBlock(const QJsonObject &json) override;

private:
    /*
        text?: TextObject;
        fields?: readonly TextObject[];
        accessory?: ButtonElement | DatePickerElement | ImageElement | MultiStaticSelectElement | OverflowElement | StaticSelectElement;

    */
    QList<AutoGenerateInteractionUiViewActionable *> mElements;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewActionsBlock)
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewActionsBlock &t);
