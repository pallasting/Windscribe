#ifndef APIRESOLUTIONGROUP_H
#define APIRESOLUTIONGROUP_H

#include "commongraphics/baseitem.h"

#include <QVariantAnimation>
#include "preferenceswindow/checkboxitem.h"
#include "preferenceswindow/editboxitem.h"
#include "preferenceswindow/preferencegroup.h"
#include "types/dnsresolutionsettings.h"

namespace PreferencesWindow {

class ApiResolutionGroup: public PreferenceGroup
{
    Q_OBJECT
public:
    explicit ApiResolutionGroup(ScalableGraphicsObject *parent,
                                const QString &desc = "",
                                const QString &descUrl = "");

    void setApiResolution(const types::DnsResolutionSettings &ar);
    bool hasItemWithFocus() override;

signals:
    void apiResolutionChanged(const types::DnsResolutionSettings &ar);

private slots:
    void onAutomaticChanged(QVariant value);
    void onIPChanged(const QString &text);

private:
    void updateMode();
    ComboBoxItem *resolutionModeItem_;
    EditBoxItem *editBoxIP_;
    types::DnsResolutionSettings settings_;
};

} // namespace PreferencesWindow

#endif // APIRESOLUTIONGROUP_H