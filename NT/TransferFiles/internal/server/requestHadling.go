package server

import (
	ni "TransferFiles/internal/networkInteraction"
	"net"
	"fmt"
	"strconv"
	"io"
	"os"
)

const (
	storePackage = "uploads"
)

func HandleRequest(conn net.Conn) {
	defer conn.Close()
	file := PrepareReceivingFile(conn)
	if !file.DoReceive {
		fmt.Println("do not receive file on conn: ", conn.RemoteAddr().String())
		return
	}

	ReceiveFile(conn, file)
	fmt.Printf("End handle on %v\n", conn.RemoteAddr())
}

func ReceiveFile(conn net.Conn, metaFile MetaFile) {
	filePath := storePackage + "/" + metaFile.Name
	file, err := os.OpenFile(filePath, os.O_CREATE|os.O_RDWR, 0666)
	if err != nil {
		fmt.Println("Error open file")
		return
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
			fmt.Println("FATAL Error receiving file")
			return
		}
		// fmt.Printf("buffer: %v\n", string(buffer[:n]))
		receivedSize += n
		_, err = writer.Write(buffer[:n])
		if err != nil {
			fmt.Println("FATAL Error writing into file")
			return
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
	fmt.Println("getFileName(): ", metaFile.Name)
	fmt.Println("getFileSize(): ", metaFile.Size)
	fmt.Println("getOverwrite(): ", overwrite)
	fmt.Println("getOverwrite(): ", isOverwrite)
	filePath := storePackage + "/" + metaFile.Name
	fileExists, err := DoesFileExist(filePath)
	if err != nil {
		panic(err)
	}
	if fileExists && !isOverwrite {
		metaFile.DoReceive = false
		message = "ABCD"
	} else {
		// os.OpenFile(filePath, os.O_CREATE|os.O_RDWR, 0666)
		metaFile.DoReceive = true
		message = "9876"
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
	fmt.Printf("Does %s exists: %v\n", path, found)
	return
}

func setIsOverwrite(overwrite string) bool {
	return overwrite == "1"
}

func GetOverwrite(conn net.Conn) string {
	overwrite := ni.GetMessage(conn)
	fmt.Printf("get row overwrite: %v, size: %v\n", overwrite, len(overwrite))

	ni.SendMessage(ni.SuccessMsg, conn)
	return overwrite
}

func GetFileName(conn net.Conn) string {
	fileName := ni.GetMessage(conn)
	fmt.Printf("get row fileName: %v, size: %v\n", fileName, len(fileName))

	ni.SendMessage(ni.SuccessMsg, conn)
	return fileName
}

func GetFileSize(conn net.Conn) int64 {
	fileSize := ni.GetMessage(conn)
	fmt.Printf("get row fileName: %v, size: %v\n", fileSize, len(fileSize))

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
