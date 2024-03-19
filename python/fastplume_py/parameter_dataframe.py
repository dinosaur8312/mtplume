# -*- coding: utf-8 -*-
"""
Created on Wed Feb 21 16:22:05 2024

@author: Stage
"""

import pandas as pd
import utils

def default_params():
    '''
    '''      
    ddd = {
        'Q_mg' : 1.0e6,
        'zplume' : 0,
        'dur' : 0,
        'istab' : 3,
        'U' : 2,
        'zi' : 10000,
        'x' : 1000,
        'y' : 0,
        'z' : 0,
        't' : 1000,
        'sig_x0' : 0,
        'sig_y0' : 0,
        'sig_z0' : 0,  
           }      
    
    return ddd

def parameter_dataframe(ddd, curve_vars=[]):
    '''
    Create a DataFrame from a dictionary.
    
    Input:
        ddd, a dictionary containing parameter values
        curve_vars, names of parameters to use for curves.
        
    Output:
        df, a pandas dataFrame containing all combinations
        of the parameters.
    '''
    
    icurve = 0
    df = pd.DataFrame()
    df['icurve'] = [icurve]
    
    for k, vals0 in ddd.items():
        if not k in curve_vars:
            if utils.is_iterable(vals0):
                vals = vals0
            else:
                vals = [vals0]
            
            df1 = pd.DataFrame(vals, columns=[k])
            df = pd.merge(df, df1, how='cross')
            
    df['icurve'] = list(df.index)
    
    for k, vals0 in ddd.items():
        if k in curve_vars:
            if utils.is_iterable(vals0):
                vals = vals0
            else:
                vals = [vals0]
            
            df1 = pd.DataFrame(vals, columns=[k])
            df = pd.merge(df, df1, how='cross')
      
    return df
    
    
if __name__ == '__main__':

    pass

    #pars = default_params()
    #ddd = pars
    
    ddd = {
        'x' : 1000,
        'y' : 0,
        'z' : 0,
        't' : 1000,
           }    
    
    ddd['t'] = [0, 10, 20]
    ddd['x'] = [100, 1000]
    ddd['y'] = [5, 6, 7]
    ddd['z'] = [20, 30]
    ddd['q'] = ['aa', 'bb']
    
    curve_vars = ['t', 'q']
    
    df = parameter_dataframe(ddd, curve_vars=curve_vars)
    
    
   