# -*- coding: utf-8 -*-
"""
Created on Mon Feb 26 14:55:06 2024

@author: Stage
"""

import os
import pandas as pd
import numpy as np
import math
import matplotlib.pyplot as plt

from scipy.stats import norm

import config as cfg
from zfunction import zFunction
import utils
from parameter_dataframe import parameter_dataframe

from plume import Sigma_Model, GetPlume, Plume



def default_met():
    '''
    '''
    met = {
        'istab': 3,
        'U': 3,
        'zi': 100000,
    }
    met['stab'] = 'ABCDEFG'[met['istab']]
    return met
    
def default_src():
    '''
    '''
    src = {
        'Q_mg': 1.0e6,
        'zplume': 0,
        'dur'   : 0,
        'sig_x0': 0,
        'sig_y0': 0,
        'sig_z0': 0,
        'title' : 'sample'
    }
    return src    

def default_loc():
    '''
    '''
    loc = {
        'x': 1000,
        'y': 0,
        'z': 0,
        't': 0,
    }
    return loc

def get_test_params(met, src, loc, curve_var='dummy'):
    '''
    '''
    # Met    
    df_met0 = parameter_dataframe({k: item for (k, item) in met.items() if k!=curve_var})
    df_met = df_met0[[c for c in df_met0.columns if not c=='icurve']]
    df_met['imet'] = list(df_met.index)
    
    # Source    
    df_src0 = parameter_dataframe({k: item for (k, item) in src.items() if k!=curve_var})
    df_src = df_src0[[c for c in df_src0.columns if not c=='icurve']]
    df_src['isrc'] = list(df_src.index)
    
    # Location
    df_loc0 = parameter_dataframe({k: item for (k, item) in loc.items() if k!=curve_var})
    df_loc = df_loc0[[c for c in df_loc0.columns if not c=='icurve']]
    df_loc['iloc'] = list(df_loc.index)
        
    # Release
    df = pd.merge(df_met, df_src, how='cross')
    df = pd.merge(df, df_loc, how='cross')
    
    df['icurve'] = list(range(0, len(df)))
    
    for dd in [met, src, loc]:
        if curve_var in dd:
            #curve_val = dd[curve_var]
            df_var = pd.DataFrame(dd[curve_var], columns=[curve_var])
            df = pd.merge(df, df_var, how='cross')
            break
    
    return df

def get_test_cases_v_time():
    '''
    '''
    kwargs = {}
    kwargs['dur'] = [0, 50, 100, 200, 400]
    kwargs['t'] = np.linspace(0, 1000, 101)

    curve_var = 't'
    kwargs['curve_var'] = curve_var
    
    df = get_test_cases(kwargs)
    return df, kwargs

def get_test_cases_v_dist():
    '''
    '''
    kwargs = {}
    #kwargs['dur'] = [0, 50, 100, 200, 400]
    kwargs['dur'] = [500, 10]
    kwargs['t'] = [1000, 1100, 1200,1300]
    kwargs['x'] = np.linspace(1000, 4000, 101)

    curve_var = 'x'
    kwargs['curve_var'] = curve_var
    
    df = get_test_cases(kwargs)
    return df, kwargs

def get_test_cases(kwargs={}):
    '''
    '''
    met = default_met()
    src = default_src()
    loc = default_loc()
    
    for dd in [met, src, loc]:
        for k, item in kwargs.items():
            if k in dd:
                dd[k] = item
       
    curve_var = 'dummy'
    if 'curve_var' in kwargs:
        curve_var = kwargs['curve_var']
    df = get_test_params(met, src, loc, curve_var)
    
    return df

def run_test_cases(df):
    '''
    '''
    df_list = []
    for imet in df['imet'].unique():
        
        df0 = df[df['imet']==imet]
        plume = GetPlume(df0.iloc[0])
        plume.calc_dosages(df0)
        df_list.append(df0)
        
    df = pd.concat(df_list)
    return df

