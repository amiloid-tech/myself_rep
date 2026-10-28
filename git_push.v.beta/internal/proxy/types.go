package proxy

import (
	"net/url"
	"strconv"
)

type Entry struct {
	Host    string `json:"host"`
	Port    int    `json:"port"`
	Secret  string `json:"secret"`
	Remarks string `json:"remarks,omitempty"`
}

func (e Entry) Key() string {
	return e.Host + ":" + strconv.Itoa(e.Port) + ":" + e.Secret
}

func BuildLink(e *Entry) string {
	v := url.Values{}
	v.Set("server", e.Host)
	v.Set("port", strconv.Itoa(e.Port))
	v.Set("secret", e.Secret)
	return "tg://proxy?" + v.Encode()
}
