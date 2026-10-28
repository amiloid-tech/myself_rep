package fetcher

import (
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"net/http"
	"net/url"
	"strconv"
	"strings"
	"sync"
	"time"

	"tg-autoproxy/internal/proxy"
)

var ErrEmpty = errors.New("список прокси пуст")

var sources = []string{
	"https://mtpro.xyz/api/?type=mtproto",
	"https://raw.githubusercontent.com/SoliSpirit/mtproto/master/all_proxies.txt",
	"https://raw.githubusercontent.com/Therealwh/MTPproxyLIST/main/verified/proxy_all_verified.txt",
	"https://raw.githubusercontent.com/leshchenko1979/tgproxy/main/docs/proxies.txt",
}

func Fetch() ([]proxy.Entry, error) {
	client := &http.Client{
		Timeout:   10 * time.Second,
		Transport: &http.Transport{DisableKeepAlives: true},
	}

	results := make([][]proxy.Entry, len(sources))
	var wg sync.WaitGroup
	wg.Add(len(sources))

	for i, source := range sources {
		go func(i int, source string) {
			defer wg.Done()
			entries, err := fetchSource(client, source)
			if err != nil {
				return
			}
			results[i] = entries
		}(i, source)
	}
	wg.Wait()

	var all []proxy.Entry
	for _, entries := range results {
		all = append(all, entries...)
	}

	result := unique(all)
	if len(result) == 0 {
		return nil, ErrEmpty
	}
	return result, nil
}

func fetchSource(client *http.Client, source string) ([]proxy.Entry, error) {
	resp, err := client.Get(source)
	if err != nil {
		return nil, err
	}
	defer resp.Body.Close()

	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("unexpected status %d", resp.StatusCode)
	}

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, err
	}

	if entries := parseJSON(body); len(entries) > 0 {
		return entries, nil
	}
	if entries := parseGeoJSON(body); len(entries) > 0 {
		return entries, nil
	}
	return parseLines(string(body)), nil
}

func parseJSON(data []byte) []proxy.Entry {
	var list []proxy.Entry
	if err := json.Unmarshal(data, &list); err != nil {
		return nil
	}

	result := make([]proxy.Entry, 0, len(list))
	for _, entry := range list {
		if valid(entry) {
			result = append(result, entry)
		}
	}
	return result
}

func parseGeoJSON(data []byte) []proxy.Entry {
	var payload struct {
		Data []struct {
			IP     string `json:"ip"`
			Port   int    `json:"port"`
			Secret string `json:"secret"`
		} `json:"data"`
	}
	if err := json.Unmarshal(data, &payload); err != nil {
		return nil
	}

	result := make([]proxy.Entry, 0, len(payload.Data))
	for _, item := range payload.Data {
		entry := proxy.Entry{Host: item.IP, Port: item.Port, Secret: item.Secret}
		if valid(entry) {
			result = append(result, entry)
		}
	}
	return result
}

func parseLines(text string) []proxy.Entry {
	var result []proxy.Entry
	for _, line := range strings.Split(text, "\n") {
		line = strings.TrimSpace(line)
		if line == "" || strings.HasPrefix(line, "#") {
			continue
		}

		if entry, ok := parseLink(line); ok {
			result = append(result, entry)
			continue
		}

		parts := strings.Split(line, ":")
		if len(parts) != 3 {
			continue
		}

		port, err := strconv.Atoi(parts[1])
		if err != nil {
			continue
		}

		entry := proxy.Entry{Host: strings.TrimSpace(parts[0]), Port: port, Secret: strings.TrimSpace(parts[2])}
		if valid(entry) {
			result = append(result, entry)
		}
	}
	return result
}

func parseLink(line string) (proxy.Entry, bool) {
	query := line
	if index := strings.IndexByte(query, '?'); index >= 0 {
		prefix := query[:index]
		if !strings.HasSuffix(prefix, "proxy") && !strings.HasSuffix(prefix, "proxy/") {
			return proxy.Entry{}, false
		}
		query = query[index+1:]
	} else {
		return proxy.Entry{}, false
	}

	if index := strings.IndexByte(query, '|'); index >= 0 {
		query = query[:index]
	}

	values, err := url.ParseQuery(query)
	if err != nil {
		return proxy.Entry{}, false
	}

	port, err := strconv.Atoi(values.Get("port"))
	if err != nil {
		return proxy.Entry{}, false
	}

	entry := proxy.Entry{Host: values.Get("server"), Port: port, Secret: values.Get("secret")}
	return entry, valid(entry)
}

func valid(entry proxy.Entry) bool {
	return entry.Host != "" && entry.Port > 0 && len(entry.Secret) >= 16
}

func unique(list []proxy.Entry) []proxy.Entry {
	seen := make(map[string]struct{}, len(list))
	result := make([]proxy.Entry, 0, len(list))

	for _, entry := range list {
		key := entry.Key()
		if _, ok := seen[key]; ok {
			continue
		}
		seen[key] = struct{}{}
		result = append(result, entry)
	}
	return result
}
