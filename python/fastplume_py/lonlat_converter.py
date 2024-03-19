# -*- coding: utf-8 -*-
"""
Created on Wed Feb 28 20:42:33 2024

@author: Stage
"""

import math
import pandas as pd

R_EARTH =  6371e3   # in meters
RPD = math.pi / 180
M_PER_DEG_LAT = R_EARTH * RPD

def to_degrees(s):
    '''
    Convert to decimal degrees.
    s is an integer in format ddmmss.
    '''
    f = float(s) * 1.0e-4
    
    mmss, deg = math.modf(f)
    ss, mm = math.modf(mmss * 100)
    ss *= 100
    
    result = deg + (mm + ss / 60) / 60
    
    return result    

class LonLat_Converter():
    '''
    Class for converting between
    (x, y) and (longitude, latitude) coordinates.
    '''
    
    def __init__(self, lon0, lat0):
        '''
        Constructor
        '''
        
        self.lon0 = lon0
        self.lat0 = lat0
        
        self.m_per_deg_lon = (math.cos(RPD * lat0) * M_PER_DEG_LAT)
        
        
    def lonlat_to_xy(self, lon, lat):
        '''
        Convert from (longitude, latitude) to (x, y) coordinates.
        '''
        xx = [(lon1 - self.lon0) * self.m_per_deg_lon for lon1 in lon]
        yy = [(lat1 - self.lat0) * M_PER_DEG_LAT for lat1 in lat]
        
        return xx, yy
    
    def xy_to_lonlat(self, xx, yy):
        '''
        Convert from (x, y) to (longitude, latitude) coordinates.
        '''
        lon = [x / self.m_per_deg_lon + self.lon0 for x in xx]
        lat = [y / M_PER_DEG_LAT + self.lat0 for y in yy]
        
        return lon, lat
    
def test_converter():
    '''
    '''
    lon0, lat0 = -130, 30
    llc = LonLat_Converter(lon0, lat0)    
    
    dxx = pd.DataFrame([100, 500, 1000], columns=['xx'])
    dyy = pd.DataFrame([100, 500, 1000], columns=['yy'])    
    dxy = pd.merge(dxx, dyy, how='cross')
    
    lon, lat = llc.xy_to_lonlat(dxy['xx'], dxy['yy'])
    dxy['lon'] = lon
    dxy['lat'] = lat
    
    xx1, yy1 = llc.lonlat_to_xy(dxy['lon'], dxy['lat'])
    dxy['xx1'] = xx1
    dxy['yy1'] = yy1
    
    dxy['diffx'] = dxy['xx1'] - dxy['xx']
    dxy['diffx'] = dxy['xx1'] - dxy['xx']
    
    print(dxy)    
    
    
if __name__ == '__main__':    
        
    pass
    test_converter()
    




        
        