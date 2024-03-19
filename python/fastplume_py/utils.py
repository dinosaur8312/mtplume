# -*- coding: utf-8 -*-
"""
Created on Wed Jan 24 18:01:21 2024

@author: Stage
"""

import pandas as pd, numpy as np
import math
import matplotlib.pyplot as plt

from scipy.stats import norm

ROOT_PI = math.sqrt(math.pi)
CONST = math.sqrt(0.5 / math.pi)

def gauss(x):
    '''
    Gaussian normal pdf with mean=0 and sigma=1.
    Does not include 1/ROOT_2PI and therefore gauss(0) = 1
    '''
    return math.exp(-0.5 * x * x)

def Icdf(x, scale = 1):
    '''
    The integral of the normal cdf from -inf to x
    '''
    xs = x / scale
    #r = xs * norm.cdf(x, scale=scale) + CONST * (math.exp(-0.5*xs*xs) - 1) # Integral from 0 to x
    #r = xs * norm.cdf(x, scale=scale) + CONST * math.exp(-0.5*(xs*xs))        # Integral from -inf to x
    r = xs * norm.cdf(x, scale=scale) + norm.pdf(xs)
    
    return r
    
def Ierf(x):
    '''
    Integral of the error function from 0 to x.
    Ref: https://www.wolframalpha.com/input?i=integrate+erf%28x%29+dx
    '''
    r = x * math.erf(x) + (math.exp(-x*x) - 1) / ROOT_PI
    return r

def Ierf_ninf(x):
    '''
    Integral of the error function from -inf to x.
    Ref: https://www.wolframalpha.com/input?i=integrate+erf%28x%29+dx
    '''
    #r = x + x * math.erf(x) + (math.exp(-x*x) - 1) / ROOT_PI
    r = Ierf(x) + x + 1/ ROOT_PI
    return r
    
"""
def my_gauss(x, sigx=1.0):
    '''
    Gaussian normal pdf with mean=0 and sigma=sigx.    
    '''
    arg = x / sigx    
    return arg

def my_cdf(x):
    '''
    '''
    return 0.5 * (1 + math.erf(x))
    
    

def my_log(x):
    '''
    Version of math.log that returns NaN for errors.
    '''
    try:
        rr = math.log(x)
    except:
        rr = float('NaN')
    return rr
    
def my_exp(x):
    '''
    Version of math.exp that returns NaN for errors.
    '''
    try:
        rr = math.exp(x)
    except:
        rr = float('NaN')
    return rr
    
    ffTest = {
        'gauss' : gauss,
        'Ierf' : Ierf,
        'Ierf_ninf' : Ierf_ninf,
        'my_log' : my_log,
        'my_exp' : my_exp,
        'my_cdf' : my_cdf,
        'cdf'    : norm.cdf,
        }
    
"""

def test(ff, xx = np.array(np.linspace(-5,5,101))):
    
    df = pd.DataFrame()
    df['x'] = xx
    
    for nn, f in ff.items():
        df[nn] = [f(x) for x in xx]
    
    if True:
        for c in ff:
            fig, ax = plt.subplots()
            ax.plot(df['x'], df[c])
            fig.suptitle(c)
            
            
    return df

def is_ascending(ss):
    '''
    Return True is ss in in ascending order
    '''
    rr = list(ss)
    if len(rr)<2:
        return True
    prev = rr[0]
    result = True
    for s in rr[1:]:
        if s < prev:
            result = False
            break
        prev = s
    return result
    
def Icdf_diff(tau):
    
    return lambda x: Icdf(x) - Icdf(x-tau)

def is_iterable(object):
    '''
    '''
    if type(object)==str:
        # For the purposes of this program, we do not want
        # to treat strings as iterable.
        return False
        
    try:
        iter(object)
        return True
    except TypeError:
        return False
    
def try_float(object):
    '''
    If possible, convert object to float.
    Otherwise, return object.
    '''
    
    if is_iterable(object):
        result = [try_float(r) for r in object]
        return result
    
    try:
        result = float(object)
        return result
    except:
        return object
    
    
if __name__ == '__main__':    
    
    pass
    
    if False:
        ffTest = {
            'gauss' : gauss,
            'cdf' : norm.cdf,
            'Ierf_ninf' : Ierf_ninf,
            'Ierf' : Ierf,
            'Icdf' : Icdf,
            'Icdf_scale2' : lambda x: Icdf(x, scale=2),
            'Icdf_scale5' : lambda x: Icdf(x, scale=5),
            'tau=5' : Icdf_diff(5),
            'tau=10' : Icdf_diff(10),
            }
        
        xx = np.array(np.linspace(-5,15,101))
        df = test(ffTest, xx)
        print(df)
        
        print(is_ascending([1,3,5,55,77]))   # True
        print(is_ascending([1,3,5,3,55,77]))  #False
    
    if True:
        print('\nTest try_float')
        vv = [
            '1', '2.345', 'abc', 
            ['1.2', '2.3', '3.4'], 
            ['abc', '123', '3.14159']
            ]
        
        for v in vv:
            print(f'{v}')
            print(f'   {try_float(v)}')
            
        
    if False:
        xx = np.array([x for x in np.linspace(-10, 10, 101)])
        y = norm.cdf(xx) - norm.cdf(-xx)
        z = 2 * norm.cdf(xx) - 1
        
        dd = pd.DataFrame()
        dd['xx'] = xx
        dd['y'] = y
        dd['z'] = z
        df['diff'] = z - y
        
        fig,ax = plt.subplots()
        ax.plot(xx,y)
        ax.plot(xx,z)
    
    if False:
        print(is_iterable([1,2,3]) )
        print(is_iterable(1.23) )
        print(is_iterable('abc')) 
        print(is_iterable((1,2,3))) 
   
        