def fix_icurve(df):
    '''
    Correct icurve values in df.
    
    When DataFrames are combined,
    sometimes sections of the DataFrame that
    should produce different curves have the same 
    values of icurve.
    This routine is used to reassign icurve to fix
    that problem.
    '''
    if not 'icurve' in df:
        return
    
    icurve = list(df['icurve'])
    
    jcurve = []
    ic = icurve[0]
    jc = ic
    for i in icurve:
        if i!=ic:
            # icurve has changed
            # Assign a unique new number to 
            # the next curve
            ic = i
            jc += 1
        jcurve.append(jc)
    
    df['icurve'] = jcurve
    
    
def plot_curves(df, xlabl, ylabl, fig=None, ax=None, xscale='log', yscale='log'):
    '''
    '''
    if ax == None:
        fig, ax = plt.subplots()
        ax.set_xlabel(xlabl)
        ax.set_ylabel(ylabl)        
        ax.set_xscale(xscale)
        ax.set_yscale(yscale)
        
    if type(ylabl) == list:
        for c in ylabl:
            plot_curves(df, xlabl, c, fig=fig, ax=ax)
        return
        
    if ylabl in df.columns:
        for i in df['icurve'].unique():
            dfi = df[df['icurve']==i].sort_index()
            ax.plot(dfi[xlabl], dfi[ylabl])
    else:
        print(f'{ylabl} not in df')
        
    return fig, ax
    
def plot_dosages(df, x_var='t', xscale='log', yscale='log'):
    '''
    '''
    
    fig, ax = plt.subplots(2, 1)
    ax[1].set_xlabel(x_var)
    ax[0].set_ylabel('conc')
    ax[1].set_ylabel('dosage')
    ax[0].set_xscale(xscale)
    ax[0].set_yscale(yscale)
    ax[1].set_xscale(xscale)
    ax[1].set_yscale(yscale)
    
    for i in df['icurve'].unique():
        dfi = df[df['icurve']==i]
        ii = 0
        ax[ii].plot(dfi[x_var], dfi['conc'])
        ax[ii].plot(dfi[x_var], dfi['cpeak'])
    
        ii = 1
        ax[ii].plot(dfi[x_var], dfi['dose'])
        ax[ii].plot(dfi[x_var], dfi['dinf'])
        
    return fig, ax
    
def dose_v_time(save=True):
    '''
    '''
    
    df_in_list = []
    df_list =[]
    iseries = 0
    
    # Plots for durations
    # Curves for distances
    xx = [1000, 1200, 1400]
    for dur in [0, 200, 400]:
        df_in, df = dose_v_time_sub(xx, dur)
        
        df_in['iseries'] = iseries
        df['iseries'] = iseries
        df_in_list.append(df_in)
        df_list.append(df)
        iseries += 1
    
    # Plots for distances
    # Curves for durations
    dur = [0, 200, 400]    
    for xx in [1000, 1200, 1400]:
        df_in, df = dose_v_time_sub(xx, dur)
        
        df_in['iseries'] = iseries
        df['iseries'] = iseries
        df_in_list.append(df_in)
        df_list.append(df)
        iseries += 1
        
    df_in = pd.concat(df_in_list)
    df = pd.concat(df_list)
    
    if save:
        df_in.to_csv('dose_v_time_in.csv')
        df.to_csv('dose_v_time_out.csv')

    return df_in, df
    
def dose_v_time_sub(xx, dur, tt=np.linspace(0, 1000, 101)):
    '''
    '''
    kwargs = {}
    kwargs['dur'] = dur
    kwargs['x'] = xx

    kwargs['t'] = tt
    curve_var = 't'
    kwargs['curve_var'] = curve_var
    
    x_var = 't'
    kwargs['x_var'] = x_var
    
    df_in = get_test_cases(kwargs)
    df = run_test_cases(df_in)
    
    lbl = f'dist={xx}, dur={dur}'
    for yvar in ['conc', 'dose']:
        fig, ax = plot_curves(df, x_var, yvar, xscale='linear', yscale='linear')
        ax.set_title(f'{yvar} v {curve_var} for {lbl}')
    
    fig, ax = plot_dosages(df, x_var, xscale='linear', yscale='linear')
    fig.suptitle(f'{lbl}')
    
    return df_in, df

