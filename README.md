# RPiHomeHub
Raspberry Pi hub for logging temps and controlling hot water/heating

Uses a NRF24L01+ radio module to receive temperature data from an Arduino temperature sensor module.
Temperature data is saved to a SQLite database and the last 24 hours are displayed using Civetweb embedded web server.

It also includes web controls for hot water and heating, controlling an Arduino wirelessly with a relay attached to turn the hot water and heating controls on.
