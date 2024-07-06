#include "service.h"

#include <sstream>

#include "../settings.h"
#include "../../../utils/applicationinfo.h"
#include "../../../utils/logger.h"
#include "servicecontrolmanager.h"

using namespace std;

Service::Service(double weight) : IInstallBlock(weight, L"Service")
{
}

int Service::executeStep()
{
    installWindscribeService();
    return 100;
}

void Service::installWindscribeService()
{
    try {
        std::wstring serviceName = ApplicationInfo::serviceName();
        std::wstring serviceCmdLine;
        {
            std::wostringstream stream;
            stream << L"\"" << Settings::instance().getPath() << L"\\WindscribeService.exe\"";
            serviceCmdLine = stream.str();
        }

        wsl::ServiceControlManager scm;
        scm.openSCM(SC_MANAGER_ALL_ACCESS);
        scm.installService(serviceName.c_str(), serviceCmdLine.c_str(),
                           L"Windscribe Service", L"Manages the firewall and controls the VPN tunnel",
                           SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START, L"Nsi\0TcpIp\0", true);

        scm.setServiceSIDType(SERVICE_SID_TYPE_UNRESTRICTED);
        scm.openService(serviceName.c_str(), SERVICE_QUERY_STATUS | SERVICE_START);
        scm.startService();
    }
    catch (std::system_error& ex) {
        Log::instance().out(L"installWindscribeService - %hs", ex.what());
    }
}
