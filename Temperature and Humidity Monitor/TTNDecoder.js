function Decoder(bytes, port)
{
  var decoded = {};

  if (bytes[0] == 1)
  {
      
      decoded.Temperature = ((bytes[1] << 8) | (bytes[2]));
      decoded.Humidity = ((bytes[3] << 8) | (bytes[4]));
      
      return decoded;
  }
 else if (bytes[0] == 5) {
// add more sensor data formats here
//        } else if (bytes.[0] == xx) {
    }
}