def dose_v_dist(save=True):
    '''
    '''
    # Plots for durations
    # Curves for times
    #tt = [1000, 1200, 1400]
    tt = [1400, 1600, 1800]
    
    df_in_list = []
    df_list =[]
    
    iseries = 0
    for dur in [0, 200, 400, 800]:
        df_in, df = dose_v_dist_sub(tt, dur)
    
        df_in['iseries'] = iseries
        df['iseries'] = iseries
        df_in_list.append(df_in)
        df_list.append(df)
        iseries += 1
        
    # Plots for times
    # Curves for durations
    dur = [0, 200, 400, 800]    
    for xx in [1000, 1200, 1400]:
        df_in, df = dose_v_dist_sub(xx, dur)
        
        df_in['iseries'] = iseries
        df['iseries'] = iseries
        df_in_list.append(df_in)
        df_list.append(df)
        iseries += 1
    
    df_in = pd.concat(df_in_list)
    df = pd.concat(df_list)
    
    if save:
        df_in.to_csv('dose_v_dist_in.csv')
        df.to_csv('dose_v_dist_out.csv')

    return df_in, df
    
def dose_v_dist_sub(tt, dur, xx=np.linspace(2000, 8000, 101)    ):
    '''
    '''
    kwargs = {}
    kwargs['dur'] = dur
    kwargs['x'] = xx
    
    kwargs['t'] = tt
    curve_var = 'x'
    kwargs['curve_var'] = curve_var
    
    x_var = 'x'
    
    df_in = get_test_cases(kwargs)
    df = run_test_cases(df_in)
    
    lbl = f't={tt}, dur={dur}'
    for yvar in ['conc', 'dose']:
        fig, ax = plot_curves(df, x_var, yvar, xscale='linear', yscale='linear')
        ax.set_title(f'{yvar} v {curve_var} for {lbl}')
    
    fig, ax = plot_dosages(df, x_var, xscale='linear', yscale='linear')
    fig.suptitle(f'{lbl}')
    
    return df_in, df
    
def source_sigma_series1():
    '''
    Vary source sigmas one at a time.
    '''
    sigs = [0, 1, 5, 10, 20]
    df_in_list = []
    df_list = []
    
    iseries = 0
    
    for c in 'xyz':
        
        kwargs = {}
        labl = f'sig_{c}0'
        kwargs[labl] = sigs
        
        kwargs['x'] = np.linspace(10, 1000, 101)   
        #kwargs['x'] = [20]
        
        kwargs['t'] = [5000]
        kwargs['curve_var'] = curve_var = 'x'
        
        kwargs['x_var'] = x_var = 'x'
        
        df_in = get_test_cases(kwargs)
        df = run_test_cases(df_in)
        
        df_in['iseries'] = iseries
        df['iseries'] = iseries
        df_in_list.append(df_in)
        df_list.append(df)
        iseries += 1
        df_in_list.append(df_in)
        df_list.append(df)
    
        #vars = ['conc', 'cpeak', 'dose', 'dinf', 'sig_x', 'sig_y', 'sig_z']
        vars = ['cpeak', 'dinf', 'sig_x', 'sig_y', 'sig_z']
        
        xscale, yscale = 'log', 'log'
        #xscale, yscale = 'linear', 'linear'
        
        lbl = f'{labl} = {kwargs[labl]}'
        for yvar in vars:
            fig, ax = plot_curves(df, x_var, yvar, xscale=xscale, yscale=yscale)
            ax.set_title(f'{yvar} v {curve_var} for {lbl}')
        
        fig, ax = plot_dosages(df, x_var, xscale=xscale, yscale=yscale)
        fig.suptitle(f'{lbl}')
        
    df_in = pd.concat(df_in_list)
    df = pd.concat(df_list)
    
    return df_in, df
        
