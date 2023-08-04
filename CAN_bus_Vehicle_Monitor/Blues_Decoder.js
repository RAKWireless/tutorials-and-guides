function Decoder(request) {
    var data = JSON.parse(request.body);
    var device = data.device; // this is your Device UID
 
    var decoded = {};
    decoded.CANvoltage = data.body.CANvoltage;
    decoded.CANrpm = data.body.CANrpm;
    decoded.CANoilpressure = data.body.CANoilpressure;
 
    return [
       {
             device: device,
             field: "CANvoltage",
             value: decoded.CANvoltage
       },
       {
             device: device,
             field: "CANrpm",
             value: decoded.CANrpm
       },
       {
             device: device,
             field: "CANoilpressure",
             value: decoded.CANoilpressure
       }
    ];
 }