function Decoder(request) {
    var data = JSON.parse(request.body);
    var device = data.device; // this is your Device UID
 
    var decoded = {};
    decoded.ppm10s = data.body.ppm10s;
    decoded.ppm25s = data.body.ppm25s;
    decoded.ppm100s = data.body.ppm100s;
    decoded.ppm10e = data.body.ppm10e;
    decoded.ppm25e = data.body.ppm25e;
    decoded.ppm100e = data.body.ppm100e;
    decoded.ppm03 = data.body.ppm03;
    decoded.ppm05 = data.body.ppm05;
    decoded.ppm10 = data.body.ppm10;
    decoded.ppm25 = data.body.ppm25;
    decoded.ppm50 = data.body.ppm50;
    decoded.ppm100 = data.body.ppm100;
    
    return [
       {
             device: device,
             field: "ppm10s",
             value: decoded.ppm10s
       },
        {
             device: device,
             field: "ppm25s",
             value: decoded.ppm25s
       },
        {
             device: device,
             field: "ppm100s",
             value: decoded.ppm100s
       },
        {
             device: device,
             field: "ppm10e",
             value: decoded.ppm10e
       },
        {
             device: device,
             field: "ppm25e",
             value: decoded.ppm25e
       },
        {
             device: device,
             field: "ppm100e",
             value: decoded.ppm100e
       },
        {
             device: device,
             field: "ppm03",
             value: decoded.ppm03
       },
        {
             device: device,
             field: "ppm05",
             value: decoded.ppm05
       },
        {
             device: device,
             field: "ppm10",
             value: decoded.ppm10
       },
       {
             device: device,
             field: "ppm25",
             value: decoded.ppm25
       },
       {
             device: device,
             field: "ppm50",
             value: decoded.ppm50
       },
       {
             device: device,
             field: "ppm100",
             value: decoded.ppm100
       },
       
    ];
 }