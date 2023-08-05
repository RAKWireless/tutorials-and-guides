async function formatPayload(args){
    var ubidots_payload = {};
    // Log received data for debugging purposes:
    // console.log(JSON.stringify(args));
    // Get RSSI and SNR variables using gateways data:
    var gateways = args['uplink_message']['rx_metadata'];
    for (const i in gateways) {  
      // Get gateway EUI and name
      var gw = gateways[i];
      var gw_eui = gw['gateway_ids']['eui'];
      var gw_id = gw['gateway_ids']['gateway_id'];
      // Build RSSI and SNR variables
      ubidots_payload['rssi-' + gw_id] = {
        "value": gw['rssi'],
        "context": {
          "channel_index": gw['channel_index'],
          "channel_rssi": gw['channel_rssi'],
          "gw_eui": gw_eui,
          "gw_id": gw_id,
          "uplink_token": gw['uplink_token']
        }
      }
      ubidots_payload['snr-' + gw_id] = gw['snr'];
  
    }
    
    // Get Fcnt and Port variables:
    ubidots_payload['f_cnt'] = args['uplink_message']['f_cnt'];
    ubidots_payload['f_port'] = args['uplink_message']['f_port'];
    
    // Get uplink's timestamp
    ubidots_payload['timestamp'] = new Date(args['uplink_message']['received_at']).getTime(); 
    
    // If you're already decoding in TTS using payload formatters, 
    // then uncomment the following line to use "uplink_message.decoded_payload".
    // PROTIP: Make sure the incoming decoded payload is an Ubidots-compatible JSON (See https://ubidots.com/docs/hw/#sending-data)
    // var decoded_payload = args['uplink_message']['decoded_payload'];
   
    // By default, this plugin uses "uplink_message.frm_payload" and sends it to the decoding function "decodeUplink".
    // For more vendor-specific decoders, check out https://github.com/TheThingsNetwork/lorawan-devices/tree/master/vendor
    let bytes =  Buffer.from(args['uplink_message']['frm_payload'], 'base64');
    var decoded_payload = decodeUplink(bytes)['data'];
    // Merge decoded payload into Ubidots payload
    Object.assign(ubidots_payload, decoded_payload);
    return ubidots_payload
  }
  
  function decodeUplink(bytes) {
    // Decoder for the RAK1906 WisBlock Environmental Sensor (https://store.rakwireless.com/products/rak1906-bme680-environment-sensor)
    var decoded = {};
    if (bytes[0] == 1) {
        // If received data is of Environment Monitoring type
        decoded.air_humidity = (bytes[0] << 8 | (bytes[1])) / 100;
        decoded.air_temperature = (bytes[2] << 8 | (bytes[3])) / 100;
        decoded.air_pressure = (bytes[6] |(bytes[5] << 8) | (bytes[4] << 16)) / 100;
        decoded.light_level = (bytes[9] |(bytes[8] << 8) | (bytes[7] << 16));
        decoded.soil_moisture = (bytes[10] << 8 | (bytes[11]));
        decoded.soil_temperature = (bytes[10] << 8 | (bytes[11])) / 10;
    } 
    return {"data": decoded};
  }
  
  module.exports = { formatPayload };