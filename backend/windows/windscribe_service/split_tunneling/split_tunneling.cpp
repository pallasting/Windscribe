#include "../all_headers.h"
#include "split_tunneling.h"
#include "ip_address_table.h"
#include "tap_adapter_detector.h"
#include "../close_tcp_connections.h"
#include "../logger.h"
#include "../utils.h"
#include "../../../../common/utils/crashhandler.h"

SplitTunneling::SplitTunneling(FirewallFilter &firewallFilter, FwpmWrapper &fwmpWrapper) : firewallFilter_(firewallFilter), calloutFilter_(fwmpWrapper), 
				 isSplitTunnelActive_(false), isExclude_(false), bKeepLocalSockets_(false)
{
	connectStatus_.isConnected = false;
	detectWindscribeExecutables();
}

SplitTunneling::~SplitTunneling()
{
	assert(isSplitTunnelActive_ == false);
}

/*void SplitTunneling::start()
{
	if (!bStarted_)
	{
		Logger::instance().out(L"SplitTunneling::start()");

		splitTunnelServiceManager_.start();

		bStarted_ = true;
	}
}

void SplitTunneling::stop()
{
	if (bStarted_)
	{
		calloutFilter_.disable();
		routesManager_.disable();
		firewallFilter_.setSplitTunnelingDisabled();
		bStarted_ = false;

		splitTunnelServiceManager_.stop();
		Logger::instance().out(L"SplitTunneling::stop()");

		// close TCP sockets if TAP-connected
		if (bTapConnected_)
		{
			Logger::instance().out(L"SplitTunneling::threadFunc() close all TCP sockets");
			CloseTcpConnections::closeAllTcpConnections(bKeepLocalSockets_);
		}
	}
}*/

void SplitTunneling::setSettings(bool isActive, bool isExclude, const std::vector<std::wstring> &apps, const std::vector<std::wstring> &ips, const std::vector<std::string> &hosts)
{
	isSplitTunnelActive_ = isActive;
	isExclude_ = isExclude;

	apps_ = apps;

	///ipHostnamesManager_.setSettings(isExclude, ips, hosts);

	///applyExtraRules(apps_);
	routesManager_.updateState(connectStatus_, isSplitTunnelActive_, isExclude_);
	updateState();


	/*if (bStarted_)
	{
		Logger::instance().out(L"SplitTunneling::setSettings() set settings message");

		AppsIds appsIds;
		appsIds.setFromList(apps);
		if (!isExclude)
		{
			appsIds.addFrom(windscribeExecutablesIds_);
		}

		calloutFilter_.setSettings(isExclude, appsIds);
		firewallFilter_.setSplitTunnelingAppsIds(appsIds, isExclude);

		std::vector<Ip4AddressAndMask> ipsList;
		for (auto it = ips.begin(); it != ips.end(); ++it)
		{
			ipsList.push_back(Ip4AddressAndMask(it->c_str()));
		}

		routesManager_.setSettings(isExclude, ipsList, hosts);

		// close TCP sockets if TAP-connected
		if (bTapConnected_)
		{
			Logger::instance().out(L"SplitTunneling::threadFunc() close all TCP sockets");
			CloseTcpConnections::closeAllTcpConnections(bKeepLocalSockets_);
		}
	}
	else
	{
		assert(false);
	}*/
}

void SplitTunneling::setConnectStatus(CMD_CONNECT_STATUS &connectStatus)
{
	connectStatus_ = connectStatus;
	routesManager_.updateState(connectStatus_, isSplitTunnelActive_, isExclude_);
	updateState();

	/*if (bStarted_)
	{
		bTapConnected_ = connectStatus.isConnected;

		if (connectStatus.isConnected)
		{
			DWORD ip;
			NET_LUID luid;
			if (!TapAdapterDetector::detect(ip, luid))
			{
				Logger::instance().out(L"SplitTunneling::setConnectStatus() failed detect connected TAP-adapter");
				return;
			}

			NET_IFINDEX ifIndex;
			if (ConvertInterfaceLuidToIndex(&luid, &ifIndex) == NO_ERROR)
			{
				DWORD defaultIp;
				MIB_IPFORWARDROW defaultRow;

				if (getIpAddressDefaultInterface(ifIndex, defaultIp, defaultRow))
				{
					firewallFilter_.setSplitTunnelingEnabled();
					calloutFilter_.enable(ip, defaultIp);
					routesManager_.enable(defaultRow);
				}
				else
				{
					Logger::instance().out(L"SplitTunneling::setConnectStatus() getIpAddressDefaultInterface() failed");
				}
			}
			else
			{
				Logger::instance().out(L"SplitTunneling::setConnectStatus() ConvertInterfaceLuidToIndex() failed");
			}
		}
		else
		{
			calloutFilter_.disable();
			routesManager_.disable();
			firewallFilter_.setSplitTunnelingDisabled();
		}
	}*/
}

