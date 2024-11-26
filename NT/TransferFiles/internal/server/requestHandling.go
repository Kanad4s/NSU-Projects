package server

import (
	ni "TransferFiles/internal/networkInteraction"
	"net"
	"fmt"
)

const (
	storePackage = "uploads"
)

func HandleRequest(conn net.Conn) {
	defer conn.Close()
	file := PrepareReceivingFile(conn)
	if !file.DoReceive {
		fmt.Println("do not receive file on conn: ", conn.RemoteAddr().String())
		panic("receiving unaccepted")
	}

	ReceiveFile(conn, file)
	fmt.Printf("End handle on %v\n", conn.RemoteAddr())
	controlMsg := ni.SuccessMsg
	defer func() {
		if mes := recover(); mes != nil {
			fmt.Printf("Error msg: %v\n", mes)
			controlMsg = ni.ErrMsg
		}
		ni.SendMessage(controlMsg, conn)
		}()
}
