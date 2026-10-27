package fetcher

import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"strings"
	"time"

	"tg-autoproxy/internal/proxy"
)

func Fetch() ([]proxy.Entry, error) {
	client := &http.Client{Timeout: 15 * time.Second}
	client.Transport = &http.Transport{
		DisableKeepAlives: true,
	}
	var all []proxy.Entry

	urls := []string{
		"https://mtpro.xyz/api/?type=mtproto",
		"https://raw.githubusercontent.com/soroushmirzaei/telegram-configs-collector/main/protocols/mtproto",
		"https://raw.githubusercontent.com/roosterkid/openproxylist/main/MTProto.txt",
	}

	for _, u := range urls {
		resp, err := client.Get(u)
		if err != nil {
			continue
		}
		if resp.StatusCode != 200 {
			resp.Body.Close()
			continue
		}
		body, err := io.ReadAll(resp.Body)
		resp.Body.Close()
		if err != nil {
			continue
		}

		var list []proxy.Entry
		if json.Unmarshal(body, &list) == nil && len(list) > 0 {
			all = append(all, list...)
			continue
		}

		var geo struct {
			Data []struct {
				IP     string `json:"ip"`
				Port   int    `json:"port"`
				Secret string `json:"secret"`
			} `json:"data"`
		}
		if json.Unmarshal(body, &geo) == nil {
			for _, d := range geo.Data {
				if d.IP != "" && d.Port > 0 && len(d.Secret) >= 16 {
					all = append(all, proxy.Entry{Host: d.IP, Port: d.Port, Secret: d.Secret})
				}
			}
			continue
		}

		lines := strings.Split(string(body), "\n")
		for _, line := range lines {
			line = strings.TrimSpace(line)
			if line == "" || strings.HasPrefix(line, "#") {
				continue
			}
			parts := strings.Split(line, ":")
			if len(parts) == 3 {
				host := parts[0]
				var port int
				fmt.Sscanf(parts[1], "%d", &port)
				secret := parts[2]
				if host != "" && port > 0 && len(secret) >= 16 {
					all = append(all, proxy.Entry{Host: host, Port: port, Secret: secret})
				}
			}
		}
	}

	seen := map[string]bool{}
	filtered := []proxy.Entry{}
	for _, p := range all {
		if p.Host == "" || p.Port <= 0 || len(p.Secret) < 16 {
			continue
		}
		key := fmt.Sprintf("%s:%d:%s", p.Host, p.Port, p.Secret)
		if seen[key] {
			continue
		}
		seen[key] = true
		filtered = append(filtered, p)
	}

	if len(filtered) == 0 {
		return nil, fmt.Errorf("no proxies found")
	}
	return filtered, nil
}
