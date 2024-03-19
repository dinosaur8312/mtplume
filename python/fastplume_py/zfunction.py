# -*- coding: utf-8 -*-
"""
Created on Thu Feb  8 11:32:45 2024

@author: Stage
"""

import pandas as pd
import numpy as np
import math
import matplotlib.pyplot as plt

ROOT2PI = math.sqrt(2 * math.pi)
INV_ROOT2PI = 1.0 / ROOT2PI

def zFunction(zrcp, zplume, hml, sigz):
    '''
    The vertical function for concentration and dosage.
    Includes reflections.
    '''
    if zrcp > hml and zplume <= hml:
        return 0
    
    if zplume <= hml:
        # Plume is in mixing layer.
        
        if zrcp > hml:
            # Receptor is above mixing layer
            # Assume no agent crosses the top of the mixing layer.
            return 0
    
        if sigz > hml:
            # well mixed
            zf = 1.0 / hml
            return zf
        
        # Avoid infinite loop
        # of reflections when zplume == 0
        zplume = max(0.001, zplume)
        
        # Avoid infinite loop
        # of reflections when zplume == hml
        if zplume <= hml:
            zplume = min(hml - 0.001, zplume)
            
        zrefl = zReflections(zrcp, zplume, hml, sigz)
        
    else:
        # Plume is above the mixing layer.
        
        if zrcp <= hml:
            # Recepot is in mixing layer.
            # Assume no agent crosses the top of the mixing layer.
            return 0
           
        arg = (zrcp - zplume) / sigz    
        zrefl = math.exp(-0.5 * arg * arg)    
        
        # Reflect off top of mixing layer
        arg = (zrcp + 2 * hml - zplume) / sigz    
        zrefl += math.exp(-0.5 * arg * arg)         
    
    zf = INV_ROOT2PI / sigz * zrefl
        
    return zf

def zReflections(zrcp, zplume, hml, sigz):
    '''
    Recursive routine to calculate reflections
    for zFunction.
    '''
    arg = (zrcp - zplume) / sigz    
    
    if abs(arg) > 4:
        return 0
    
    zf = math.exp(-0.5 * arg * arg)    
    
    if zplume > 0:
        # reflect off surface
        zf += zReflections(zrcp, -zplume, hml, sigz)
        
    if zplume < hml:
        # reflect off hml
        zf += zReflections(zrcp, 2 * hml - zplume, hml, sigz)
        
    return zf

def calc_zFunction(df):
    '''
    '''
    data = []
    for row in df.iloc:
        data.append(zFunction(row['zrcp'], row['zplume'], row['hml'], row['sigz']))

    df['zfunc'] = data
    
    return df

'''
================================================================
Tests
================================================================
'''

def test_hml_series(zrcp, zplume, hml, sigz, iplot=0, plot=True):
    '''
    '''
    zrcp = np.linspace(0, 2000, 201)
    
    cols = ['iplot', 'icurve', 'zrcp', 'zplume', 'hml', 'sigz']
    data = [[iplot,        0 ,  zrcp ,  zplume ,  hml ,  sigz ]] * len(zrcp)
    
    df0 = pd.DataFrame(data,columns=cols)
    df0['zrcp'] = zrcp
    
    hml_list = [2000, 1000, 750, 600, 550, 501, 499, 250]
    
    titl = ''
    for c in ['zplume', 'sigz']:
        v = df0.loc[0, c]
        titl += f'{c}={v}, '
    titl += f'\nhml={hml_list}'
    
    
    df_list = []
    
    icurve = 0
    for hml in hml_list:
        
        df = df0.copy()
        df['icurve'] = icurve
        df['hml'] = hml
        
        df = calc_zFunction(df)
        df_list.append(df)
        icurve += 1
        
    result = pd.concat(df_list)
    
    if plot:
        fig, ax = plot_zfunct(result, titl)
    
    return result    

def test_sigz_series(zrcp, zplume, hml, sigz, iplot=0, plot=True):
    '''
    '''
    zrcp = np.linspace(0, 2000, 201)
    
    cols = ['iplot', 'icurve', 'zrcp', 'zplume', 'hml', 'sigz']
    data = [[iplot ,       0 ,  zrcp ,  zplume ,  hml ,  sigz ]] * len(zrcp)
    
    df0 = pd.DataFrame(data,columns=cols)
    df0['zrcp'] = zrcp
    
    sigz_list = [100, 200, 400, 600, 700, 800]
    
    titl = ''
    for c in ['zplume', 'hml']:
        v = df0.loc[0, c]
        titl += f'{c}={v}, '
    titl += f'\nsigz={sigz_list}'
    
    df_list = []
    
    icurve = 0
    for sigz in sigz_list:
        
        df = df0.copy()
        df['icurve'] = icurve
        df['sigz'] = sigz
        
        df = calc_zFunction(df)
        df_list.append(df)
        icurve += 1
        
    result = pd.concat(df_list)
    
    if plot:
        fig, ax = plot_zfunct(result, titl)
        
    
    return result    

