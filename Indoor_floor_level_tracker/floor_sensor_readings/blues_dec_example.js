function Decoder(request) {
    
    var data = JSON.parse(request.body);
    var device = data.device;
    
    var file = data.file;
    
    var decoded = {};
    
    if (file === "_track.qo") {
        
        decoded.voltage = data.body.voltage;
        decoded.motion = data.body.motion;
        decoded.seconds = data.body.seconds;
        
    } else if (file === "_session.qo") {
        
        decoded.voltage = data.voltage;

    } else if (file === "data.qo") {
        
        decoded.altitude = data.body.altitude;
        decoded.pressure = data.body.pressure;
        decoded.temperature = data.body.temperature;
    } else if (file === "sensors.qo") {
        
        decoded.temperature = data.body.temp;
        decoded.humidity = data.body.humidity;
    }
    
    if (("tower_lat" in data) && ("tower_lon" in data)) {
        decoded.tower_location = "(" + data.tower_lat + "," + data.tower_lon + ")";
    }
    if (("where_lat" in data) && ("where_lon" in data)) {
        decoded.device_location = "(" + data.where_lat + "," + data.where_lon + ")";
    }
    
    decoded.rssi = data.rssi;
    decoded.bars = data.bars;
    decoded.orientation = data.orientation;
    decoded.card_temperature = data.body.temperature;
    
    // Array where we store the fields that are being sent to Datacake
    var datacakeFields = []
    
    // take each field from decodedElsysFields and convert them to Datacake format
    for (var key in decoded) {
        if (decoded.hasOwnProperty(key)) {           
            datacakeFields.push({field: key.toUpperCase(), value: decoded[key], device: device})
        }
    }      
    
    // forward data to Datacake
    return datacakeFields;
    
}