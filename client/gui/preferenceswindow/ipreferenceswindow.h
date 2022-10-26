#ifndef IPREFERENCESWINDOW_H
#define IPREFERENCESWINDOW_H

#include <QGraphicsObject>
#include "preferenceswindow/preferencestab/ipreferencestabcontrol.h"
#include "connectionwindow/connectionwindowitem.h"
#include "types/robertfilter.h"

// abstract interface for preferences window
class IPreferencesWindow
{
public:
    virtual ~IPreferencesWindow() {}

    virtual QGraphicsObject *getGraphicsObject() = 0;

    virtual int minimumHeight() = 0;
    virtual void setHeight(int height) = 0;

    virtual void setCurrentTab(PREFERENCES_TAB_TYPE tab) = 0;
    virtual void setCurrentTab(PREFERENCES_TAB_TYPE tab, CONNECTION_SCREEN_TYPE subpage ) = 0;
    virtual void setScrollBarVisibility(bool on) = 0;

    virtual void setLoggedIn(bool loggedIn) = 0;
    virtual void setConfirmEmailResult(bool bSuccess) = 0;
    virtual void setSendLogResult(bool bSuccess) = 0;

    virtual void updateNetworkState(types::NetworkInterface network) = 0;

    virtual void addApplicationManually(QString filename) = 0;

    virtual void updateScaling() = 0;

    virtual void setPacketSizeDetectionState(bool on) = 0;
    virtual void showPacketSizeDetectionError(const QString &title, const QString &message) = 0;

    virtual void setRobertFilters(const QVector<types::RobertFilter> &filters) = 0;
    virtual void setRobertFiltersError() = 0;

    virtual void setScrollOffset(int offset) = 0;

    virtual void setSplitTunnelingActive(bool active) = 0;

signals:
    virtual void escape() = 0;
    virtual void signOutClick() = 0;
    virtual void loginClick() = 0;
    virtual void quitAppClick() = 0;
    virtual void sizeChanged() = 0;
    virtual void resizeFinished() = 0;

    virtual void viewLogClick() = 0;
    virtual void sendConfirmEmailClick() = 0;
    virtual void sendDebugLogClick() = 0;
    virtual void accountLoginClick() = 0;
    virtual void manageAccountClick() = 0;
    virtual void addEmailButtonClick() = 0;
    virtual void manageRobertRulesClick() = 0;

    virtual void currentNetworkUpdated(types::NetworkInterface) = 0;

    virtual void advancedParametersClicked() = 0;

#ifdef Q_OS_WIN
    virtual void setIpv6StateInOS(bool bEnabled, bool bRestartNow) = 0;
#endif
};

Q_DECLARE_INTERFACE(IPreferencesWindow, "IPreferencesWindow")

#endif // IPREFERENCESWINDOW_H
