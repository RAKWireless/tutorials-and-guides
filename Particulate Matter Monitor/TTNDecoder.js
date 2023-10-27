function Decoder(bytes, port)
{
  var decoded = {};

  if (bytes[0] == 1)
  {
      
      decoded.ppm10_standard = ((bytes[1] << 8) | (bytes[2]));
      decoded.ppm25_standard = ((bytes[3] << 8) | (bytes[4]));
      decoded.ppm100_standard = ((bytes[5] << 8) | (bytes[6]));
      decoded.ppm10_env = ((bytes[7] << 8) | (bytes[8]));
      decoded.ppm25_env = ((bytes[9] << 8) | (bytes[10]));
      decoded.ppm100_env = ((bytes[11] << 8) | (bytes[12]));
      decoded.ppm_03um = ((bytes[13] << 8) | (bytes[14]))*100;
      decoded.ppm_05um = ((bytes[15] << 8) | (bytes[16]));
      decoded.ppm_10um = ((bytes[17] << 8) | (bytes[18]));
      decoded.ppm_25um = ((bytes[19] << 8) | (bytes[20]));
      decoded.ppm_50um = ((bytes[21] << 8) | (bytes[22]));
      decoded.ppm_100um = ((bytes[23] << 8) | (bytes[24]));
   
      return decoded;
  }
 else if (bytes[0] == 5) {
// add more sensor data formats here
//        } else if (bytes.[0] == xx) {
    }
}
 