def test_zplume_series(zrcp, zplume, hml, sigz, iplot=0, plot=True):
    '''
    '''
    zrcp = np.linspace(0, 2000, 201)
    
    cols = ['iplot', 'icurve', 'zrcp', 'zplume', 'hml', 'sigz']
    data = [[iplot ,       0 ,  zrcp ,       0 ,  hml ,  sigz ]] * len(zrcp)
    
    df0 = pd.DataFrame(data,columns=cols)
    df0['zrcp'] = zrcp
    
    zplume_list = [0, 250, 500, 750, 1000, 1250, 1500]
    
    titl = ''
    for c in ['hml', 'sigz']:
        v = df0.loc[0, c]
        titl += f'{c}={v}, '
    titl += f'\nzplume={zplume_list}'
    
    df_list = []
    
    icurve = 0
    for zplume in zplume_list:
        
        df = df0.copy()
        df['icurve'] = icurve
        df['zplume'] = zplume
        
        df = calc_zFunction(df)
        df_list.append(df)
        icurve += 1
        
    result = pd.concat(df_list)
    
    if plot:
        fig, ax = plot_zfunct(result, titl)
        
    
    return result    

def plot_zfunct(result, titl=''):
    '''
    Plot zFunction vs receptor height.
    '''
    
    for j in result['iplot'].unique():
        
        fig, ax = plt.subplots()
        fig.suptitle(titl)
        ax.set_xlabel('zfunc')
        ax.set_ylabel('zrcp')
        
        for i in result['icurve'].unique():
            resulti = result[(result['iplot'] == j) & (result['icurve'] == i)]
            if len(resulti) > 0:
                ax.plot(resulti['zfunc'], resulti['zrcp'])
    
    return fig, ax
    

def zFunction_module_test(fout='', hml_series=True, sigz_series=True, zplume_series=True, plot=True):
    '''
    Run module tests for zFunction
    '''
    
    result_list = []
    
    iplot = 0
    
    if hml_series:
        # hml series
        zrcp, zplume, hml, sigz = 0, 0, 1000, 500    
        result_list.append(test_hml_series(zrcp, zplume, hml, sigz, iplot=iplot, plot=plot))
        
        iplot += 1
        zrcp, zplume, hml, sigz = 0, 500, 1000, 500    
        result_list.append(test_hml_series(zrcp, zplume, hml, sigz, iplot=iplot, plot=plot))
        
    if sigz_series:
        # sigz series
        
        iplot += 1
        zrcp, zplume, hml, sigz = 0, 0, 1000, 500    
        result_list.append(test_sigz_series(zrcp, zplume, hml, sigz, iplot=iplot, plot=plot))
    
        iplot += 1
        zrcp, zplume, hml, sigz = 0, 500, 1000, 500    
        result_list.append(test_sigz_series(zrcp, zplume, hml, sigz, iplot=iplot, plot=plot))
    
        iplot += 1
        zrcp, zplume, hml, sigz = 0, 1000, 1000, 500    
        result_list.append(test_sigz_series(zrcp, zplume, hml, sigz, iplot=iplot, plot=plot))
        
        iplot += 1
        zrcp, zplume, hml, sigz = 0, 1200, 1000, 500 
        #zrcp, zplume, hml, sigz = 0, 1200, 2000, 500 
        result_list.append(test_sigz_series(zrcp, zplume, hml, sigz, iplot=iplot, plot=plot))
        
    if zplume_series:
        # zplume series
        
        iplot += 1
        zrcp, zplume, hml, sigz = 0, 1200, 1000, 100 
        #zrcp, zplume, hml, sigz = 0, 1200, 2000, 500 
        result_list.append(test_zplume_series(zrcp, zplume, hml, sigz, iplot=iplot, plot=plot))
        
        iplot += 1
        zrcp, zplume, hml, sigz = 0, 1200, 1000, 500 
        #zrcp, zplume, hml, sigz = 0, 1200, 2000, 500 
        result_list.append(test_zplume_series(zrcp, zplume, hml, sigz, iplot=iplot, plot=plot))
        
        iplot += 1
        zrcp, zplume, hml, sigz = 0, 1200, 1000, 750 
        #zrcp, zplume, hml, sigz = 0, 1200, 2000, 500 
        result_list.append(test_zplume_series(zrcp, zplume, hml, sigz, iplot=iplot, plot=plot))
        
    result = pd.concat(result_list)
    
    if len(fout) > 0:
        print(f'\nWriting results to {fout}')
        result.to_csv(fout)
        
    return result


if __name__ == '__main__':    
    
    pass
    
    fout = 'test_zFunction_module.csv'
    result = zFunction_module_test(fout=fout, hml_series=True, sigz_series=True, zplume_series=True)
        
    