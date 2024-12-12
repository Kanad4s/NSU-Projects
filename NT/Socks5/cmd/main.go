package main

import (
	"runtime"

	"Socks5/internal/cli"
	"Socks5/internal/proxy"
)

func main() {
	runtime.GOMAXPROCS(1)
	port := cli.Parse()
	proxy.Start(port)
}
