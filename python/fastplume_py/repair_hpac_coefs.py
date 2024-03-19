# -*- coding: utf-8 -*-
"""
Created on Fri Jan 26 14:03:52 2024

@author: Stage

==================================================
Repair various issues with values 
in hpac_dispersion_coefs.csv.

==================================================
Fix FastPlume model coefficients to remove
violations of the Second Law of Thermodynamics.

The FastPlume coefficents determined by matching
HPAC results sometimes violate the Second Law
by having sigmas that decrease with downwind distance.


6 runs have this behavior:
    
    Runs that fail:
    0, 0.8, True, True, False
    0, 2.3, True, True, False
    2, 2.3, True, True, False
    2, 4.1, True, True, False
    2, 12.0, True, True, False
    4, 6.7, True, False, False
    6 runs fail.
    
For the first 5 of these, the problems are minor.
They involve only a few points and the points
are at long downwind distances where the depth of the 
mixing layer is important. For these points,
sigma_z is large compared to Z_i.
(HPSC runs used Z_i=5000m).

These cases were fixed by setting z_i to the
max of all previous values.


*** TODO ***
For the case of E, 6.7 m/s there are more serious 
problems that must be fixed.
*************

==================================================
fix_missing_distances

The run for istab=4, 6.7 m/s was missing coefficients 
for x=[3.1622776601683795, 10.0, 31.6227766016838, 100.0]

These values were filled in by interpolation.

==================================================
add_sigma_near_source


==================================================

"""

import pandas as pd
import matplotlib.pyplot as plt

from utils import is_ascending
from plume import Sigma_Model

def monotonic_series(ss):
    '''
    If ss is monotonically non-decreasing, return (True, ss)
    
    Otherwise, return (False, ss), where the series has
    been made monotonic by increasing values to 
    the max of prior terms, as needed.
    '''
    
    if len(ss) < 2:
        return False, ss
    
    fix = False
    vmax = ss[0]
    for i in range(0, len(ss)):
        s = ss[i]
        if s < vmax:
            ss[i] = vmax + 1   # This helps for plume.
            fix = True
        vmax = ss[i]
        
    return fix, ss


def fix_second_law(fname, fout=''):
    '''
    Repair cases having sigmas that decrease with 
    downwind distance.
    
    Set sigmas equal to the max of previous values.
    '''
    print(f'\nfix_second_law, {fname}')
    mc0 = pd.read_csv(fname)
    m1 = mc0[mc0['how'] == 'fit']
    m2 = m1[['istab', 'wind', 'stab', 'x', 'sig_x', 'sig_y', 'sig_z', 'how']].copy()
    
    istabs = m2['istab'].unique()
    speeds = m2['wind'].unique()
    
    nfix = 0
    fixed = []
    istab = 0
    speed = 2.3
    for istab in istabs:
        for speed in speeds:
            bb = (m2['istab']==istab) & (m2['wind'] == speed)
            for sig in ['sig_x', 'sig_y', 'sig_z']:
                sigs = list(m2.loc[bb][sig])
                fix, ss = monotonic_series(sigs)                
                if fix:
                    print(f'Fixing {istab}, {speed}, {sig}')
                    m2.loc[bb, sig] = ss
                    nfix += 1
                    fixed.append([istab, speed, sig])
    print(f'{nfix} series fixed.')
    
    if fout != '':
        print(f'Write results to {fout}')
        m2.to_csv(fout)
        
    return fixed

def Test_Second_Law(fname, plot=True):
    '''
    Look for cases that have sigmas which decrease with donwwind distance.
    Plot cases tjhat fail.
    '''
    print(f'\nTest_Second_Law, {fname}')
    print('\nRuns that fail:')
    
    sigma_model = Sigma_Model(fname)
    pivot = sigma_model.pivot
    
    df = pd.DataFrame([list(i) for i in pivot.index])
    df.columns=['istab', 'U', 'x']
    
    for c in df.columns:
        pivot[c] = list(df[c])
        
    fails = {}
    for istab in pivot['istab'].unique():
        piv0 = pivot[pivot['istab'] == istab]
        for U in piv0['U'].unique():
            piv1 = piv0[piv0['U'] == U]
            
            bx = is_ascending(piv1['sig_x'])
            by = is_ascending(piv1['sig_y'])
            bz = is_ascending(piv1['sig_z'])
            bb = bx & by & bz
            if not bb:
                print(f'{istab}, {U}, {bx}, {by}, {bz}')
                fails[f'{istab}, {U}, {bx}, {by}, {bz}'] = piv1
    
    print(f'{len(fails)} runs fail.')
    
    if plot:
        plot_fails(fails)
    
    return fails    

def  plot_fails(fails):
    '''
    Plot sigmas v dist for cases that fail.
    '''
    for lbl, pp in fails.items():
        
        fig, ax = plt.subplots(1, 3, figsize=(10,3))
        fig.suptitle(lbl)
        for ax0 in ax:
            ax0.set_xscale('log')
            ax0.set_yscale('log')
            ax0.set_xlabel('x')
        ax[0].set_title('sig_x')
        ax[1].set_title('sig_y')
        ax[2].set_title('sig_z')
        ii = 0
        ss = ['sig_x', 'sig_y', 'sig_z']
        for s in ss:
            ax[ii].plot(pp['x'], pp[s])
            ax[ii].scatter(pp['x'], pp[s])
            ii += 1

