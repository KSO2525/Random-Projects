#color1red = 235.0
#color1green = 172.0
#color1blue = 43.0

#color2red = 149.0
#color2green = 46.0
#color2blue = 49.0


def remap_range(
    val: float,
    old_min: float,
    old_max: float,
    new_min: float,
    new_max: float,
) -> float:
    """
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
    """
    # TODO: remap val to the new range
    old_range = (old_max - old_min)
    if (old_range == 0):
        new_value = new_min
    else:
        new_range = (new_max - new_min)  
        new_value = (((val - old_min) * new_range) / old_range) + new_min
    return new_value

r = float(input("R: "))
g = float(input("G: "))
b = float(input("B: "))

def rgb_to_hsv(r, g, b):
 
    # R, G, B values are divided by 255
    # to change the range from 0..255 to 0..1:
    r, g, b = r / 255.0, g / 255.0, b / 255.0
 
    # h, s, v = hue, saturation, value
    cmax = max(r, g, b)    # maximum of r, g, b
    cmin = min(r, g, b)    # minimum of r, g, b
    diff = cmax-cmin
    # if cmax and cmax are equal then h = 0
    if cmax == cmin:
        h = 0
     
    # if cmax equal r then compute h
    elif cmax == r:
        h = (60 * ((g - b) / diff) + 360) % 360
 
    # if cmax equal g then compute h
    elif cmax == g:
        h = (60 * ((b - r) / diff) + 120) % 360
 
    # if cmax equal b then compute h
    elif cmax == b:
        h = (60 * ((r - g) / diff) + 240) % 360
 
    # if cmax equal zero
    if cmax == 0:
        s = 0
    else:
        s = (diff / cmax) * 100
 
    # compute v
    v = cmax * 100
    return h, s, v
#rp = (r - color1red)/(color2red-color1red)
#gp = (b - color1green)/(color2green-color1green)
#bp = (g - color1blue)/(color2blue-color1blue)
#x = ((rp + gp + bp)/3)
#print(x)
#x = remap_range(va, 10.0, 100.0, 1.0, 6.0)
#add_color = float(input("put in R+G+B number as float: "))
#x = remap_range(add_color, 384.0, 403.0, 1.0, 6.0)

h, s, v = rgb_to_hsv(r, g, b)
print (h, s, v)
if h > 350 and h < 360:
    h -= 360.0
    print("h:", h)
h = round(h, 0)
print("final h:", h)
if h < 55.0 and h >= 40.0:
    x = remap_range(h, 55.0, 40.0, 0.0, 1.0)
elif h < 40.0 and h >= 23.0:
    x = remap_range(h, 40.0, 23.0, 1.0, 2.0)
elif h < 23.0 and h >= 20.0:
    x = remap_range(h, 23.0, 20.0, 2.0, 3.0)
elif h < 20.0 and h >= 1.0:
    x = remap_range(h, 20.0, 1.0, 3.0, 4.0)
elif h < 1.0 and h >= -1.0:
    x = remap_range(h, 1.0, -1.0, 4.0, 5.0)
elif h < -1.0 and h >= -2.0:
    x = remap_range(h, -1.0, -2.0, 5.0, 6.0)
else:
    x = 0.0
    print("JUST STOP, YOU HAVE WAY TOO MUCH NITRATE STOP KILLING THE FISHHHHH!!!!!")
print(x)
print(2.4667 * (2.00467 ** x))
#print(2.4667 * (2.00467 ** (x * 6.0 + 0.5)))
