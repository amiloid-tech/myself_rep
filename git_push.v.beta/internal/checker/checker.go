package checker

import (
	"errors"
	"net"
	"strconv"
	"sync"
	"time"

	"tg-autoproxy/internal/proxy"
)

var ErrNotAlive = errors.New("нет доступных прокси")

func IsAlive(host string, port int, timeout time.Duration) bool {
	conn, err := net.DialTimeout("tcp", net.JoinHostPort(host, strconv.Itoa(port)), timeout)
	if err != nil {
		return false
	}
	conn.Close()
	return true
}

func FindAlive(list []proxy.Entry, timeout time.Duration, batch int) (*proxy.Entry, error) {
	if batch < 1 {
		batch = 1
	}

	for start := 0; start < len(list); start += batch {
		end := min(start+batch, len(list))
		alive := make([]bool, end-start)

		var wg sync.WaitGroup
		wg.Add(end - start)
		for i := start; i < end; i++ {
			go func(i int) {
				defer wg.Done()
				alive[i-start] = IsAlive(list[i].Host, list[i].Port, timeout)
			}(i)
		}
		wg.Wait()

		for i, ok := range alive {
			if ok {
				entry := list[start+i]
				return &entry, nil
			}
		}
	}

	return nil, ErrNotAlive
}
