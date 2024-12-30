package cli
import (
	"github.com/spf13/pflag"
	"github.com/spf13/viper"

	"Snake/internal/log"
)

const (
	configOptionDescription     = "Config file path"
)

func Parse() (string) {
	pflag.StringP("config", "c", "../config/config.json", configOptionDescription)

	pflag.Parse()
	err := viper.BindPFlags(pflag.CommandLine)
	if err != nil {
		log.Logger.Fatalf("Command line parser error: %v", err)
	}

	return viper.GetString("config")
}