//go:build windows

package instance

import (
	"errors"

	"golang.org/x/sys/windows"
)

const mutexName = `Local\TgAutoProxy.SingleInstance`

var mutex windows.Handle

func Acquire() (bool, error) {
	name, err := windows.UTF16PtrFromString(mutexName)
	if err != nil {
		return false, err
	}

	handle, err := windows.CreateMutex(nil, false, name)
	if err != nil {
		if errors.Is(err, windows.ERROR_ALREADY_EXISTS) {
			if handle != 0 {
				windows.CloseHandle(handle)
			}
			return false, nil
		}
		return false, err
	}

	mutex = handle
	return true, nil
}

func Release() {
	if mutex == 0 {
		return
	}
	windows.CloseHandle(mutex)
	mutex = 0
}
