Pushd "%~dp0"
start /w pnputil -i -a atheros_bth.inf
sc stop "AtherosSvc"