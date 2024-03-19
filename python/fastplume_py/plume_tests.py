# -*- coding: utf-8 -*-
"""
Created on Mon Feb  5 13:38:46 2024

@author: Stage

Tests for plume.py

"""
import os
import pandas as pd
import numpy as np
import math
import matplotlib.pyplot as plt

import plume

# ==============================================================
# Tests for plume.Sigma_Model
# ==============================================================

def test_virtual_sources(iopt=2, fout=''):
    '''
    Test that Sigma_Model.calc_virtual_sources is the inverse of Sigma_Model.calc_sigmas
    '''
    
    print(f'\ntest_virtual_sources, iopt={iopt}')
    
    path = '.'
    fn = 'hpac_dispersion_coefs.csv'
    fname = os.path.join(path, fn)
    
    sigma_model = plume.Sigma_Model(fname)
    speeds = sigma_model.speeds
    
    if iopt==2:
        speeds = list(range(1,13)) + [15]
    
    status_list = []
    result_list = []
    fail_list = []
    for istab in range(0,7):
        for U in speeds:
            
            if iopt==1 and not sigma_model.is_valid_combination(istab, U):
                # not valiid case
                # skip it.
                pass
            
            else:
                if iopt==1:
                    status, result = test_virtual_sources_sub_01(istab, U)
                else:
                    status, result = test_virtual_sources_sub_02(istab, U)
                
                status_list.append(status)
                result_list.append(result)
                if status=='Fail':
                    fail_list.append(f'{istab}. {U}')

    result = pd.concat(result_list)    
    
    npass = len([s for s in status_list if s == 'Pass'])
    nfail = len([s for s in status_list if s == 'Fail'])
    status = ['Fail', 'Pass'][nfail==0]
    if status=='Fail':
        print('\nCases that fail:')
        for f in fail_list:
            print(f'   {f}')
        print()
        
    print(f'\n{len(status_list)} Cases tested,\n{npass} Pass\n{nfail} Fail')
    
    if len(fout)>0:
        print(f'\nWriting results to {fout}')
        result.to_csv(fout)
        
    return status, result
    
    
def test_virtual_sources_sub_01(istab, U):
    '''
    Test that Sigma_Model.calc_virtual_sources is the inverse of Sigma_Model.calc_sigmas
    
    This test uses the same virtual sources for sigma_x, sigma_y and sigma_z.
    '''
    
    path = '.'
    fn = 'hpac_dispersion_coefs.csv'
    fname = os.path.join(path, fn)
    
    sigma_model = plume.Sigma_Model(fname, istab, U)
    
    piv = sigma_model.piv
    
    #xx = [-1, -2, 0, 2, 3, 4, 99900.0, 100100.0, 100200.0]
    xx = sorted([-200, -100, 0, 0.1] + list(np.logspace(0.1,5,11)) + [99990, 100010, 150000])
    #xx = [99990, 100000]
    
    result = pd.DataFrame()
    result['istab'] = len(xx) * [istab]
    result['speed'] = len(xx) * [U]
    result['stab'] = len(xx) * ['ABCDEFG'[istab]]
    
    result['x'] = xx
    sigma_model.calc_sigmas(xx)
    sigmas = sigma_model.sigmas
    #for c in sigmas.columns:
    #    result[c] = sigmas[c].copy()
    
    sigma_model.calc_virtual_sources(sigmas)
    x_virt = sigma_model.x_virt
    for c in x_virt.columns:
        result[c] = x_virt[c]
        
    for c in ['x', 'y', 'z']:
        result[f'diff_{c}'] = result[f'xv_{c}'] - result['x']
            
    xmin = min(piv.index)
    xmax = max(piv.index)
    
    tol = 1.0e-3
    data = []
    for row in result.iloc:
        if row['x'] < xmin:
            stat = abs(row['xv_x'] - xmin) < tol
            stat &= abs(row['xv_y'] - xmin) < tol
            stat &= abs(row['xv_z'] - xmin) < tol
        elif row['x'] > xmax:
            stat = abs(row['xv_x'] - xmax) < tol
            stat &= abs(row['xv_y'] - xmax) < tol
            stat &= abs(row['xv_z'] - xmax) < tol
        else:
            stat = abs(row['diff_x']) < tol
            stat &= abs(row['diff_y']) < tol
            stat &= abs(row['diff_z']) < tol
        data.append(stat)
            
    result['Pass'] = data
    
    tf = len(result[result['Pass']==False]) == 0
    status = ['Fail', 'Pass'][tf]
    print(f'test_virtual_sources(istab={istab}, U={U}) = {status}')
    
    return status, result
    
