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
//    Q_PROPERTY(QSystemTrayIcon nSekf READ nSekf WRITE setnSekf NOTIFY nSekfChanged)
public:
    void createActions();
    void createTrayIcon();

    void setWindowMinimized(const bool &val);
    bool windowMinimized() const;
    void showNotification(const QString userID, const QString userName, QString message);

    static Notification * self();
    Notification();

signals:
    void windowMinimizedChanged();

private:
    static Notification *n_self;

    QAction *restoreAction;
    QAction *quitAction;
    QMenu *trayIconMenu;

    bool n_windowMinimized;
};


#endif // NOTIFICATION_H
