#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QSystemTrayIcon>
#include <QWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

class Notification: public QSystemTrayIcon{
    Q_OBJECT
    Q_PROPERTY (bool windowMinimized READ windowMinimized WRITE setWindowMinimized NOTIFY windowMinimizedChanged)

public:
    void createActions();
    void createTrayIcon();

    void setWindowMinimized(const bool &val);
    bool windowMinimized() const;
    void showNotification(const QString userID, const QString userName, QString message);

    Q_INVOKABLE static Notification * self();

signals:
    void windowMinimizedChanged();

public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    static Notification *n_self;
    Notification();

    QAction *quitAction;
    QMenu *trayIconMenu;

    bool n_windowMinimized;
};


#endif // NOTIFICATION_H
