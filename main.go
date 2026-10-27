package main

import (
	"fmt"
	"log"
	"os/exec"
	"runtime"
	"time"

	"github.com/getlantern/systray"

	"tg-autoproxy/internal/autostart"
	"tg-autoproxy/internal/checker"
	"tg-autoproxy/internal/fetcher"
	"tg-autoproxy/internal/proxy"
)

var currentProxy *proxy.Entry

func main() {
	systray.Run(onReady, onExit)
}

func onReady() {
	systray.SetTitle("TgAutoProxy")
	systray.SetTooltip("TG AutoProxy - поиск рабочего прокси")

	mConnect := systray.AddMenuItem("Подключить в Telegram", "Открыть tg://proxy ссылку")
	mRefresh := systray.AddMenuItem("Найти новый рабочий прокси", "Обновить список и выбрать лучший")
	mAuto := systray.AddMenuItemCheckbox("Автозапуск Windows", "Добавить в автозагрузку", autostart.IsEnabled())
	systray.AddSeparator()
	mQuit := systray.AddMenuItem("Выход", "Закрыть приложение")

	go findAndSelectProxy()

	go func() {
		for {
			select {
			case <-mConnect.ClickedCh:
				if currentProxy != nil {
					link := proxy.BuildLink(currentProxy)
					openURL(link)
					systray.SetTooltip(fmt.Sprintf("Подключено: %s:%d", currentProxy.Host, currentProxy.Port))
				} else {
					systray.SetTooltip("Нет рабочего прокси. Нажми 'Найти новый'")
				}
			case <-mRefresh.ClickedCh:
				findAndSelectProxy()
			case <-mAuto.ClickedCh:
				if mAuto.Checked() {
					autostart.Disable()
					mAuto.Uncheck()
				} else {
					autostart.Enable()
					mAuto.Check()
				}
			case <-mQuit.ClickedCh:
				systray.Quit()
				return
			}
		}
	}()
}

func onExit() {
}

func findAndSelectProxy() {
	systray.SetTooltip("Ищу рабочие прокси...")
	list, err := fetcher.Fetch()
	if err != nil {
		log.Println(err)
		systray.SetTooltip("Ошибка при получении списка")
		return
	}

	for _, p := range list {
		if checker.IsAlive(p.Host, p.Port, 5*time.Second) {
			currentProxy = &p
			systray.SetTooltip(fmt.Sprintf("Найден рабочий: %s:%d", p.Host, p.Port))
			link := proxy.BuildLink(currentProxy)
			openURL(link)
			return
		}
	}

	systray.SetTooltip("Рабочих прокси не найдено. Попробуй через минуту")
}

func openURL(url string) {
	var cmd *exec.Cmd
	switch runtime.GOOS {
	case "windows":
		cmd = exec.Command("rundll32", "url.dll,FileProtocolHandler", url)
	default:
		return
	}
	_ = cmd.Start()
}