/*bool SplitTunneling::detectDefaultInterfaceFromRouteTable(IF_INDEX excludeIfIndex, IF_INDEX &outIfIndex, MIB_IPFORWARDROW &outRow)
{
	std::vector<unsigned char> arr(sizeof(MIB_IPFORWARDTABLE));
	DWORD dwSize = 0;
	if (GetIpForwardTable((MIB_IPFORWARDTABLE *)&arr[0], &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER)
	{
		arr.resize(dwSize);
	}

	if (GetIpForwardTable((MIB_IPFORWARDTABLE *)&arr[0], &dwSize, 0) != NO_ERROR)
	{
		return false;
	}

	MIB_IPFORWARDTABLE *pIpForwardTable = (MIB_IPFORWARDTABLE *)&arr[0];
	std::vector<ROUTE_ITEM> items;

	for (DWORD i = 0; i < pIpForwardTable->dwNumEntries; i++)
	{
		if (pIpForwardTable->table[i].dwForwardDest == 0)
		{
			if (pIpForwardTable->table[i].dwForwardIfIndex != excludeIfIndex)
			{
				ROUTE_ITEM ri;
				ri.interfaceIndex = pIpForwardTable->table[i].dwForwardIfIndex;
				ri.metric = pIpForwardTable->table[i].dwForwardMetric1;
				ri.row = pIpForwardTable->table[i];
				items.push_back(ri);
			}
		}
	}

	if (items.size() > 0)
	{
		std::sort(items.begin(), items.end(),
			[](const ROUTE_ITEM &a, const ROUTE_ITEM &b) -> bool
		{
			return a.metric < b.metric;
		});

		outIfIndex = items[0].interfaceIndex;
		outRow = items[0].row;
		return true;
	}
	else
	{
		return false;
	}
}

bool SplitTunneling::getIpAddressDefaultInterface(IF_INDEX tapAdapterIfIndex, DWORD &outIp, MIB_IPFORWARDROW &outRow)
{
	IF_INDEX outIfIndex;
	if (detectDefaultInterfaceFromRouteTable(tapAdapterIfIndex, outIfIndex, outRow))
	{
		IpAddressTable addrTable;
		outIp = addrTable.getAdapterIpAddress(outIfIndex);
		return true;
	}

	return false;
}*/

void SplitTunneling::removeAllFilters(FwpmWrapper &fwmpWrapper)
{
	CalloutFilter::removeAllFilters(fwmpWrapper);
}

void SplitTunneling::detectWindscribeExecutables()
{
	std::wstring exePath = Utils::getExePath();
	std::wstring fileFilter = exePath + L"\\*.exe";

	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFileEx(fileFilter.c_str(), FindExInfoBasic, &ffd, FindExSearchNameMatch, NULL, 0);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	std::vector<std::wstring> windscribeExeFiles;
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			std::wstring f = exePath + L"\\" + ffd.cFileName;
			windscribeExeFiles.push_back(f);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);

	windscribeExecutablesIds_.setFromList(windscribeExeFiles);
}

void SplitTunneling::updateState()
{
	if (connectStatus_.isConnected && isSplitTunnelActive_)
	{
		splitTunnelServiceManager_.start();

		DWORD redirectIp;
		AppsIds appsIds;

		if (isExclude_)
		{
			Ip4AddressAndMask ipAddress(connectStatus_.defaultAdapter.adapterIp.c_str());
			redirectIp = ipAddress.ipNetworkOrder();
			appsIds.setFromList(apps_);

			//ipHostnamesManager_.enable(connectStatus_.defaultAdapter.gatewayIp);
		}
		else
		{


			if (connectStatus_.protocol == CMD_PROTOCOL_OPENVPN || connectStatus_.protocol == CMD_PROTOCOL_STUNNEL_OR_WSTUNNEL)
			{
				//ipHostnamesManager_.enable(connectStatus_.vpnAdapter.gatewayIp);
			}
			else if (connectStatus_.protocol == CMD_PROTOCOL_IKEV2)
			{
				//ipHostnamesManager_.enable(connectStatus_.vpnAdapter.adapterIp);
			}
			else if (connectStatus_.protocol == CMD_PROTOCOL_WIREGUARD)
			{
				//ipHostnamesManager_.enable(connectStatus_.vpnAdapter.adapterIp);
			}
		}

		calloutFilter_.enable(redirectIp, appsIds);
	}
	else
	{
		calloutFilter_.disable();
		//ipHostnamesManager_.disable();
		splitTunnelServiceManager_.stop();
	}
}