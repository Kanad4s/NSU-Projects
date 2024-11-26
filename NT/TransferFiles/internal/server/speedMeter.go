package server

import (
	"fmt"
	"time"
)

type SpeedMeter struct {
	startTime              time.Time
	previousTime           time.Time
	previousActualFileSize int64
	FinishChannel		   *chan bool
}

func NewSpeedMeter() *SpeedMeter {
	c := make(chan bool)
	return &SpeedMeter{
		startTime:              time.Now(),
		previousTime:           time.Now(),
		previousActualFileSize: 0,
		FinishChannel: &c,
	}
}

func (speedCounter *SpeedMeter) CalcCurSpeed(file *MetaFile)  {
	instantSpeed := float64(file.Size - speedCounter.previousActualFileSize) * 1e9 /
		float64(time.Now().UnixNano() - speedCounter.previousTime.UnixNano())
	averageSpeed := float64(file.Size) * 1e9 / float64(time.Now().UnixNano() - speedCounter.startTime.UnixNano())
	speedCounter.previousTime = time.Now()
	speedCounter.previousActualFileSize = file.Size

	fmt.Printf("Transfer %s: %s, %s\n", file.Name, SpeedToString(instantSpeed), SpeedToString(averageSpeed))
}

func SpeedToString(speed float64) string {
	if speed < 1024 {
		return fmt.Sprintf("%.2f B/s", speed)
	} else if speed < 1024*1024 {
		return fmt.Sprintf("%.2f KB/s", speed / 1024)
	} else if speed < 1024*1024*1024 {
		return fmt.Sprintf("%.2f MB/s", speed / (1024*1024))
	} else {
		return fmt.Sprintf("%.2f GB/s", speed / (1024*1024*1024))
	}
}

func (speedMeter *SpeedMeter) StartSpeedCalc(file *MetaFile) {
	ticker := time.NewTicker(time.Second)
	for {
		select {
		case <-*speedMeter.FinishChannel:
			ticker.Stop()
			speedMeter.CalcCurSpeed(file)
			return
		case <-ticker.C:
			speedMeter.CalcCurSpeed(file)
		}
	}
}

func (speedMeter *SpeedMeter) StopSpeedCalc() {
	*speedMeter.FinishChannel <- true
}
