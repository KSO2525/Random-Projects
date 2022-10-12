#include <math.h>


float HSVtoNitrate(int r, int g, int b){
  float h, s, v;
  float x;
  h, s, v = RGBtoHSV(r, g, b);
  //print (h, s, v)
  if (h > 350 and h < 360){
      h -= 360.0;
  }
      //print("h:", h)
  h = round(h);
//  print("final h:", h)
  if (h < 55.0 and h >= 40.0){
      x = remapRange(h, 55.0, 40.0, 0.0, 1.0);
  }
  else if (h < 40.0 and h >= 23.0){
      x = remapRange(h, 40.0, 23.0, 1.0, 2.0);
  }
  else if (h < 23.0 and h >= 20.0){
      x = remapRange(h, 23.0, 20.0, 2.0, 3.0);
  }
  else if( h < 20.0 and h >= 1.0){
      x = remapRange(h, 20.0, 1.0, 3.0, 4.0);
  }
  else if (h < 1.0 and h >= -1.0){
      x = remapRange(h, 1.0, -1.0, 4.0, 5.0);
  }
  else if (h < -1.0 and h >= -2.0){
      x = remapRange(h, -1.0, -2.0, 5.0, 6.0);
  }
  else{
      x = 0.0;
      Serial.print("JUST STOP, YOU HAVE WAY TOO MUCH NITRATE STOP KILLING THE FISHHHHH!!!!!");
  }
  Serial.print(x);
  Serial.print(2.4667 * pow(x, 2.00467));
}


float remapRange(float val, float oldMin, float oldMax, float newMin, float newMax){
  /*
    Remaps a value from one range to another range.

    Args:
        val: A number form the old range to be rescaled.
        old_min: The inclusive 'lower' bound of the old range.
        old_max: The inclusive 'upper' bound of the old range.
        new_min: The inclusive 'lower' bound of the new range.
        new_max: The inclusive 'upper' bound of the new range.

    Note:
        min need not be less than max; flipping the direction will cause the sign of
        the mapping to flip.  val does not have to be between old_min and old_max.
    */
  float oldRange;
  float newValue;
  float newRange;
  oldRange = (oldMax - oldMin);
  if (oldRange == 0){
    newValue = newMin;
  }
  else{
    newRange = (newMax - newMin);
    newValue = (((val - oldMin) * newRange) / oldRange) + newMin;
  }
  return newValue;
}

float RGBtoHSV(int r, int g, int b){
  r, g, b = r/255.0, g/255.0, b/255.0;
  // R, G, B values are divided by 255
    // to change the range from 0..255 to 0..1
    r = r / 255.0;
    g = g / 255.0;
    b = b / 255.0;
 
    // h, s, v = hue, saturation, value
    double cmax = max(r, max(g, b)); // maximum of r, g, b
    double cmin = min(r, min(g, b)); // minimum of r, g, b
    int diff = cmax - cmin; // diff of cmax and cmin.
    double h = -1, s = -1;
 
    // if cmax and cmax are equal then h = 0
    if (cmax == cmin)
        h = 0;
 
    // if cmax equal r then compute h
    else if (cmax == r)
        h = (60 * ((g - b) / diff) + 360) % 360;
 
    // if cmax equal g then compute h
    else if (cmax == g)
        h = (60 * ((b - r) / diff) + 120) % 360;
 
    // if cmax equal b then compute h
    else if (cmax == b)
        h = (60 * ((r - g) / diff) + 240) % 360;
 
    // if cmax equal zero
    if (cmax == 0)
        s = 0;
    else
        s = (diff / cmax) * 100;
 
    // compute v
    double v = cmax * 100;
    return h, s, v;
}
