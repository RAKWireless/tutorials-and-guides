function Decoder(bytes, port)
{
  var decoded = {};

  if (bytes[0] == 1)
  {
      
      decoded.buttonpressed = ((bytes[1] << 8) | (bytes[2]));
      
      return decoded;
  }
 else if (bytes[0] == 5) {
// add more sensor data formats here
//        } else if (bytes.[0] == xx) {
    }
}
 
 