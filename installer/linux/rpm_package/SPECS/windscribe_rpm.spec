# make sure rpmbuild does not attempt to strip our binaries, which sometimes causes corruption
%define debug_package %{nil}
%define __strip /bin/true

Name:		windscribe
Version:	2.6.0
Release:	0
Summary:	Windscribe Client

Group:		Applications/Internet
License:	GPLv2
URL:		https://www.windscribe.com
Vendor:		Windscribe Limited
BuildArch:	x86_64
Source0:        windscribe.tar

Requires:	bash
Requires:	iptables
Requires:	glibc >= 2.28

%description
Windscribe client.

%prep

%build

%install
mv -f %{_sourcedir}/* %{buildroot}

%post
systemctl enable windscribe-helper
systemctl start windscribe-helper
ln -sf /opt/windscribe/windscribe-cli /usr/bin/windscribe-cli
update-desktop-database
echo linux_rpm_x64 > ../etc/windscribe/platform

%postun
rm -rf /usr/bin/windscribe-cli

%files
%config /etc/systemd/system-preset/50-windscribe-helper.preset
%config /etc/systemd/system/windscribe-helper.service
%config /etc/windscribe/*
/opt/windscribe/*
/usr/polkit-1/actions/com.windscribe.authhelper.policy
/usr/share/applications/windscribe.desktop
/usr/share/icons/hicolor/*/apps/windscribe.png