def test_virtual_sources_sub_02(istab, U):
    '''
    Test that Sigma_Model.calc_virtual_sources is the inverse of Sigma_Model.calc_sigmas
    
    This test uses different virtual sources for sigma_x, sigma_y and sigma_z.
    '''
    
    path = '.'
    fn = 'hpac_dispersion_coefs.csv'
    fname = os.path.join(path, fn)    
    
    sigma_model = plume.Sigma_Model(fname, istab, U)
    
    piv = sigma_model.piv
    
    #xx = [-1, -2, 0, 2, 3, 4, 99900.0, 100100.0, 100200.0]
    xx = pd.Series(sorted([-200, -100, 0, 0.1] + list(np.logspace(0.1,5,11)) + [99990, 100010, 150000]))
    
    result = pd.DataFrame()
    result['istab'] = [istab] * len(xx)
    result['speed'] = [U] * len(xx)
    result['stab'] = ['ABCDEFG'[istab]] * len(xx)
    
    rr = {'x' : 0.66, 'y' : 2.99, 'z' : 5.3}
    
    for s in 'xyz':
        result[f'x0{s}'] = xx * rr[s]
    
    for s in 'xyz':    
        sigma_model.calc_sigmas(result[f'x0{s}'])        
        result[f'sig_{s}'] = sigma_model.sigmas[f'sig_{s}']
    
    sigma_model.calc_virtual_sources(result)
    
    for s in 'xyz':
        result[f'xv_{s}'] = sigma_model.x_virt[f'xv_{s}']
    
    for s in 'xyz':
        result[f'diff_{s}'] = result[f'xv_{s}'] - result[f'x0{s}']
            
    xmin = min(piv.index)
    xmax = max(piv.index)
    
    tol = 1.0e-3
    data = []
    for row in result.iloc:
        stat = True
        for s in 'xyz':
            if row[f'x0{s}'] < xmin:
                stat &= abs(row[f'xv_{s}'] - xmin) < tol
            elif row[f'x0{s}'] > xmax:
                stat &= abs(row[f'xv_{s}'] - xmax) < tol
            else:
                stat &= abs(row[f'diff_{s}']) < tol
        data.append(stat)
            
    result['Pass'] = data
    
    tf = len(result[result['Pass']==False]) == 0
    status = ['Fail', 'Pass'][tf]
    print(f'test_virtual_sources(istab={istab}, U={U}) = {status}')
    
    # Relabel sigma columns
    cols = [f'{c}0' if 'sig' in c else c for c in result.columns]
    result.columns = cols
    
    return status, result
    

def test_calc_sigmas(fname, fout=''):
    '''
    Test computing sigmas for cases with non-zero source sigmas.
    '''
    
    istab, U = 3, 5
    sigma_model = plume.Sigma_Model(fname, istab, U)
    
    cases = [
        [3, 5],   # istab, U
        [4, 2.5],
        ]
    
    result_list = []
    for istab, U in cases:
        res = test_calc_sigmas_sub(sigma_model, istab, U)
        result_list.append(res)
        
    df = pd.concat(result_list)
    
    if len(fout)>0:
        print(f'\nSaving results to {fout}')
        df.to_csv(fout)
        
    return df
    

def test_calc_sigmas_sub(sigma_model, istab=3, U=5):
    '''
    Test computing sigmas for cases with non-zero source sigmas.
    '''

    piv = sigma_model.piv
    x_virt = sigma_model.x_virt
    
    sigmas = [
        # sig_x0, sig_y0, sig_z0
        [0, 0, 0],
        [10, 100, 5],   
        [20, 50, 12],
        [25, 15, 5],
        [15, 25, 0],
        [25, .01, 5],
        [125000, 25, 10]
        ]
    '''
    sigmas = [
        [10, 100, 5],
        ]
    '''
    sig0 = pd.DataFrame(sigmas, columns=['sig_x', 'sig_y', 'sig_z'])
    
    sigma_model.calc_virtual_sources(sig0)
    x_virt = sigma_model.x_virt
    #print(x_virt)
    
    xx = [-100, -10, 0] + list(np.logspace(-1, 3, 9)) + [100000, 120000]
    
    result_list = []
    for s0 in sig0.iloc:
        result = pd.DataFrame()
        result['istab'] = len(xx) * [istab]
        result['speed'] = len(xx) * [U]
        result['stab'] = len(xx) * ['ABCDEFG'[istab]]
        
        #print('\n', pd.DataFrame(s0).transpose())
        for c in 'xyz':
            result[f'sig_{c}0'] = len(xx) * [s0[f'sig_{c}']]
        
        sigma_model.calc_virtual_sources(pd.DataFrame(s0).transpose())
        sigma_model.calc_sigmas(xx)
        result[sigma_model.sigmas.columns] = sigma_model.sigmas[sigma_model.sigmas.columns]
        
        result_list.append(result)
    
        #print(result)
        
    df = pd.concat(result_list)
    
    return df
    
# ==============================================================
# Tests for plume.Plume
# ==============================================================

# *** TODO ***    

# ==============================================================

if __name__ == '__main__':    
    
    pass
    
    path = '.'
    fn = 'hpac_dispersion_coefs.csv'
    fname = os.path.join(path, fn)
    
    #status, result = test_virtual_sources_sub_01(4, 6.7)
    
    if False:
        #stat, res = test_virtual_sources_sub_02(istab=1, U=0.8)
        stat, res = test_virtual_sources_sub_02(istab=1, U=1.0)
        
    if False:
        #status1, result1 = test_virtual_sources(iopt=1, fout = 'test_virtual_sources_01.csv')
        status2, result2 = test_virtual_sources(iopt=2, fout = 'test_virtual_sources_02.csv')
    
    if True:
        #status1, result1 = test_virtual_sources(iopt=1)
        status2, result2 = test_virtual_sources(iopt=2)
    
    if False:        
        result = test_calc_sigmas(fname, fout='test_calc_sigmas.csv')
        
        
        
    
