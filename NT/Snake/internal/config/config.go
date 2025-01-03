package config

import (
	"path/filepath"

	"github.com/spf13/viper"

	"Snake/internal/log"
)

type P2PMulticastConfig struct {
	Address string `mapstructure:"address"`
	Port    int    `mapstructure:"port"`
}

type P2PConfig struct {
	Delay     int                `mapstructure:"delay"`
	Multicast P2PMulticastConfig `mapstructure:"multicast"`
}

type APIConfig struct {
	PublicUrl string `mapstructure:"public_url"`
	Port      int    `mapstructure:"port"`
	Timeout   int    `mapstructure:"timeout"`
}

type HubMulticastConfig struct {
	Address string `mapstructure:"address"`
	Port    int    `mapstructure:"port"`
}

type HubConfig struct {
	Multicast HubMulticastConfig `mapstructure:"multicast"`
}

type AllConfig struct {
	P2P P2PConfig `mapstructure:"p2p"`
	API APIConfig `mapstructure:"api"`
	Hub HubConfig `mapstructure:"hub"`
}

var Config AllConfig

func LoadConfig(filePath string) {
	viper.SetConfigFile(filePath)
	viper.SetConfigType(filepath.Ext(filePath)[1:])

	err := viper.ReadInConfig()
	if err != nil {
		log.Logger.Fatalf("Config parser error: %v", err)
	}

	err = viper.Unmarshal(&Config)
	if err != nil {
		log.Logger.Fatalf("Config parser error: %v", err)
	}
}