def fix_missing_distances(fname, fout=''):      
    '''
    Check that all values of x are included for each run.
    If any are missing, insert them usign interpolation.
    '''
    
    sigma_model = Sigma_Model(fname)
    
    mc = pd.read_csv(fname)
    mc1 = mc[mc['how'] == 'fit']
    
    istabs = sorted(mc1['istab'].unique())
    speeds = sorted(mc1['wind'].unique())
    dists = sorted(mc1['x'].unique())
    
    coef_list = []
    print('\nCheck for missing distances.')
    for istab in istabs:
        for speed in speeds:
            bb = (mc1['istab']==istab) & (mc1['wind']==speed)
            mc2 = mc1.loc[bb]
            if len(mc2)>0:
                dist2 = mc2['x'].unique()
                missing = [x for x in dists if not x in dist2]
                if len(missing)>0:
                    print(f'{istab}, {speed} is missing x: {missing}')
                
                    if len(fout)>0:
                        sigma_model.set_met(istab, speed)
                        sigma_model.calc_sigmas(missing)
                        sigmas = sigma_model.sigmas
                        sigmas['istab'] = istab
                        sigmas['wind'] = speed
                        sigmas['stab'] = 'ABCDEFG'[istab]
                        sigmas['how'] = 'fit'
                        print(sigmas)
                        coef_list.append(sigmas)
    
    if len(coef_list) <1:
        print('No distances are missing.')
        
    if len(fout)>0:
        coefs_out = pd.concat([mc] + coef_list)
        print(f'\nSaving to {fout}')
        coefs_out.to_csv(fout)

def add_sigma_near_source(fname, fout=''):
    '''
    The sigmas obtained from fitting hpac have
    sigmas for x >=  1m.
    
    For each stability and wind speed combination,
    add a point at x = 0.01m that has
    sig_x = sig_y = sig_z = 0.01m.
    
    This makes it easier to do interpolations
    for sigmas and for virtual sources.
    '''
    
    mc = pd.read_csv(fname)
    mc1 = mc[mc['how'] == 'fit']
    
    istabs = sorted(mc1['istab'].unique())
    speeds = sorted(mc1['wind'].unique())
    dists = sorted(mc1['x'].unique())
    
    coef_list = []
    print('\nadd_sigma_near_source.')
    
    xmin = 0.001     # 1 mm
    sigmin = 0.001   # 1 mm
    
    coef_list = []
    
    for istab in istabs:
        for speed in speeds:
            bb = (mc1['istab']==istab) & (mc1['wind']==speed)
            mc2 = mc1.loc[bb]
            if len(mc2)>0:
                if min(mc2['x']) > xmin:
                    row = mc2.iloc[0].copy()
                    row['x'] = xmin
                    for c in 'xyz':
                        row[f'sig_{c}'] = sigmin
                    coef_list.append(pd.DataFrame(row).transpose())
                coef_list.append(mc2)
                
    coefs_out = pd.concat(coef_list)
    
    if len(fout)>0:
        print(f'\nSaving to {fout}')
        coefs_out.to_csv(fout)
    
def plot_hpac_coefs(fname):
    '''
    Plot sigmas
    '''
    
    mc = pd.read_csv(fname)
    mc1 = mc[mc['how'] == 'fit']
    
    istabs = sorted(mc1['istab'].unique())
    speeds = sorted(mc1['wind'].unique())
    dists = sorted(mc1['x'].unique())
    
    for istab in istabs:
        for speed in speeds:
            bb = (mc1['istab']==istab) & (mc1['wind']==speed)
            mc2 = mc1.loc[bb]
            if len(mc2)>0:
                
                fig, ax = plt.subplots()
                titl = f'{"ABCDEFG"[istab]}, {speed}'
                print(titl)
                ax.set_title(titl)
                ax.set_xlabel('x')
                ax.set_ylabel('sigma')
                ax.set_xscale('log')
                ax.set_yscale('log')
                
                for c in 'xyz':
                    ax.plot(mc2['x'], mc2[f'sig_{c}'], '-+', label=f'sig_{c}')
                ax.legend()
                    
                
    
if __name__ == '__main__':    
    
    pass
    
    fn0 = 'hpac_dispersion_coefs_original.csv'
    fn1 = 'hpac_dispersion_coefs_2nd_law.csv'
    fn2 = 'hpac_dispersion_coefs_1m.csv'   # xmin is 1 meter for this file
    fn3 = 'hpac_dispersion_coefs.csv'      # xmin is 1 mm for this file
    
    if False:
        Test_Second_Law(fn0, plot=True)
        Test_Second_Law(fn1, plot=True)
        
    if False:
        fixed1 = fix_second_law(fn0, fout=fn1)   # fix sigmas
        fixed2 = fix_second_law(fn1)             # test the fixed file
        
    if False:
        fix_missing_distances(fn1, fn2)   # fix distances
        fix_missing_distances(fn2)        # check the fixed file
        
    if False:
        add_sigma_near_source(fn2, fn3)
        
    if True:
        plot_hpac_coefs(fn3)