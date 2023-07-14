function Decoder(request) {
    var data = JSON.parse(request.body);
    var device = data.device;
    var file = data.file;
    var decoded = {};
    if (file === "locations.qos") {
    decoded.voltage = data.body.voltage;
    decoded.motion = data.body.motion;
    decoded.seconds = data.body.seconds;
    } else if (file === "_session.qo") {
    decoded.voltage = data.voltage;
    }
    else if (file === "data.qo") {
    decoded.temperature = data.body.sensortemperature;
    decoded.humidity = data.body.sensorhumidity;
    }
    if (("tower_lat" in data) && ("tower_lon" in data)) {
    decoded.tower_location = "(" + data.tower_lat + "," + data.tower_lon + ")";
    }
    if (("where_lat" in data) && ("where_lon" in data)) {
    decoded.device_location = "(" + data.where_lat + "," + data.where_lon + ")";
    }
    decoded.rssi = data.rssi;
    decoded.bars = data.bars;
    decoded.temp = data.temp;
    decoded.orientation = data.orientation;
    return [
         {
    device: device,
    field: "Temperature",
    value: decoded.temperature
    },
     {
    device: device,
    field: "Humidty",
    value: decoded.humidity
    },
    {
    device: device,
    field: "TOWER_LOCATION",
    value: decoded.tower_location
    },
    {
    device: device,
    field: "DEVICE_LOCATION",
    value: decoded.device_location
    },
    {
    device: device,
    field: "RSSI",
    value: decoded.rssi
    },
    {
    device: device,
    field: "BARS",
    value: decoded.bars
    },
    {
    device: device,
    field: "VOLTAGE",
    value: decoded.voltage
    },
    {
    device: device,
    field: "CARD_TEMPERATURE",
    value: decoded.temp
    },
    {
    device: device,
    field: "ORIENTATION",
    value: decoded.orientation
    }
    ];
    }