{
  "device": {
    "deviceId": device
  },
  "measurements": {
    "temp": body.temp,
    "humidity": body.humidity,
    "co2": body.co2
  },
  "timestamp": $fromMillis(when * 1000)
}