def source_sigma_series(save=True):
    '''
    Vary source sigmas as a set
    '''
    
    
    sigs = [0, 1, 5, 10, 20]
    df_in_list = []
    
    kwargs = {}
    kwargs['x'] = np.linspace(10, 1000, 101)   
    #kwargs['x'] = [0] + list(np.logspace(-3, 1, 5))
    
    kwargs['t'] = [5000]
    kwargs['curve_var'] = curve_var = 'x'
    
    kwargs['x_var'] = x_var = 'x'
    
    df_in0 = get_test_cases(kwargs)
    
    icurve = 0
    for sig in sigs:
        
        df_in = df_in0.copy()
        df_in['icurve'] = icurve
        
        df_in['sig_x0'] = sig
        df_in['sig_y0'] = sig
        df_in['sig_z0'] = sig * 0.1
        
        df_in_list.append(df_in)
        icurve += 1
        
    df_in = pd.concat(df_in_list)
    
    df = run_test_cases(df_in)
    
    #vars = ['conc', 'cpeak', 'dose', 'dinf', 'sig_x', 'sig_y', 'sig_z']
    vars = ['cpeak', 'dinf', 'sig_x', 'sig_y', 'sig_z']
    
    xscale, yscale = 'log', 'log'
    #xscale, yscale = 'linear', 'linear'
    
    lbl = ' Various source sigmas'
    for yvar in vars:
        fig, ax = plot_curves(df, x_var, yvar, xscale=xscale, yscale=yscale)
        ax.set_title(f'{yvar} v {curve_var} for {lbl}')
    
    fig, ax = plot_dosages(df, x_var, xscale=xscale, yscale=yscale)
    fig.suptitle(f'{lbl}')
    
    if save:
        df_in.to_csv('source_sigma_series_in.csv')
        df.to_csv('source_sigma_series_out.csv')
        #df_in.to_csv('debug_source_sigma_series_in.csv')
        #df.to_csv('debug_source_sigma_series_out.csv')
        
    return df_in, df
        
def zi_series(save=True):
    '''
    '''
    
    kwargs = {}
    kwargs['x'] = np.linspace(100, 50000, 101)    
    
    kwargs['t'] = 1.0e6
    
    kwargs['x_var'] = x_var = 'x'
    kwargs['curve_var'] = curve_var = 'x'
    
    kwargs['zi'] = zi = [100, 200, 500, 1000, 10000]
    
    df_in = get_test_cases(kwargs)
    df = run_test_cases(df_in)
    
    vars = ['cpeak', 'dinf', 'sig_z', 'zfunc']
    
    xscale, yscale = 'log', 'log'
    #xscale, yscale = 'linear', 'linear'
    
    lbl = f'zi = {zi}'
    for yvar in vars:
        fig, ax = plot_curves(df, x_var, yvar, xscale=xscale, yscale=yscale)
        ax.set_title(f'{yvar} v {curve_var} for {lbl}')
    
    fig, ax = plot_dosages(df, x_var, xscale=xscale, yscale=yscale)
    fig.suptitle(f'{lbl}')
    
    if save:
        df_in.to_csv('zi_series_in.csv')
        df.to_csv('zi_series_out.csv')
    
    return df_in, df
    
def read_fastplume(fn):
    '''
    '''
    df = pd.read_csv(fn)
    df['imet'] = 0
    cols = [c.strip() for c in df.columns]
    
    # translate column headings
    trans = {
        'concentration' : 'conc'
        }    
    cols = [trans[c] if c in trans else c for c in cols]
    df.columns = cols
    
    fix_icurve(df)
    
    return df    

