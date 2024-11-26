package server

import (
	ni "TransferFiles/internal/networkInteraction"
	"fmt"
	"io"
	"net"
	"os"
	"strconv"
)

type MetaFile struct {
	Name       string
	Size       int64
	DoReceive  bool
	IsReceived bool
}

func ReceiveFile(conn net.Conn, metaFile MetaFile) {
	filePath := storePackage + "/" + metaFile.Name
	file, err := os.OpenFile(filePath, os.O_CREATE|os.O_RDWR, 0666)
	if err != nil {
		// fmt.Println("Error open file")
		panic("Error open file")
	}

	speed := NewSpeedMeter()
	go speed.StartSpeedCalc(&metaFile)
	buffer := ni.GetBufferBySize(metaFile.Size)
	writer := io.Writer(file)
	receivedSize := 0
	for int64(receivedSize) < metaFile.Size {
		// conn.SetReadDeadline(time.Now().Add(clientTimeout))
		n, err := conn.Read(buffer)
		if err != nil {
			panic("FATAL Error receiving file")
		}
		// fmt.Printf("buffer: %v\n", string(buffer[:n]))
		receivedSize += n
		_, err = writer.Write(buffer[:n])
		if err != nil {
			panic("FATAL Error writing into file")
		}
	}
	speed.StopSpeedCalc()
}

func PrepareReceivingFile(conn net.Conn) (metaFile MetaFile) {
	var message string
	metaFile.Name = GetFileName(conn)
	overwrite := GetOverwrite(conn)
	metaFile.Size = GetFileSize(conn)
	isOverwrite := setIsOverwrite(overwrite)
	filePath := storePackage + "/" + metaFile.Name
	fileExists, err := DoesFileExist(filePath)
	if err != nil {
		panic(err)
	}
	if fileExists && !isOverwrite {
		metaFile.DoReceive = false
		message = ni.TransferFileDeclineMsg
	} else {
		metaFile.DoReceive = true
		message = ni.TransferFileAcceptedMsg
	}

	ni.SendMessage(message, conn)
	return
}

func DoesFileExist(path string) (found bool, err error) {
	if _, err := os.Stat(path); err == nil {
		if os.IsNotExist(err) {
			err = nil
		} else {
			found = true
		}
	}
	fmt.Printf("Does receiving file: %s exists: %v\n", path, found)
	return
}

func setIsOverwrite(overwrite string) bool {
	return overwrite == "1"
}

func GetOverwrite(conn net.Conn) string {
	overwrite := ni.GetMessage(conn)
	// fmt.Printf("get row overwrite: %v, size: %v\n", overwrite, len(overwrite))

	ni.SendMessage(ni.SuccessMsg, conn)
	return overwrite
}

func GetFileName(conn net.Conn) string {
	fileName := ni.GetMessage(conn)
	// fmt.Printf("get row fileName: %v, size: %v\n", fileName, len(fileName))

	ni.SendMessage(ni.SuccessMsg, conn)
	return fileName
}

func GetFileSize(conn net.Conn) int64 {
	fileSize := ni.GetMessage(conn)
	// fmt.Printf("get row fileName: %v, size: %v\n", fileSize, len(fileSize))

	ni.SendMessage(ni.SuccessMsg, conn)
	size, err := strconv.Atoi(fileSize)
	if err != nil {
		fmt.Println("Error convert fileSize to int: ", err.Error())
	}
	return int64(size)
}

func CreateStore() {
	err := os.Mkdir(storePackage, 0755)
	if err != nil {
		fmt.Println("Error Mkdir: ", err.Error())
	}
}
