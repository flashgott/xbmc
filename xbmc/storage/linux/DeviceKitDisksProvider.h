#pragma once
/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include <string>
#include <vector>

#include "storage/IStorageProvider.h"
#ifdef HAS_DBUS
#include "DBusUtil.h"

class CDeviceKitDiskDevice
{
public:
  CDeviceKitDiskDevice(const char *DeviceKitUDI);
  virtual ~CDeviceKitDiskDevice() { }

  virtual void Update() = 0;

  bool Mount();
  bool UnMount();

  bool IsApproved();

  std::string toString();

  CMediaSource ToMediaShare();

  std::string m_UDI, m_DeviceKitUDI, m_MountPath, m_FileSystem, m_Label;
  bool m_isMounted, m_isMountedByUs, m_isRemovable, m_isPartition, m_isFileSystem, m_isSystemInternal, m_isOptical;
  float m_PartitionSizeGiB;
};

class CDeviceKitDiskDeviceNewAPI : public CDeviceKitDiskDevice
{
public:
  CDeviceKitDiskDeviceNewAPI(const char *DeviceKitUDI) : CDeviceKitDiskDevice(DeviceKitUDI) { Update(); }
  ~CDeviceKitDiskDeviceNewAPI() override { }

  void Update() override;
};

class CDeviceKitDiskDeviceOldAPI : public CDeviceKitDiskDevice
{
public:
  CDeviceKitDiskDeviceOldAPI(const char *DeviceKitUDI) : CDeviceKitDiskDevice(DeviceKitUDI) { Update(); }
  ~CDeviceKitDiskDeviceOldAPI() override { }

  void Update() override;
};

class CDeviceKitDisksProvider : public IStorageProvider
{
public:
  CDeviceKitDisksProvider();
  ~CDeviceKitDisksProvider() override;

  void Initialize() override;
  void Stop() override { }

  void GetLocalDrives(VECSOURCES &localDrives) override { GetDisks(localDrives, false); }
  void GetRemovableDrives(VECSOURCES &removableDrives) override { GetDisks(removableDrives, true); }

  bool Eject(const std::string& mountpath) override;

  std::vector<std::string> GetDiskUsage() override;

  bool PumpDriveChangeEvents(IStorageEventsCallback *callback) override;

  static bool HasDeviceKitDisks();
private:
  typedef std::map<std::string, CDeviceKitDiskDevice *> DeviceMap;
  typedef std::pair<std::string, CDeviceKitDiskDevice *> DevicePair;

  void DeviceAdded(const char *object, IStorageEventsCallback *callback);
  void DeviceRemoved(const char *object, IStorageEventsCallback *callback);
  void DeviceChanged(const char *object, IStorageEventsCallback *callback);

  std::vector<std::string> EnumerateDisks();

  void GetDisks(VECSOURCES& devices, bool EnumerateRemovable);

  int m_DaemonVersion;

  DeviceMap m_AvailableDevices;

  CDBusConnection m_connection;
};
#endif
