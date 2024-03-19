# -*- coding: utf-8 -*-
"""
Created on Wed Mar  6 10:01:00 2024

@author: Stage

Read parameters used for a run.

The input file contains one or more blocks of data.

Each block of data consistes of:
    
    BEGIN: name, typ
    
    Followed by one or more lines of data.
    
    Commas are used to separate tokens in the data.
    If possible, values are converted to float.
    Empty lines are ignored.
    Anything after a # is ignored.
    
Output is a dictionary containing {key : values_from_block}.    

Where typ can be 'dict', 'array', 'csv', 'list', or 'text':
    
    typ = dict:
        Returns a dictionary.
        
        Each line of data contains:
            key : value(s)
        
    typ = array (default):
        Returns an array of values from lines in the block.
        
        If a line contains one token, the value of the
        token is appended to the array.
        If a line contains more than one token,
        an array containing the values of the tokens from the line
        is appended to the array.
        
    typ = csv:
        Returns a pandas.DataFrame.
        
        The first line of the block must contain column headings.
        Remaining lines of the block contain data.
        
    typ = list:
        Returns a 1D list containing values of all tokens in the block.
        
    typ = text:
        Returns a string containing the verbatim contents of the block.
        
        White space and comments are included.


"""

import pandas as pd, numpy as np
import os

import utils

def read_run_parametersXXX(fname):
    '''
    Read file containing parameters used when running HPAC.
    Old version. can only read dictionary information.
    '''
    runpar = {}
    
    if not os.path.isfile((fname)):
        runpar['Error'] = 'File does not exist'
        runpar['fname'] = fname
        return runpar
    
    with open(fname) as f:
        lines = f.read().splitlines()
        
    for line in lines:
        line = line.split('#')[0]   # remove comments
        line = line.strip()
        if len(line) > 0:
            
            cc = [c.strip() for c in line.split(':')]
            if len(cc) > 1:
                runpar[cc[0]] = [utils.try_float(s.strip()) for s in cc[1].split(',')]
    
    return runpar

def get_values(val):
    '''
    Split a line into tokens and convert tokens to float if possible.
    If the line contains one token, the value of the token is returned.
    If the line contians more than one token, an array is returned
    containing the token values.
    '''
    toks = val.strip().split(',')
    
    if len(toks) <= 1:
        result = utils.try_float(val)
        
    else:
        result = [utils.try_float(t.strip()) for t in toks]
    
    return result
      
         
               
def read_run_parameters(fname):
    '''
    Read file containing parameters used for run
    '''
    runpar = {}
    
    if not os.path.isfile((fname)):
        runpar['Error'] = 'File does not exist'
        runpar['fname'] = fname
        return runpar
    
    with open(fname) as f:
        lines = f.read().splitlines()
        
    # Ensure the last block gets written, even if there is no 'END:' in the file.
    lines.append('END:')
    
    typ = 'array'
    name = 'values'
    data = {}
    results = {}
    
    for line0 in lines:
        line = line0.split('#')[0]   # remove comments
        line = line.strip()
        if len(line) > 0 or typ in ['text']:
            
            key, sep, val = line.partition(':')
            key = key.strip()
            val = val.strip()
            
            print(line)
            print(key, val)
            nnn=-999
            
            if key in ['BEGIN', 'END']:
                
                # Process existing block
                
                if len(data)>0:  
                    if typ in ['dict']:
                        results[name] = data
                    elif typ in ['csv']:
                        results[name] = pd.DataFrame(data[1:], columns = data[0])
                    elif typ in ['array', 'list']:
                        results[name] = data
                    else:
                        results[name] = data
                        
                    print(f'results[{name}] = \n{results[name]}')
                    print(')')
                
                # Initialize for new block
                if key in ['BEGIN']:
                    name, sep, typ = val.partition(',')
                    name = name.strip()
                    typ = typ.strip()
                    if typ == '':
                        print('\n================================================')
                        print('WARNING: Comma needed between name and typ')
                        print('Using typ = "array"')
                              
                        print(line)
                        print('\n================================================')
                        typ - 'array'
                        
                    if typ in ['dict']:
                        data = {}
                    elif typ in ['csv', 'array', 'list']:
                        data = []  
                    elif typ in ['text']:
                        data = ''
                    else:
                        data = []
            
                else:   # key == 'END'
                    return results
                
            else:
                
                if type(data) == dict:                    
                    data[key] = get_values(val)                    
                elif typ in ['array']:
                    data.append(get_values(key))
                elif typ in ['list']:
                    vv = get_values(key)
                    if type(vv) in [str, float]:
                        vv = [vv]
                    data += vv
                elif typ in ['text']:
                    if len(data)<1:
                        data = line0
                    else:
                        data += '\n' + line0
                else:
                    data.append(get_values(key))
                    
    
    return results


                
if __name__ == '__main__':

    pass
    
    #fname = 'parametersXXX.txt'
    #runpar = read_run_parametersXXX(fname)
    
    fname = 'parameters.txt'
    runpar = read_run_parameters(fname)
    
    for k, v in runpar.items():
        print(f'\nkey = {k}')
        print(v)
    
    if False:
        ss = 'acd:def'
        
        print('acd:def'.partition(':'))
        print('acddef'.partition(':'))
        print(':acddef'.partition(':'))
        print('acddef:'.partition(':'))
        
        print('acd   :def'.partition(':'))
        print('acd:  def'.partition(':'))
        print('acd  :  def'.partition(':'))
    
    if False:
        
        get_values('1.0, min')
        
        get_values(',istab,U,zi,stab,imet,Q_mg,zplume,dur,sig_x0,sig_y0,sig_z0,title,isrc,y,z,t,iloc,icurve,x')
        
        get_values('0,3,3,100000,D,0,1000000.0,0,0,0,0,0.0,sample,0,0,0,5000,0,0,0.001')
        
        
        
    
    
    
    
    