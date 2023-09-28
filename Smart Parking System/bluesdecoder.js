function Decoder(request) {
    var data = JSON.parse(request.body);
    var device = data.device; // this is your Device UID
 
    var decoded = {};
    decoded.buttonpressed = data.body.buttonpressed;
    
    return [
       {
             device: device,
             field: "buttonpressed",
             value: decoded.buttonpressed
       },
      
    ];
 }