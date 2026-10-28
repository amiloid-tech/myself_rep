package config

import (
	"encoding/json"
	"os"
	"path/filepath"
)

type Config struct {
	AutoSwitch bool `json:"auto_switch"`
}

func Default() Config {
	return Config{AutoSwitch: true}
}

func Load() Config {
	cfg := Default()

	data, err := os.ReadFile(path())
	if err != nil {
		return cfg
	}
	if err := json.Unmarshal(data, &cfg); err != nil {
		return Default()
	}
	return cfg
}

func Save(cfg Config) error {
	name := path()
	if err := os.MkdirAll(filepath.Dir(name), 0o755); err != nil {
		return err
	}

	data, err := json.MarshalIndent(cfg, "", "  ")
	if err != nil {
		return err
	}
	return os.WriteFile(name, data, 0o644)
}

func path() string {
	dir, err := os.UserConfigDir()
	if err != nil {
		dir = os.TempDir()
	}
	return filepath.Join(dir, "TgAutoProxy", "config.json")
}
