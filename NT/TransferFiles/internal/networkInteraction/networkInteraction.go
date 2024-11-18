package networkInteraction

import (
	"net"
	"fmt"
	"bufio"
	"strings"
)

const (
	SuccessMsg = "success"
	ErrMsg = "error"
	EndSymbol = '\a'
	bufferSize = 1048576 // 1024 ^ 2
	bufferSizeBig = 1073741824 // 1024 ^ 3
)

func GetMessage(conn net.Conn) (msg string) {
	msg, err := bufio.NewReader(conn).ReadString(EndSymbol)
	if err != nil {
		fmt.Println("Error reading fileName: ", err.Error())
	}
	endSymbolIndex := strings.LastIndex(msg, string(EndSymbol))
	if endSymbolIndex == -1 {
		endSymbolIndex = len(msg) - 1
	}
	msg = msg[:endSymbolIndex]
	return
}

func SendMessage(msg string, conn net.Conn) {
	_, err := conn.Write([]byte(msg + string(EndSymbol)))
	if err != nil {
		println("Error sendResponse: ", err.Error())
	}
}

func SetBufferSize(fileSize int) int {
	if (fileSize / bufferSize >= 50) {
		return bufferSizeBig
	}
	return bufferSize
}

