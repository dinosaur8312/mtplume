# -*- coding: utf-8 -*-
"""
Created on Wed Feb 28 17:00:52 2024

@author: Stage

Working with HPAC output.

"""

import os
import pandas as pd
import numpy as np
import math
import matplotlib.pyplot as plt

import utils
import plume
from plot_plume import get_test_cases, run_test_cases


from lonlat_converter import LonLat_Converter

def read_hpac_hrf(fn):
    '''
    '''
    with open(fn) as f:
        lines = f.readlines()
        
    header = [line.replace('\n', '') for line in lines if line[0] == '#']
    
    # Remove tabs and line feeds
    fff = lambda s: s.replace('\n', '').split('\t')
    data = [fff(line) for line in lines if not line[0] == '#']
    dfs = pd.DataFrame(data, columns=['label', 'lon', 'lat', 'dose'])
    # Get rid of N and E at end of lon and lat
    dfs['lon'] = [s.replace('E', '') for s in dfs['lon']]
    dfs['lat'] = [s.replace('N', '') for s in dfs['lat']]
    
    # convert to float
    lbl = list(dfs['label'])
    lon = [float(s.replace('E', '')) for s in dfs['lon']]
    lat = [float(s.replace('N', '')) for s in dfs['lat']]
    
    const = 1.0e6   # convert from kg-s/m3 to mg-s/m3
    
    dos = [float(s) * const for s in dfs['dose']]
    
    df = pd.DataFrame([lbl, lon, lat, dos]).transpose()
    df.columns = dfs.columns
    
    
    print(df['lon'].unique())
    print(df['lat'].unique())
    
    '''
    fig, ax = plt.subplots()
    ax.scatter(df['lon'], df['lat'])
    '''
    
    lon0, lat0 = -102, 42
    
    llc = LonLat_Converter(lon0, lat0)
    xx, yy = llc.lonlat_to_xy(df['lon'], df['lat'])
    df['x'] = xx
    df['y'] = yy
    
    '''
    print(df['x'].unique())
    print(df['y'].unique())
    '''
    print()
    print('Number of x locations = ', len(df['x'].unique()))
    print('Number of y locations = ', len(df['y'].unique()))
        
    print('\nranges:')
    for c in ['x', 'y', 'dose']:
        print(c, min(df[c]),max(df[c]))
    
    return header, df

def compare_to_hpac(key, value, hp_file, path):
    '''
    '''
    
    fname = os.path.join(path, hp_file)
    header, df_hpac = read_hpac_hrf(fname)
    
    ss = '{:.5E}'.format(value)
    title = f'{key} = {ss}'
    
    # Parameters for run
    kwargs = {}
    kwargs['title'] = title
    kwargs['t'] = 1.0e6
    
    kwargs[key] = value

    #curve_var = 'dummy'
    #kwargs['curve_var'] = curve_var
    
    df0 = get_test_cases(kwargs)
    cols = [c for c in df0.columns if not c in ['x', 'y']]
    df_params = df0[cols]
    
    # Get locations from hpac results
    df_locs = df_hpac[['x', 'y']].copy()
    df_locs['hpac_dose'] = df_hpac['dose']
    
    df_in0 = pd.merge(df_params, df_locs, how='cross')
    
    # only consider x>0
    bb = df_in0['x'] > 1
    
    # REstrict to plume centerline
    #bb &= df_in0['y'] < 1
    #bb &= df_in0['y'] > -1
    
    df_in = df_in0[bb]
    
    df_out0 = run_test_cases(df_in)
    df_out = df_out0[df_out0['dose'] > 1.0e-3]
    
    dmin = min(min(df_out['hpac_dose']), min(df_out['dose']))    
    dmax = max(max(df_out['hpac_dose']), max(df_out['dose']))    
    
    fig, ax = plt.subplots()
    ax.set_title(title)
    ax.set_xlabel('x')
    ax.set_ylabel('dose')
    ax.set_xscale('log')
    ax.set_yscale('log')        
    for c in ['dose', 'hpac_dose']:
        ax.scatter(df_out['x'], df_out[c], label=c)
    ax.legend()
    
    fig, ax = plt.subplots()
    ax.set_title(title)
    ax.set_xlabel('dose py')
    ax.set_ylabel('dose hpac')
    ax.set_xscale('log')
    ax.set_yscale('log')        
    ax.scatter(df_out['dose'], df_out['hpac_dose'])

    for r in [0.5, 1.0, 2.0]:
        ax.plot([dmin, dmax], [dmin*r, dmax*r], c='tan')
    
