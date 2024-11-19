package server

import (
	"time"
	"fmt"
)

type speedMeter struct {
	startTime              time.Time
	previousTime           time.Time
	previousActualFileSize int64
}

func NewSpeedCounter() *speedMeter {
	return &speedMeter{
		startTime:              time.Now(),
		previousTime:           time.Now(),
		previousActualFileSize: 0,
	}
}

func calcSpeed(sessionInfo metaFIle) (speedCounter *speedMeter)  {
	instantSpeed := float64(sessionInfo.actualFileSize-speedCounter.previousActualFileSize) * 1e9 /
		float64(time.Now().UnixNano()-speedCounter.previousTime.UnixNano())
	averageSpeed := float64(sessionInfo.actualFileSize) * 1e9 /
		float64(time.Now().UnixNano()-speedCounter.startTime.UnixNano())

	speedCounter.previousTime = time.Now()
	speedCounter.previousActualFileSize = sessionInfo.actualFileSize

	log.Log.Infof("Transfer %s: %s, %s", sessionInfo.filePath, SpeedToString(instantSpeed),
		SpeedToString(averageSpeed))
}

func SpeedToString(speed float64) string {
	if speed < 1024 {
		return fmt.Sprintf("%.2f B/s", speed)
	} else if speed < 1024*1024 {
		return fmt.Sprintf("%.2f KB/s", speed/1024)
	} else if speed < 1024*1024*1024 {
		return fmt.Sprintf("%.2f MB/s", speed/(1024*1024))
	} else {
		return fmt.Sprintf("%.2f GB/s", speed/(1024*1024*1024))
	}
}
