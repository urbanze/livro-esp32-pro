# WiFi AP
Demonstração e utilização do WiFi AP com ESP32.

## Saída de informações
```
I (750) wifi_connect: Aguardando conexao WiFi
I (750) net_handler: WiFi STA: Start
I (2800) wifi:new:<11,0>, old:<1,0>, ap:<255,255>, sta:<11,0>, prof:1
I (2800) wifi:state: init -> auth (b0)
I (2840) wifi:state: auth -> assoc (0)
I (2870) wifi:state: assoc -> run (10)
I (2910) wifi:connected with Teste, aid = 10, channel 11, BW20, bssid = 24:aa:12:3c:11:05
I (2910) wifi:security: WPA2-PSK, phy: bgn, rssi: -70
I (2920) wifi:pm start, type: 1

I (2920) net_handler: WiFi STA: Connected
I (2940) wifi:AP's beacon interval = 102400 us, DTIM period = 1
I (3540) esp_netif_handlers: sta ip: 192.168.1.6, mask: 255.255.255.0, gw: 192.168.1.1
I (3540) net_handler: Got IP
I (3550) wifi_connect: WiFi conectado
```