#include "stdafx.h"

#include <chrono>

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Media;

bool RecordTimeSpan(uint32_t TimeInSeconds)
{
	TCHAR TempPath[MAX_PATH] = TEXT("");
	GetTempPath(ARRAYSIZE(TempPath), TempPath);

	TCHAR TempFileName[MAX_PATH] = TEXT("");
	GetTempFileName(TempPath, TEXT("vid"), 0, TempFileName);

	try
	{
		auto AsyncOutputFile = StorageFile::GetFileFromPathAsync(TempFileName);
		StorageFile OutputFile = AsyncOutputFile.get();

		AppRecording::AppRecordingManager AppRecordingManager = AppRecording::AppRecordingManager::GetDefault();
		AppRecording::AppRecordingStatus AppRecordingStatus = AppRecordingManager.GetStatus();

		// make sure that time span / "background" recording is enabled at the OS level and for the Game
		if (!AppRecordingStatus.CanRecordTimeSpan())
		{
			AppRecording::AppRecordingStatusDetails AppRecordingStatusDetails = AppRecordingStatus.Details();

			bool IsAnyAppBroadcasting = AppRecordingStatusDetails.IsAnyAppBroadcasting();
			bool IsAppInactive = AppRecordingStatusDetails.IsAppInactive();
			bool IsBlockedForApp = AppRecordingStatusDetails.IsBlockedForApp();
			bool IsCaptureResourceUnavailable = AppRecordingStatusDetails.IsCaptureResourceUnavailable();
			bool IsDisabledBySystem = AppRecordingStatusDetails.IsDisabledBySystem();
			bool IsDisabledByUser = AppRecordingStatusDetails.IsDisabledByUser();
			bool IsGameStreamInProgress = AppRecordingStatusDetails.IsGameStreamInProgress();
			bool IsGpuConstrained = AppRecordingStatusDetails.IsGpuConstrained();
			bool IsTimeSpanRecordingDisabled = AppRecordingStatusDetails.IsTimeSpanRecordingDisabled();

			if (AppRecordingStatusDetails.IsTimeSpanRecordingDisabled())
			{
				MessageBox(nullptr, TEXT("IsTimeSpanRecordingDisabled is True"), TEXT("RecordTimeSpan"), MB_ICONERROR);
				return false;
			}

			MessageBox(nullptr, TEXT("CanRecordTimeSpan is False"), TEXT("RecordTimeSpan"), MB_ICONERROR);
			return false;
		}

		AppRecording::AppRecordingStatusDetails AppRecordingStatusDetails = AppRecordingStatus.Details();

		// cap duration at what is available
		TimeSpan TimeAvailable = AppRecordingStatus.HistoricalBufferDuration();
		std::chrono::seconds TimeRequested(TimeInSeconds);
		TimeSpan TimeDuration = std::min<std::common_type_t<std::chrono::seconds, TimeSpan>>(TimeRequested, TimeAvailable);

		auto AsyncAppRecordingResult = AppRecordingManager.RecordTimeSpanToFileAsync(winrt::clock::now() - TimeDuration, TimeDuration, OutputFile);
		AppRecording::AppRecordingResult AppRecordingResult = AsyncAppRecordingResult.get();
	}
	catch (winrt::hresult_error const& Exception)
	{
		MessageBox(nullptr, Exception.message().c_str(), TEXT("RecordTimeSpan"), MB_ICONERROR);
		return false;
	}

	MessageBox(nullptr, TEXT("Success!"), TEXT("RecordTimeSpan"), MB_ICONINFORMATION);

	return true;
}

concurrency::task<bool> RecordTimeSpanAsync(uint32_t TimeInSeconds)
{
	return concurrency::create_task([=] {return RecordTimeSpan(TimeInSeconds); });
}

bool StartRecording()
{
	TCHAR TempPath[MAX_PATH] = TEXT("");
	GetTempPath(ARRAYSIZE(TempPath), TempPath);

	TCHAR TempFileName[MAX_PATH] = TEXT("");
	GetTempFileName(TempPath, TEXT("vid"), 0, TempFileName);

	try
	{
		auto AsyncOutputFile = StorageFile::GetFileFromPathAsync(TempFileName);
		StorageFile OutputFile = AsyncOutputFile.get();

		AppRecording::AppRecordingManager AppRecordingManager = AppRecording::AppRecordingManager::GetDefault();
		AppRecording::AppRecordingStatus AppRecordingStatus = AppRecordingManager.GetStatus();

		// make sure that recording is enabled at the OS level and for the Game
		if (!AppRecordingStatus.CanRecord())
		{
			AppRecording::AppRecordingStatusDetails AppRecordingStatusDetails = AppRecordingStatus.Details();

			bool IsAnyAppBroadcasting = AppRecordingStatusDetails.IsAnyAppBroadcasting();
			bool IsAppInactive = AppRecordingStatusDetails.IsAppInactive();
			bool IsBlockedForApp = AppRecordingStatusDetails.IsBlockedForApp();
			bool IsCaptureResourceUnavailable = AppRecordingStatusDetails.IsCaptureResourceUnavailable();
			bool IsDisabledBySystem = AppRecordingStatusDetails.IsDisabledBySystem();
			bool IsDisabledByUser = AppRecordingStatusDetails.IsDisabledByUser();
			bool IsGameStreamInProgress = AppRecordingStatusDetails.IsGameStreamInProgress();
			bool IsGpuConstrained = AppRecordingStatusDetails.IsGpuConstrained();
			bool IsTimeSpanRecordingDisabled = AppRecordingStatusDetails.IsTimeSpanRecordingDisabled();

			MessageBox(nullptr, TEXT("CanRecord is False"), TEXT("StartRecording"), MB_ICONERROR);
			return false;
		}

		auto AsyncAppRecordingResult = AppRecordingManager.StartRecordingToFileAsync(OutputFile);

		Sleep(1000);

		AsyncAppRecordingResult.Cancel();
	}
	catch (winrt::hresult_error const& Exception)
	{
		MessageBox(nullptr, Exception.message().c_str(), TEXT("StartRecording"), MB_ICONERROR);
		return false;
	}

	MessageBox(nullptr, TEXT("Success!"), TEXT("StartRecording"), MB_ICONINFORMATION);

	return true;
}

concurrency::task<bool> StartRecordingAsync()
{
	return concurrency::create_task([=] {return StartRecording(); });
}



