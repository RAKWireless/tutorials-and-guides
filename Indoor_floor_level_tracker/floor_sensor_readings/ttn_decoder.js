function Decoder(bytes, port) {
    
    // Basic dictionary that holds all the decoded measurement values
    var decoded = {};
    
    // Working with bytes for decoding of payload
    if (port === 2) {
        
        // Adapt this to the payload byte structure of the messages your device is sending
        var temperature = ((bytes[0] << 16)|(bytes[1] << 8)|(bytes[2]))/100;
        var pressure = ((bytes[3] << 16)|(bytes[4] << 8)|(bytes[5]))/100;
        var altitude= ((bytes[6] << 16)|(bytes[7] << 8)|(bytes[8]))/100;
        
        console.log("received sensor data");
        
        // Working with Location
        decoded.Temperature = temperature;
        
        // Working with battery
        decoded.Pressure = pressure
        
        // Working with Sensor data
        decoded.Altitude = altitude;
        
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