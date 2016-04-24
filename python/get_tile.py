__author__ = 'catherineh'
import os

import urllib
from PIL import Image


def stitch_raw(cell_x, cell_y):
    # create a new blank image
    stitched_image = Image.new('RGBA', (256*8, 256*8))
    for x in range(0, 8):
        for y in range(0, 8):
            sub_cell_y = cell_y*8 + y
            sub_cell_x = cell_x*8 + x
            target_filename = get_texture_filename(cell_x, cell_y, sub_cell_x, sub_cell_y)
            sub_image = Image.open(target_filename)
            stitched_image.paste(im=sub_image, box=(x*256, y*256))
            #sub_image.show()
    out_extension = 'bmp'
    target_filename = get_stitched_filename(cell_x, cell_y, out_extension)
    f_out = open(target_filename, 'w+')
    stitched_image.save(f_out, out_extension)


def get_location(x=20000, y=5000):
    """
    grab the altimeter and texture tiles at location on mars x, y
    """
    high_resolution = False
    [cell_x, cell_y] = location_to_scale_alt(x, y)
    target_filename = get_altimeter_filename(cell_x, cell_y)
    #first, check to make sure the image isn't already in the cache
    if not os.path.isfile(target_filename):
        url = 'https://api.nasa.gov/mars-wmts/catalog/Mars_MGS_MOLA_DEM_mosaic_global_463m_8/1.0.0/default/' \
          'default028mm/5/'+str(cell_y)+'/'+str(cell_x)+'.png'
        print(url)
        urllib.urlretrieve(url, filename=target_filename)
    texture_ext = 'bmp'
    if high_resolution:
        # get the higher resolution viking images
        for sub_cell_x in range(cell_x*8, (cell_x+1)*8+1):
            for sub_cell_y in range(cell_y*8, (cell_y+1)*8+1):
                target_filename = get_texture_filename(cell_x, cell_y, sub_cell_x, sub_cell_y)

                if not os.path.isfile(target_filename):
                    url = 'https://api.nasa.gov/mars-wmts/catalog/Mars_Viking_MDIM21_ClrMosaic_global_232m/1.0.0/default/' \
                        'default028mm/8/'+str(sub_cell_y)+'/'+str(sub_cell_x)+'.png'
                    print(url)
                    urllib.urlretrieve(url, filename=target_filename)
    else:
        target_filename = get_stitched_filename(cell_x, cell_y, 'png')
        if not os.path.isfile(target_filename):
            url = 'https://api.nasa.gov/mars-wmts/catalog/Mars_Viking_MDIM21_ClrMosaic_global_232m/1.0.0/default/' \
                'default028mm/5/'+str(cell_y)+'/'+str(cell_x)+'.png'
            print(url)
            urllib.urlretrieve(url, filename=target_filename)

    target_filename = get_stitched_filename(cell_x, cell_y, texture_ext)
    if not os.path.isfile(target_filename):
        stitch_raw(cell_x, cell_y)


def location_to_scale_alt(x, y):
    scale = 1.0907329542893544E+06
    alt_width = 64.0
    alt_height = 32.0
    pix_width = 256
    mars_radius = 20734.0
    my_scale = alt_height/mars_radius
    cell_x = int(x*my_scale)
    cell_y = int(y*my_scale)
    return [cell_x, cell_y]


def get_altimeter_filename(cell_x, cell_y):
    target_filename = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))),
                                   'tile_cache/altimeter/'+str(cell_x)+'_'+str(cell_y)+'.png')
    return target_filename


def get_texture_filename(cell_x, cell_y, sub_cell_x, sub_cell_y):
    target_filename = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))),
                                   'tile_cache/texture/raw/'+str(cell_x)+'_'+str(cell_y)+'_'+str(sub_cell_x)+'_'+str(sub_cell_y)+'.png')
    return target_filename


def get_stitched_filename(cell_x, cell_y, texture_ext):
    target_filename = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))),
                                   'tile_cache/texture/stitched/'+str(cell_x)+'_'+str(cell_y)+'.'+texture_ext)
    return target_filename


def get_raw(x=20000, y=5000):
    """
    Get the raw bytes from the altimeter data
    :param x: the x location on Mars, in km
    :param y: the y location on Mars, in km
    :return:
    """
    get_location(x, y)
    [cell_x, cell_y] = location_to_scale_alt(x, y)
    filename = get_altimeter_filename(cell_x, cell_y)
    sub_image = Image.open(filename)
    out_data = [chr(tup[0]) for tup in list(sub_image.getdata())]
    return out_data


def get_raw_colors(x=20000, y=5000, component=0):
    """
    Get the raw bytes from the altimeter data
    :param x: the x location on Mars, in km
    :param y: the y location on Mars, in km
    :return:
    """
    get_location(x, y)
    [cell_x, cell_y] = location_to_scale_alt(x, y)
    filename = get_stitched_filename(cell_x, cell_y, 'png')
    sub_image = Image.open(filename)
    out_data = [chr(tup[component]) for tup in list(sub_image.getdata())]
    return out_data


def compress_data(in_data):
    """
    since the elevations will be pretty flat, we should compress the data
    :param in_data: a list
    :return:
    """
    out_data = []
    count = 1
    last_char = None
    for i in range(len(in_data)):
        if ord(in_data[i]) > 250:
            print("in compress data, index "+str(i)+" is: "+str(ord(in_data[i])))
        if in_data[i] == last_char and count < 255:
            count += 1
        elif count > 254:
            out_data.append(chr(count))
            out_data.append(last_char)
            count = 1
        else:
            if last_char is not None:
                out_data.append(chr(count))
                out_data.append(last_char)
            last_char = in_data[i]
            count = 1
    return out_data


def generate_test():
    """
    Generate a test image to verify the mesh server
    :return:
    """
    out_image = Image.new('L', (256, 256))
    pixels = out_image.load() # create the pixel map

    for i in range(out_image.size[0]):    # for every pixel:
        for j in range(out_image.size[1]):
            if i < 10 or j < 10 or i > 245 or j > 245:
                pixels[i,j] = 37
            else:
                pixels[i,j] = 50

    f_out = open('test_image.bmp', 'w+')
    out_image.show()
    out_image.save(f_out, 'bmp')

