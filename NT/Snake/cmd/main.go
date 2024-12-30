package main

import ( 
	"net"
	"os"
	"os/signal"
	"strconv"
	"syscall"
	"time"

	"Snake/internal/api"
	"Snake/internal/cli"
	"Snake/internal/config"
	"Snake/internal/hub"
	"Snake/internal/log"
	"Snake/internal/p2p"
)

func main() {
	configPath := cli.Parse()

	log.Logger.Info("to quit application press Ctrl+C")

	config.LoadConfig(configPath)

	// Обработчик сигнала SigInt
	sigInt := make(chan os.Signal, 1)
	signal.Notify(sigInt, os.Interrupt, syscall.SIGINT)

	// Инициализация P2P узла
	p2pMulticastAddr, err := net.ResolveUDPAddr("udp",
		net.JoinHostPort(config.Config.P2P.Multicast.Address,
			strconv.Itoa(config.Config.P2P.Multicast.Port)))
	if err != nil {
		log.Logger.Fatalf("resolving P2P node multicast address error: %v", err)
		return
	}
	peer := p2p.NewPeer(p2pMulticastAddr)
	defer func() {
		err := peer.Close()
		if err != nil {
			log.Logger.Error(err)
		}
		log.Logger.Info("P2P node has completed")
	}()

	// Init and start API server
	apiServer := api.NewServer(
		config.Config.API.Port,
		time.Duration(config.Config.API.Timeout)*time.Millisecond,
		peer)
	if err := apiServer.Start(); err != nil {
		log.Logger.Fatal(err)
	}
	defer func() {
		err := apiServer.Close()
		if err != nil {
			log.Logger.Error(err)
		}
		log.Logger.Info("API server has completed")
	}()

	
	// Init and start of message distribution by free and public nodes
	addrStr := net.JoinHostPort(
		config.Config.Hub.Multicast.Address,
		strconv.Itoa(config.Config.Hub.Multicast.Port),
	)
	addr, err := net.ResolveUDPAddr("udp", addrStr)
	if err != nil {
		log.Logger.Fatalf("hub sender error: %v", err)
	}
	hubSender, err := hub.NewSender(
		config.Config.API.PublicUrl,
		addr,
		func() bool {
			return apiServer.IsFree()
		})
	if err != nil {
		log.Logger.Fatalf("hub sender error: %v", err)
	}

	if err := hubSender.Start(); err != nil {
		log.Logger.Fatalf("hub sender error: %v", err)
	}
	defer func() {
		err := hubSender.Close()
		if err != nil {
			log.Logger.Error(err)
		}
		log.Logger.Info("hub sender has completed")
	}()

	<-sigInt

	log.Logger.Info("waiting for the application to complete")
}