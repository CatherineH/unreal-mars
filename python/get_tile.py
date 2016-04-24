__author__ = 'catherineh'

import wget
import os

import urllib
from PIL import Image


scale = 1.0907329542893544E+06
width = 512
height = 256
pix_width = 256
mars_radius = 20734
my_scale = (width*pix_width)/mars_radius


def stitch_raw(cell_x, cell_y):
    # create a new blank image
    stitched_image = Image.new('RGBA', (256*8, 256*8))
    stitched_image.show()
    for x in range(0, 8):
        for y in range(0, 8):
            sub_cell_y = cell_y*8 + y
            sub_cell_x = cell_x*8 + x
            target_filename = os.path.join(os.path.dirname(os.path.realpath(__file__)),
                                   'tile_cache/texture/raw/'+str(cell_x)+'_'+str(cell_y)+'_'+str(sub_cell_x)+'_'+str(sub_cell_y)+'.png')
            print(target_filename)
            sub_image = Image.open(target_filename)
            print(sub_image.format, sub_image.size, sub_image.mode)
            stitched_image.paste(im=sub_image, box=(x*256, y*256))
            #sub_image.show()
    out_extension = 'bmp'
    target_filename = os.path.join(os.path.dirname(os.path.realpath(__file__)),
                                   'tile_cache/texture/stitched/'+str(cell_x)+'_'+str(cell_y)+'.'+out_extension)
    stitched_image.show()
    print(sub_image.format, sub_image.size, sub_image.mode)
    f_out = open(target_filename, 'w+')
    stitched_image.save(f_out, out_extension)


def get_location(x=20000, y=5000):
    """
    grab the altimeter and texture tiles at location on mars x, y
    """
    scale = 1.0907329542893544E+06
    alt_width = 64.0
    alt_height = 32.0
    pix_width = 256
    mars_radius = 20734.0
    my_scale = alt_height/mars_radius
    print(my_scale)
    cell_x = int(x*my_scale)
    cell_y = int(y*my_scale)
    print(str(cell_x)+" "+str(cell_y))
    target_filename = os.path.join(os.path.dirname(os.path.realpath(__file__)),
                                   'tile_cache/altimeter/'+str(cell_x)+'_'+str(cell_y)+'.png')
    #first, check to make sure the image isn't already in the cache
    if not os.path.isfile(target_filename):
        url = 'https://api.nasa.gov/mars-wmts/catalog/Mars_MGS_MOLA_DEM_mosaic_global_463m_8/1.0.0/default/' \
          'default028mm/5/'+str(cell_y)+'/'+str(cell_x)+'.png'
        print(url)
        urllib.urlretrieve(url, filename=target_filename)
    texture_ext = 'bmp'
    # get the higher resolution viking images
    for sub_cell_x in range(cell_x*8, cell_x*9+1):
        for sub_cell_y in range(cell_y*8, cell_y*9+1):
            target_filename = os.path.join(os.path.dirname(os.path.realpath(__file__)),
                                   'tile_cache/texture/raw/'+str(cell_x)+'_'+str(cell_y)+'_'+str(sub_cell_x)+'_'+str(sub_cell_y)+'.png')
            if not os.path.isfile(target_filename):
                url = 'https://api.nasa.gov/mars-wmts/catalog/Mars_Viking_MDIM21_ClrMosaic_global_232m/1.0.0/default/' \
                    'default028mm/8/'+str(sub_cell_y)+'/'+str(sub_cell_x)+'.png'
                print(url)
                urllib.urlretrieve(url, filename=target_filename)

    target_filename = os.path.join(os.path.dirname(os.path.realpath(__file__)),
                                   'tile_cache/texture/stitched/'+str(cell_x)+'_'+str(cell_y)+'.'+texture_ext)
    if not os.path.isfile(target_filename):
        stitch_raw(cell_x, cell_y)



get_location()