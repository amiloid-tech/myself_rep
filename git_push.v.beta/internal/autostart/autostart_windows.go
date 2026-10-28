//go:build windows

package autostart

import (
	"errors"
	"os"
	"path/filepath"

	"golang.org/x/sys/windows/registry"
)

const (
	runKey  = `Software\Microsoft\Windows\CurrentVersion\Run`
	appName = "TgAutoProxy"
)

func Enable() error {
	exe, err := os.Executable()
	if err != nil {
		return err
	}
	if resolved, err := filepath.EvalSymlinks(exe); err == nil {
		exe = resolved
	}

	key, err := registry.OpenKey(registry.CURRENT_USER, runKey, registry.SET_VALUE)
	if err != nil {
		return err
	}
	defer key.Close()

	return key.SetStringValue(appName, `"`+exe+`"`)
}

func Disable() error {
	key, err := registry.OpenKey(registry.CURRENT_USER, runKey, registry.SET_VALUE)
	if err != nil {
		return err
	}
	defer key.Close()

	if err := key.DeleteValue(appName); err != nil && !errors.Is(err, registry.ErrNotExist) {
		return err
	}
	return nil
}

func IsEnabled() bool {
	key, err := registry.OpenKey(registry.CURRENT_USER, runKey, registry.QUERY_VALUE)
	if err != nil {
		return false
	}
	defer key.Close()

	_, _, err = key.GetStringValue(appName)
	return err == nil
}
