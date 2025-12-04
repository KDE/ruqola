/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "autogenerateinteractionuiviewblockbase.h"
#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "libruqolawidgets_private_export.h"

class LIBRUQOLAWIDGETS_TESTS_EXPORT AutoGenerateInteractionUiViewActionsBlock : public AutoGenerateInteractionUiViewBlockBase
{
    Q_OBJECT
public:
    explicit AutoGenerateInteractionUiViewActionsBlock(QObject *parent = nullptr);
    ~AutoGenerateInteractionUiViewActionsBlock() override;
    [[nodiscard]] bool operator==(const AutoGenerateInteractionUiViewActionsBlock &other) const;

    [[nodiscard]] QList<AutoGenerateInteractionUiViewActionable *> elements() const;
    void setElements(const QList<AutoGenerateInteractionUiViewActionable *> &newElements);
    QWidget *generateWidget(QWidget *parent = nullptr) override;
    void setErrorMessages(const QMap<QString, QString> &map) override;

    void assignState(const QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> &info) override;

protected:
    void serializeBlock(QJsonObject &o) const override;
    void parseBlock(const QJsonObject &json) override;
    [[nodiscard]] QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> serializeBlockState() const override;

private:
    /*
        text?: TextObject;
        fields?: readonly TextObject[];
        accessory?: ButtonElement | DatePickerElement | ImageElement | MultiStaticSelectElement | OverflowElement | StaticSelectElement;

    */
    QList<AutoGenerateInteractionUiViewActionable *> mElements;
};

Q_DECLARE_METATYPE(AutoGenerateInteractionUiViewActionsBlock)
class QDebug;
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewActionsBlock &t);
