/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewinputblock.h"
#include "autogenerateui/autogenerateinteractionuiviewactionable.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewcheckboxelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewdatepickerelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewlinearscaleelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewmultistaticselectelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewplaintextinputelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewradiobuttonelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewstaticselectelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewtimepickerelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewtoggleswitchelement.h"
#include "autogenerateui/widgets/actionelementwidget.h"
#include "ruqola_autogenerateui_debug.h"

#include <QJsonObject>
#include <QLabel>
#include <QLayout>
#include <QWidget>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewInputBlock::AutoGenerateInteractionUiViewInputBlock(QObject *parent)
    : AutoGenerateInteractionUiViewBlockBase(parent)
{
}

AutoGenerateInteractionUiViewInputBlock::~AutoGenerateInteractionUiViewInputBlock()
{
    delete mElement;
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewInputBlock &t)
{
    d.space() << "AutoGenerateInteractionUiViewBlockBase:" << static_cast<const AutoGenerateInteractionUiViewBlockBase &>(t);
    d.space() << "optional:" << t.optional();
    d.space() << "label:" << t.label();
    return d;
}

bool AutoGenerateInteractionUiViewInputBlock::operator==(const AutoGenerateInteractionUiViewInputBlock &other) const
{
    return AutoGenerateInteractionUiViewBlockBase::operator==(other) && other.optional() == optional() && other.label() == label();
}

void AutoGenerateInteractionUiViewInputBlock::parseBlock(const QJsonObject &json)
{
    /*
              element:
                | ChannelsSelectElement
                | ConversationsSelectElement
                | MultiChannelsSelectElement
                | MultiConversationsSelectElement
                | MultiStaticSelectElement
                | MultiUsersSelectElement
                | UsersSelectElement
    */
    mOptional = json["optional"_L1].toBool();
    mLabel.parse(json["label"_L1].toObject());
    const QJsonObject elementObj = json["element"_L1].toObject();
    const QString type = elementObj["type"_L1].toString();
    if (type == "plain_text_input"_L1) {
        mElement = new AutoGenerateInteractionUiViewPlainTextInputElement;
        mElement->parse(elementObj);
    } else if (type == "datepicker"_L1) {
        mElement = new AutoGenerateInteractionUiViewDatePickerElement;
        mElement->parse(elementObj);
    } else if (type == "time_picker"_L1) {
        mElement = new AutoGenerateInteractionUiViewTimePickerElement;
        mElement->parse(elementObj);
    } else if (type == "static_select"_L1) {
        mElement = new AutoGenerateInteractionUiViewStaticSelectElement;
        mElement->parse(elementObj);
    } else if (type == "linear_scale"_L1) {
        mElement = new AutoGenerateInteractionUiViewLinearScaleElement;
        mElement->parse(elementObj);
    } else if (type == "checkbox"_L1) {
        mElement = new AutoGenerateInteractionUiViewCheckboxElement;
        mElement->parse(elementObj);
    } else if (type == "radio_button"_L1) {
        mElement = new AutoGenerateInteractionUiViewRadioButtonElement;
        mElement->parse(elementObj);
    } else if (type == "toggle_switch"_L1) {
        mElement = new AutoGenerateInteractionUiViewToggleSwitchElement;
        mElement->parse(elementObj);
    } else if (type == "multi_static_select"_L1) {
        mElement = new AutoGenerateInteractionUiViewMultiStaticSelectElement;
        mElement->parse(elementObj);
    } else {
        qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "AutoGenerateInteractionUiViewInputBlock Unknown type " << type;
    }
}

QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> AutoGenerateInteractionUiViewInputBlock::serializeBlockState() const
{
    QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> lst;
    if (mElement) {
        lst.append({QString::fromLatin1(mElement->actionId()), mElement->currentValue()});
    }
    return lst;
}

bool AutoGenerateInteractionUiViewInputBlock::optional() const
{
    return mOptional;
}

void AutoGenerateInteractionUiViewInputBlock::setOptional(bool newOptional)
{
    mOptional = newOptional;
}

AutoGenerateInteractionUiViewText AutoGenerateInteractionUiViewInputBlock::label() const
{
    return mLabel;
}

void AutoGenerateInteractionUiViewInputBlock::setLabel(const AutoGenerateInteractionUiViewText &newLabel)
{
    mLabel = newLabel;
}

QWidget *AutoGenerateInteractionUiViewInputBlock::generateWidget(QWidget *parent)
{
    auto widget = new QWidget(parent);
    parent->layout()->addWidget(widget);
    auto vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins({});
    widget->setLayout(vboxLayout);
    if (!mLabel.isEmpty()) {
        auto label = new QLabel(parent);
        QFont f = label->font();
        f.setBold(true);
        label->setFont(f);
        label->setText(mLabel.generateText());
        vboxLayout->addWidget(label);
    }
    if (mElement) {
        connect(mElement, &AutoGenerateInteractionUiViewActionable::actionChanged, this, [this](const QByteArray &actionId, const QString &value) {
            // qDebug() << "AutoGenerateInteractionUiViewActionsBlock: actionId " << actionId << " value " << value << "blockId " << mBlockId;
            Q_EMIT actionChanged(mBlockId, actionId, value);
        });

        vboxLayout->addWidget(mElement->generateWidget(parent));
    }
    return widget;
}

AutoGenerateInteractionUiViewActionable *AutoGenerateInteractionUiViewInputBlock::element() const
{
    return mElement;
}

void AutoGenerateInteractionUiViewInputBlock::setElement(AutoGenerateInteractionUiViewActionable *newElement)
{
    mElement = newElement;
}

void AutoGenerateInteractionUiViewInputBlock::setErrorMessages(const QMap<QString, QString> &map)
{
    if (mElement) {
        mElement->setErrorMessages(map);
    }
}

void AutoGenerateInteractionUiViewInputBlock::assignState(const QList<StateInfo> &info)
{
    if (mElement) {
        for (const auto &stateInfo : info) {
            if (QString::fromLatin1(mElement->actionId()) == stateInfo.actionId) {
                mElement->setCurrentValue(stateInfo.value);
            }
        }
    }
}

void AutoGenerateInteractionUiViewInputBlock::serializeBlock(QJsonObject &o) const
{
    o["label"_L1] = mLabel.serialize();
    o["optional"_L1] = mOptional;
    if (mElement) {
        o["element"_L1] = mElement->serialize();
    }
}

#include "moc_autogenerateinteractionuiviewinputblock.cpp"
