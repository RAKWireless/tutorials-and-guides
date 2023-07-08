function Decoder(bytes, port) {
    
    // Basic dictionary that holds all the decoded measurement values
    var decoded = {};
    
    // Working with bytes for decoding of payload
    if (port === 2) {
        
        // Adapt this to the payload byte structure of the messages your device is sending
        var floor = bytes[0];
        var motion = bytes[1];
        var lon = ((bytes[2] << 24)|(bytes[3] << 16)|(bytes[4] << 8)|(bytes[5]))/10000000;
        var lat = ((bytes[6] << 24)|(bytes[7] << 16)|(bytes[8] << 8)|(bytes[9]))/10000000;
        var battery = bytes[4] / 10.0;
        
        console.log("received sensor data");
        
        // Working with Location
        decoded.device_location = "(" + lat + "," + lon + ")";
        
        // Working with battery
        decoded.battery = battery
        
        // Working with Sensor data
        decoded.movement= motion;
        decoded.floor = floor;
        
        // Working with status and booleans
        decoded.leakage_detected = false;
        
        // Working with Configuration Fields
        
        
    } else if (port === 100) {
        
        console.log("received configuration data");
    }    

    // Extract Gateway Metadata and LoRaWAN statistics
    // We are encapsulating this with a Try-Block in case the Gateway does not provide metadata such as RSSI, SNR, etc.
    try {
        decoded.lorawan_rssi = (!!normalizedPayload.gateways && !!normalizedPayload.gateways[0] && normalizedPayload.gateways[0].rssi) || 0;
        decoded.lorawan_snr = (!!normalizedPayload.gateways && !!normalizedPayload.gateways[0] && normalizedPayload.gateways[0].snr) || 0;
        decoded.lorawan_datarate = normalizedPayload.data_rate;           
    } catch (e) {
        console.log("Failed to read gateway metadata");
    }

    // Return data to datacake
    return decoded;

}
