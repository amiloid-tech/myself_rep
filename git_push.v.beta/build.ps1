$ErrorActionPreference = "Stop"
Set-Location $PSScriptRoot

go vet ./...
go build -ldflags="-H=windowsgui -s -w" -o TgAutoProxy.exe .
