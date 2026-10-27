package proxy

import (
	"fmt"
	"net/url"
)

type Entry struct {
	Host    string
	Port    int
	Secret  string
	Remarks string
}

func BuildLink(e *Entry) string {
	v := url.Values{}
	v.Set("server", e.Host)
	v.Set("port", fmt.Sprintf("%d", e.Port))
	v.Set("secret", e.Secret)
	return "tg://proxy?" + v.Encode()
}