def plot_fastplume(fn, xscale='linear', yscale='linear'):
    '''
    '''
    df = read_fastplume(fn)
    kwargs = {}
    kwargs['x_var'] = 't'
    
    plot_curves(df, kwargs['x_var'], 'conc', xscale=xscale, yscale=yscale)
    plot_curves(df, kwargs['x_var'], 'dose', xscale=xscale, yscale=yscale)
    
    df['cpeak'] = 0
    df['dinf'] = 0
    
    plot_dosages(df, kwargs['x_var'], xscale=xscale, yscale=yscale)

def read_results(fn_list, path='.'):
    '''
    '''
    df_list = []
    for fn0 in fn_list:
        fn = os.path.join(path, fn0)
        df_list.append(read_fastplume(fn))
    return df_list
    
def compare_results(df_list, x_var, xscale='linear', yscale='linear', title=''):
    '''
    '''
    vars = ['conc', 'dose']
    #vars = ['conc', 'dose', 'sig_x', 'sig_y', 'sig_z']
    
    for c in vars:
        fig, ax = None, None
        for df in df_list:
            fig, ax = plot_curves(df, x_var, c, fig=fig, ax=ax, xscale=xscale, yscale=yscale)
            ax.set_title(title)
    
    '''
    df['cpeak'] = 0
    df['dinf'] = 0
    plot_dosages(df, kwargs['x_var'], xscale=xscale, yscale=yscale)
    '''
    
def read_and_compare(fn_list, x_var, path='.', xscale='linear', yscale='linear', title=''):
    '''
    '''
    df_list = read_results(fn_list, path='.')
    compare_results(df_list, x_var, xscale=xscale, yscale=yscale, title=title + ' both')
    
    compare_results(df_list[0:1], x_var, xscale=xscale, yscale=yscale, title=title + ' fp')
    compare_results(df_list[1:2], x_var, xscale=xscale, yscale=yscale, title=title + ' py')
    
def compare(ii):
    '''
    '''
    path = '.'
    
    data = [
        ['sample'              , 'output_sample.csv'     , 'sample_out.csv', 't', 'linear', 'linear'],
        ['dose_v_time'         , 'output_dose_v_time.csv', 'dose_v_time_out.csv', 't', 'linear', 'linear'],
        ['dose_v_dist'         , 'output_dose_v_dist.csv', 'dose_v_dist_out.csv', 'x', 'log', 'log'],
        ['zi_series'           , 'output_zi_series.csv'  , 'zi_series_out.csv', 'x', 'log', 'log'],
        ['source_sigma_series' , 'output_source_sigma_series.csv', 'source_sigma_series_out.csv', 'x', 'log', 'log'],
     ]
    plot_data = pd.DataFrame(data, columns=['title', 'fn_fp', 'fn_py', 'x_var', 'xscale', 'yscale'])
    
    row = plot_data.iloc[ii]    
    title = row['title']
    fn_list = list(row[['fn_fp', 'fn_py']])
    
    read_and_compare(fn_list, row['x_var'], path=path, xscale=row['xscale'], yscale=row['yscale'], title=title)
    
def plot_kwargs(kwargs0={}):
    '''
    '''
    kwargs = kwargs0.copy()
    
    df_in = get_test_cases(kwargs)
    df = run_test_cases(df_in)
    
    x_var = kwargs['x_var']
    curve_var = kwargs['curve_var']
    label = kwargs['label']
    
    for yvar in kwargs['yvars']:
        fig, ax = plot_curves(df, x_var, yvar, xscale='linear', yscale='linear')
        ax.set_title(f'{yvar} v {curve_var} for {label}')
    
    fig, ax = plot_dosages(df, x_var, xscale='linear', yscale='linear')
    fig.suptitle(kwargs['label'] )
    
