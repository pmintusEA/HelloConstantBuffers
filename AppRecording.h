#pragma once

concurrency::task<bool> StartRecordingAsync();
concurrency::task<bool> RecordTimeSpanAsync(uint32_t TimeInSeconds);

