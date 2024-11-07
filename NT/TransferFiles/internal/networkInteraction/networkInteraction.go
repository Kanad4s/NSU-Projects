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


