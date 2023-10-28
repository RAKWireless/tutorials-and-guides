function decodeUplink(input) {
  var data = {};
  var events = {
    1: "setup",
    2: "interval",
    3: "motion",
    4: "button"
  };
  data.event = events[input.fPort];
  data.c02 = (input.bytes[0] << 8) + input.bytes[1];
  data.temperature= ((input.bytes[2] << 8) + input.bytes[3])/100;
  data.humidity = ((input.bytes[4] << 8) + input.bytes[5])/100;
  
  return {
    data: data,
  };
}
 
