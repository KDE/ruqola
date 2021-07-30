#ifndef ROOMLISTHEADINGSPROXYMODEL_H
#define ROOMLISTHEADINGSPROXYMODEL_H

#include "libruqolacore_export.h"
#include <QIdentityProxyModel>

class LIBRUQOLACORE_EXPORT RoomListHeadingsProxyModel : public QIdentityProxyModel
{
    Q_OBJECT
public:
    explicit RoomListHeadingsProxyModel(QObject *parent = nullptr);
};

#endif // ROOMLISTHEADINGSPROXYMODEL_H
