# Random-Projects

RGB Color Sensor to Nitrate ppm Concentration
1) takes an rgb color reading from an Adafruit (TCS34725) 
2) converts RGB2HSV
3) put the Hue value into ranges of the pre-determined Hue values of the API Freshwater Nitrate Color Chart
4) uses remapRange function to get a x-val (0-6) based on which pre-determined Hue value it lies in and the result when put into an exponential regression in the nitrate ppm
