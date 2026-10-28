//go:build windows

package opener

import "os/exec"

func Open(link string) error {
	return exec.Command("rundll32", "url.dll,FileProtocolHandler", link).Start()
}
