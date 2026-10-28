package main

import (
	_ "embed"
	"fmt"
	"log"
	"sync"
	"sync/atomic"
	"time"

	"github.com/getlantern/systray"

	"tg-autoproxy/internal/autostart"
	"tg-autoproxy/internal/checker"
	"tg-autoproxy/internal/config"
	"tg-autoproxy/internal/fetcher"
	"tg-autoproxy/internal/instance"
	"tg-autoproxy/internal/opener"
	"tg-autoproxy/internal/proxy"
)

//go:embed assets/icon.ico
var iconData []byte

const (
	appName      = "TgAutoProxy"
	checkTimeout = 4 * time.Second
	checkBatch   = 48
	watchPeriod  = time.Minute
)

type app struct {
	mu        sync.Mutex
	searching bool
	active    *proxy.Entry
	cfg       config.Config

	autoSwitch atomic.Bool
	stop       chan struct{}

	mStatus  *systray.MenuItem
	mConnect *systray.MenuItem
	mRefresh *systray.MenuItem
	mAuto    *systray.MenuItem
	mStartup *systray.MenuItem
}

func main() {
	acquired, err := instance.Acquire()
	if err != nil {
		log.Fatalf("single instance: %v", err)
	}
	if !acquired {
		return
	}
	defer instance.Release()

	systray.Run(onReady, func() {})
}

func onReady() {
	a := &app{stop: make(chan struct{})}
	a.cfg = config.Load()
	a.autoSwitch.Store(a.cfg.AutoSwitch)

	systray.SetIcon(iconData)
	systray.SetTitle(appName)
	systray.SetTooltip(appName + " — поиск рабочего прокси")

	a.mStatus = systray.AddMenuItem("Прокси: не выбран", "Текущий прокси")
	a.mStatus.Disable()
	systray.AddSeparator()
	a.mConnect = systray.AddMenuItem("Подключить в Telegram", "Открыть tg://proxy")
	a.mRefresh = systray.AddMenuItem("Найти новый прокси", "Обновить список и выбрать рабочий")
	systray.AddSeparator()
	a.mAuto = systray.AddMenuItemCheckbox("Автопоиск и автопереключение", "Следить за прокси и переключаться при сбое", a.cfg.AutoSwitch)
	a.mStartup = systray.AddMenuItemCheckbox("Автозапуск с Windows", "Запускать при входе в систему", autostart.IsEnabled())
	systray.AddSeparator()
	mQuit := systray.AddMenuItem("Выход", "Закрыть приложение")

	go a.search("")
	go a.watch()

	for {
		select {
		case <-a.mConnect.ClickedCh:
			a.connect()
		case <-a.mRefresh.ClickedCh:
			go a.search(a.activeKey())
		case <-a.mAuto.ClickedCh:
			a.toggleAuto()
		case <-a.mStartup.ClickedCh:
			a.toggleStartup()
		case <-mQuit.ClickedCh:
			close(a.stop)
			systray.Quit()
			return
		}
	}
}

func (a *app) connect() {
	current := a.current()
	if current == nil {
		go a.search("")
		return
	}
	if err := opener.Open(proxy.BuildLink(current)); err != nil {
		log.Printf("open link: %v", err)
	}
}

func (a *app) toggleAuto() {
	enabled := !a.autoSwitch.Load()
	a.autoSwitch.Store(enabled)
	a.cfg.AutoSwitch = enabled

	if enabled {
		a.mAuto.Check()
	} else {
		a.mAuto.Uncheck()
	}

	if err := config.Save(a.cfg); err != nil {
		log.Printf("save config: %v", err)
	}
}

func (a *app) toggleStartup() {
	if a.mStartup.Checked() {
		if err := autostart.Disable(); err != nil {
			log.Printf("disable autostart: %v", err)
			return
		}
		a.mStartup.Uncheck()
		return
	}

	if err := autostart.Enable(); err != nil {
		log.Printf("enable autostart: %v", err)
		return
	}
	a.mStartup.Check()
}

func (a *app) watch() {
	ticker := time.NewTicker(watchPeriod)
	defer ticker.Stop()

	for {
		select {
		case <-a.stop:
			return
		case <-ticker.C:
			if a.autoSwitch.Load() {
				a.checkActive()
			}
		}
	}
}

func (a *app) checkActive() {
	current := a.current()
	if current == nil {
		a.search("")
		return
	}

	if checker.IsAlive(current.Host, current.Port, checkTimeout) {
		return
	}

	log.Printf("proxy %s:%d is down, searching replacement", current.Host, current.Port)
	a.search(current.Key())
}

func (a *app) search(exclude string) {
	if !a.beginSearch() {
		return
	}
	defer a.endSearch()

	systray.SetTooltip(appName + " — ищу рабочий прокси")

	list, err := fetcher.Fetch()
	if err != nil {
		log.Printf("fetch proxies: %v", err)
		systray.SetTooltip(appName + " — не удалось получить список прокси")
		return
	}

	candidates := list
	if exclude != "" {
		candidates = excludeEntry(list, exclude)
	}

	entry, err := checker.FindAlive(candidates, checkTimeout, checkBatch)
	if err != nil && len(candidates) != len(list) {
		entry, err = checker.FindAlive(list, checkTimeout, checkBatch)
	}
	if err != nil {
		log.Printf("check proxies: %v", err)
		systray.SetTooltip(appName + " — рабочий прокси не найден")
		return
	}

	a.setActive(entry)

	if err := opener.Open(proxy.BuildLink(entry)); err != nil {
		log.Printf("open link: %v", err)
	}
}

func (a *app) setActive(entry *proxy.Entry) {
	a.mu.Lock()
	a.active = entry
	a.mu.Unlock()

	title := fmt.Sprintf("Прокси: %s:%d", entry.Host, entry.Port)
	a.mStatus.SetTitle(title)
	systray.SetTooltip(appName + " — " + title)
}

func (a *app) current() *proxy.Entry {
	a.mu.Lock()
	defer a.mu.Unlock()
	return a.active
}

func (a *app) activeKey() string {
	if current := a.current(); current != nil {
		return current.Key()
	}
	return ""
}

func (a *app) beginSearch() bool {
	a.mu.Lock()
	defer a.mu.Unlock()

	if a.searching {
		return false
	}
	a.searching = true
	return true
}

func (a *app) endSearch() {
	a.mu.Lock()
	a.searching = false
	a.mu.Unlock()
}

func excludeEntry(list []proxy.Entry, key string) []proxy.Entry {
	result := make([]proxy.Entry, 0, len(list))
	for _, entry := range list {
		if entry.Key() != key {
			result = append(result, entry)
		}
	}
	return result
}
