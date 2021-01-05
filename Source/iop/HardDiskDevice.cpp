#include "HardDiskDevice.h"
#include <cassert>
#include "AppConfig.h"
#include "../PS2VM_Preferences.h"
#include "StringUtils.h"

using namespace Iop::Ioman;

CHardDiskDevice::CHardDiskDevice()
{
	m_basePath = CAppConfig::GetInstance().GetPreferencePath(PREF_PS2_HDD_DIRECTORY);
}

Framework::CStream* CHardDiskDevice::GetFile(uint32 accessType, const char* devicePath)
{
	if(accessType & OPEN_FLAG_CREAT)
	{
		//Create a new partition
		auto createParams = StringUtils::Split(devicePath, ',', true);
		assert(createParams.size() == 5);
		CreatePartition(createParams);
		return new CHardDiskPartition();
	}
	else
	{
		auto openParams = StringUtils::Split(devicePath, ',', true);
		assert(openParams.size() >= 2);
		auto partitionPath = m_basePath / openParams[0];
		if(fs::exists(partitionPath))
		{
			return new CHardDiskPartition();
		}
		else
		{
			return nullptr;
		}
	}
}

Directory CHardDiskDevice::GetDirectory(const char* devicePath)
{
	return Directory(m_basePath);
}

fs::path CHardDiskDevice::GetMountPath(const char* devicePath)
{
	auto mountParams = StringUtils::Split(devicePath, ',', true);
	assert(mountParams.size() >= 2);
	auto partitionPath = m_basePath / mountParams[0];
	if(!fs::exists(partitionPath))
	{
		throw std::runtime_error("Partition doesn't exist.");
	}
	return m_basePath / mountParams[0];
}

void CHardDiskDevice::CreatePartition(const std::vector<std::string>& createParams)
{
	auto partitionName = createParams[0];
	if(partitionName.empty())
	{
		throw std::runtime_error("Invalid partition name.");
	}
	auto partitionPath = m_basePath / partitionName;
	fs::create_directory(partitionPath);
}

//-----------------------------------------------
//CHardDiskPartition

void CHardDiskPartition::Seek(int64, Framework::STREAM_SEEK_DIRECTION)
{
	throw new std::runtime_error("Not supported.");
}

uint64 CHardDiskPartition::Read(void*, uint64)
{
	throw new std::runtime_error("Not supported.");
}

uint64 CHardDiskPartition::Write(const void*, uint64)
{
	throw new std::runtime_error("Not supported.");
}

uint64 CHardDiskPartition::Tell()
{
	return 0;
}

bool CHardDiskPartition::IsEOF()
{
	return false;
}