def Q_series():
    '''
    '''
    path = 'C:\\Users\\stage\\Projects\\FastPlume\\misc'
    
    #fn = "C:\\Users\\stage\\Projects\\FastPlume\\misc\\xxx_locs.tab"
    fn = "C:\\Users\\stage\\Projects\\FastPlume\\misc\\xxx_locs_xy.tab"
    
    cols = ['Q_mg', 'hp_file']
    data = [
        [1.2e9, 'test01.tab'],
        [1.2e8, 'test02.tab'],
        [1.2e7, 'test03.tab'],
        ]
    df_runs = pd.DataFrame(data, columns=cols)
    
    for row in df_runs.iloc:
        key = 'Q_mg'
        compare_to_hpac(key, row[key], row['hp_file'], path)
        
    
    

if __name__ == '__main__':

    pass
    
    Q_series()
    
    
    if False:
        # plots
        
        fig, ax = plt.subplots()
        ax.scatter(df['x'], df['y'])
        
        '''
        fig, ax = plt.subplots()
        ax.set_yscale('log')
        for lat in df['lat'].unique():
            df0 = df[df['lat'] == lat]
            ax.plot(df0['lon'], df0['dose'])
        
        fig, ax = plt.subplots()
        ax.set_yscale('log')
        for lon in df['lon'].unique():
            df0 = df[df['lon'] == lon]
            ax.plot(df0['lat'], df0['dose'])
        '''
        
        fig, ax = plt.subplots()
        ax.set_xlabel('y')
        ax.set_ylabel('dose')
        ax.set_yscale('log')
        for x in df['x'].unique():
            df0 = df[df['x'] == x]
            ax.plot(df0['y'], df0['dose'])
        
        fig, ax = plt.subplots()
        ax.set_xlabel('x')
        ax.set_ylabel('dose')
        ax.set_yscale('log')
        for y in df['y'].unique():
            df0 = df[df['y'] == y]
            ax.plot(df0['x'], df0['dose'])
        
        
        dfcl = df[df['y'] == 0]
        
        fig, ax = plt.subplots()
        ax.set_xlabel('x')
        ax.set_ylabel('dose')
        #ax.set_xscale('log')
        ax.set_yscale('log')
        ax.plot(dfcl['x'], dfcl['dose'])
        
        fig, ax = plt.subplots()
        ax.set_xlabel('x')
        ax.set_ylabel('dose')
        ax.set_xscale('log')
        ax.set_yscale('log')
        ax.plot(dfcl['x'], dfcl['dose'])
    
    if False:
        
        # Parameters for run
        kwargs = {}
        kwargs['title'] = 'compare'
        kwargs['t'] = 1.0e6
        
        kwargs['Q_mg'] = 1.2e9  # test01
        kwargs['Q_mg'] = 1.2e8  # test02
        kwargs['Q_mg'] = 1.2e7  # test03
    
        curve_var = 'dummy'
        #kwargs['curve_var'] = curve_var
        
        df0 = get_test_cases(kwargs)
        cols = [c for c in df0.columns if not c in ['x', 'y']]
        df_params = df0[cols]
        
        df_hpac = df
        # Get locations from hpac results
        df_locs = df_hpac[['x', 'y']].copy()
        df_locs['hpac_dose'] = df_hpac['dose']
        
        df_in0 = pd.merge(df_params, df_locs, how='cross')
        
        bb = df_in0['y'] < 1
        bb &= df_in0['y'] > -1
        bb &= df_in0['x'] > 1
        df_in = df_in0[bb]
        
        df_out = run_test_cases(df_in)
        
        dmax = max(max(df_out['hpac_dose']), max(df_out['dose']))
        
        fig, ax = plt.subplots()
        ax.set_xlabel('x')
        ax.set_ylabel('dose')
        ax.set_xscale('log')
        ax.set_yscale('log')        
        for c in ['dose', 'hpac_dose']:
            ax.scatter(df_out['x'], df_out[c], label=c)
        ax.legend()

        fig, ax = plt.subplots()
        ax.set_xlabel('dose py')
        ax.set_ylabel('dose hpac')
        ax.set_xscale('log')
        ax.set_yscale('log')        
        ax.scatter(df_out['dose'], df_out['hpac_dose'])
    
        rr = 1.0e-4
        ax.plot([rr*dmax, dmax], [rr*dmax, dmax])
        
        