def plot_v_x_and_t(kwargs0, xpts, tpts, xx, tt):    
    '''
    '''
    
    # --------------------
    # plot vs x
    
    kwargs = kwargs0.copy()
    kwargs['x'] = xpts
    kwargs['t'] = tt
    curve_var = 'x'
    kwargs['curve_var'] = curve_var
    
    kwargs['x_var'] = 'x'
    kwargs['label'] = f'{curve_var}={tt}, {kwargs0["label"]}'
    
    plot_kwargs(kwargs)
        
    # --------------------
    # plot vs t
    
    kwargs = kwargs0.copy()
    kwargs['x'] = xx
    kwargs['t'] = tpts
    curve_var = 't'
    kwargs['curve_var'] = curve_var
    
    kwargs['x_var'] = 't'
    kwargs['label'] = f'{curve_var}={xx}, {kwargs0["label"]}'
    
    plot_kwargs(kwargs)
        
    
    if False:
        
        fn_list = ['output_dose_v_time.csv', 'dose_v_time_out.csv']
        df_list = read_results(fn_list, path='.')
        
        df0 = df_list[0]
        
        icurve = list(df0['icurve'])        
        fix_icurve(df0)
    
    
if __name__ == '__main__':

    #pass

    df_in, df = source_sigma_series()
    
    '''
    df_in, df = dose_v_time()
    df_in, df = dose_v_dist()
    df_in, df = source_sigma_series1()
    
    df_in, df = source_sigma_series()
    df_in, df = zi_series()
    
    plot_fastplume(fn)
    '''
    
    '''
    ii = 0
    compare(ii)
    
    
    ii += 1
    compare(ii)
    '''
    
    #compare(1)
    #compare(2)
    #compare(3)
    #compare(4)
    
    
    #path = '.'
    #fn = 'hpac_dispersion_coefs.csv'
    #fname = os.path.join(path, fn)
    
    #status, result = test_virtual_sources_sub_01(4, 6.7)
    
    #def dose_v_dist_sub(tt, dur, xx=np.linspace(2000, 8000, 101)    ):
    '''
    '''
    if False:
        
        # plot versus x and verus t
        
        # Define the basic scenario    
        kwargs0 = {}
        kwargs0['sig_x0'] = 10
        kwargs0['sig_y0'] = 10
        kwargs0['sig_z0'] = 2
        
        dur = 10
        kwargs0['dur'] = dur
        kwargs0['label'] = f'dur={dur}'
    
        kwargs0['yvars'] = ['conc', 'dose']
        
        xpts = list(np.linspace(-50, 50, 51))
        tpts = list(np.linspace(-50, 50, 51))
        
        tt = [0,10,20]
        tt = [10, 20, 30, 40]
        
        tt = 0, 1, 2, 3, 4
        tt = 0, 1, 5, 10, 15, 20
        
        tt = 10, 5, 2, 1, 0, 20, 40
        tt = 1, 5, 10, 14, 15, 16
        
        xx = [0, 10, 20]    
        
        plot_v_x_and_t(kwargs0, xpts, tpts, xx, tt)
    
    if False:
        # Test plots vs time
        df_in, kwargs = get_test_cases_v_time()
        df = run_test_cases(df_in)
        
        plot_curves(df, kwargs['x_var'], 'conc', xscale='linear', yscale='linear')
        plot_curves(df, kwargs['x_var'], 'dose', xscale='linear', yscale='linear')
        plot_dosages(df, kwargs['x_var'])

    if False:
        # Test plots vs dist
        df_in, kwargs = get_test_cases_v_dist()
        df = run_test_cases(df_in)
        
        x_var = 'x'
        plot_curves(df, kwargs['x_var'], 'conc')
        plot_curves(df, kwargs['x_var'], 'dose')
        plot_dosages(df, kwargs['x_var'])

    if False:
        
        x_var = 'x'
        plot_curves(df, x_var, 'conc')
        plot_curves(df, x_var, 'ctip')
        plot_curves(df, x_var, 'ctail')
    
        plot_curves(df, x_var, 'dose')
        
        #plot_curves(df, x_var, 'tip0')
        #plot_curves(df, x_var, 'tip1')
        plot_curves(df, x_var, 'tip')
        
        #plot_curves(df, x_var, 'tail1')
        #plot_curves(df, x_var, 'tail0')
        #plot_curves(df, x_var, 'tails')
        plot_curves(df, x_var, 'tail')
        plot_curves(df, x_var, 'tip-tail')
        plot_curves(df, x_var, 'dose')
        
        plot_curves(df, x_var, ['tip1', 'tip0', 'tail1'])
    
    if False:
        plot_dosages(df)


    if False:
        Q_mg, zplume, dur = 1.0e6, 0, 0
        istab, U, zi = 3, 1, 100000
        title = 'test'

        plume = Plume(Q_mg, zplume, dur, istab, U, zi, title)

        xx = [10, 100, 1000]

        zrcp = 0

        df = pd.DataFrame()
        df['x'] = xx
        df['y'] = 0
        df['z'] = zrcp
        df['t'] = 100

        plume.calc_sigmas(df)

        #sigmas = plume.sigma_model.sigmas

        plume.calc_zFunction(df)
        plume.calc_peak_concentration(df)
        plume.calc_dosage_inf(df)

        plume.calc_concentration(df)
        plume.calc_dosage(df)
        df

    if False:
        
        fname = cfg.HPAC_COEF_FILE
        istab, U = 3, 5
        sigma_model = Sigma_Model(fname, istab, U)

        piv = sigma_model.piv
        x_virt = sigma_model.x_virt

        data = [
            [10, 100, 10],
            [20, 50, 20],
            [25, 25, 5],
            [25, 25, 0],
            [25, .01, 5],
            [125000, 25, 10]
        ]
        data = [
            [10, 100, 10],
        ]
        sig0 = pd.DataFrame(data, columns=['sig_x', 'sig_y', 'sig_z'])

        sigma_model. calc_virtual_sources(sig0)
        x_virt = sigma_model.x_virt
        print(x_virt)

        xx = [-100, -10, 0] + list(np.logspace(-1, 3, 9))

        result_list = []
        for s0 in sig0.loc:
            result = pd.DataFrame()
            result['istab'] = len(xx) * [istab]
            result['speed'] = len(xx) * [U]
            result['stab'] = len(xx) * ['ABCDEFG'[istab]]

            sigma_model.calc_virtual_sources(pd.DataFrame(s0))
            sigma_model.calc_sigmas(xx)
            result[sigma_model.sigmas.columns] = sigma_model.sigmas[sigma_model.sigmas.columns]

            result_list.append(result)

        #qq = list(map(math.log, list(piv.index)))

    if False:
        sigma_model = Sigma_Model(fname)

        pivot = sigma_model.pivot
        istabs = sigma_model.istabs
        speeds = sigma_model.speeds
        dists = sigma_model.dists

        # pivot.loc[4,:]

        # pivot.loc[:,speeds[2]]

        pivot[pivot['istab'] == 1]

        pivot.loc[1, 2.3]
        pivot.loc[1, 2.3, 10.0]
        pivot.loc[1, 2.3, 10.0:100.0]
        pivot.loc[1:2, 2.3, 10.0:100.0]

        pivot.loc[1:2, 2.3, 9.0:100.0]
        '''
        pivot = read_model_coef_pivot(fname)
        
        Q_mg, istab, U =  1.0e6, 4, 5
        
        plume = Plume(model_coefs, Q_mg, istab, U)
        
        model_coefs.loc[istab, 6.7]
        
        
        speeds = sorted(pd.Series([i[1] for i in model_coefs.index]).unique())
    
        path = '.'
        fn = 'fit_param_tab_all.csv'
        fname = os.path.join(path, fn)
        mc = pd.read_csv(fname)
        mc1 = mc[mc['variable']=='sig_y'].copy()
        mc1['variable'] = 'sig_x'
        
        mc2 = pd.concat([mc,mc1])
        
        '''
